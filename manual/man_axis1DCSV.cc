// ============================================================================
// Arquivo: man_axis1DCSV.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Programa de manual para exportar uma malha 1D em CSV.
// Autor: João Flávio Vieira de Vasconcellos
//
// SPDX-FileCopyrightText: 2026 João Flávio Vieira de Vasconcellos
// SPDX-License-Identifier: BSD-3-Clause
//
// Este arquivo faz parte do FVGridMaker.
//
// Licença: BSD 3-Clause.
// É permitido usar, copiar, modificar e redistribuir este arquivo, em código-fonte
// ou forma binária, com ou sem modificações, desde que sejam preservados os avisos
// de copyright, esta identificação de licença e as condições descritas no arquivo
// LICENSE.
//
// O nome do autor, de colaboradores ou de instituições associadas ao projeto não
// pode ser usado para endossar ou promover produtos derivados sem autorização
// prévia por escrito.
//
// Este software é fornecido sem garantias de qualquer natureza. Consulte o arquivo
// LICENSE, na raiz do repositório, para o texto completo da licença.
// ============================================================================

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

#include <FVGridMaker/FVGridMaker.h>

namespace {

using Scalar = fvgrid::Real;
using Axis = fvgrid::BasicAxis1D<Scalar>;

[[nodiscard]] bool check_geometric_consistency(const Axis& axis) {
    Scalar sum_dx = Scalar{0.0};

    for (fvgrid::Size p = 0; p < axis.num_cells(); ++p) {
        sum_dx += axis.cell_length(p);
    }

    const Scalar expected_length = axis.length();
    const Scalar error = std::abs(sum_dx - expected_length);
    const Scalar tolerance = Scalar{1.0e-12};

    std::cout << "\nTeste de consistência geométrica\n";
    std::cout << "================================\n";
    std::cout << "O bloco abaixo verifica se a soma dos comprimentos dos volumes\n";
    std::cout << "reproduz o comprimento total do domínio antes da exportação.\n\n";

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "soma dos dx      : " << sum_dx << '\n';
    std::cout << "comprimento eixo : " << expected_length << '\n';

    std::cout << std::scientific << std::setprecision(6);
    std::cout << "erro absoluto    : " << error << '\n';
    std::cout << "tolerância       : " << tolerance << '\n';

    if (error <= tolerance) {
        std::cout << "resultado        : OK\n";
        return true;
    }

    std::cout << "resultado        : FALHOU\n";
    return false;
}

void write_axis_csv(
    const Axis& axis,
    const std::filesystem::path& filepath
) {
    std::ofstream file{filepath};

    if (!file) {
        throw std::runtime_error("Não foi possível abrir o arquivo CSV.");
    }

    file << std::fixed << std::setprecision(16);

    file << "volume,"
         << "face_oeste,"
         << "centro,"
         << "face_leste,"
         << "dx,"
         << "distancia_centro_face_oeste,"
         << "distancia_face_leste_centro\n";

    for (fvgrid::Size p = 0; p < axis.num_cells(); ++p) {
        const Scalar west = axis.west_face(p);
        const Scalar center = axis.center(p);
        const Scalar east = axis.east_face(p);

        file << p << ','
             << west << ','
             << center << ','
             << east << ','
             << axis.cell_length(p) << ','
             << center - west << ','
             << east - center
             << '\n';
    }

    if (!file) {
        throw std::runtime_error("Erro ao escrever o arquivo CSV.");
    }
}

void print_axis_summary(
    const Axis& axis,
    const std::filesystem::path& csv_path
) {
    std::cout << std::fixed << std::setprecision(6);

    std::cout << "\nResumo compacto da malha\n";
    std::cout << "========================\n";
    std::cout << "O bloco abaixo apresenta uma visão compacta da malha gerada:\n";
    std::cout << "padrão de centralização, quantidade de faces e volumes,\n";
    std::cout << "limites físicos, comprimento total do domínio e arquivo de saída.\n\n";

    std::cout << "padrão            : " << axis.pattern_name() << '\n';
    std::cout << "número de faces   : " << axis.num_faces() << '\n';
    std::cout << "número de volumes : " << axis.num_cells() << '\n';
    std::cout << "xmin              : " << axis.xmin() << '\n';
    std::cout << "xmax              : " << axis.xmax() << '\n';
    std::cout << "comprimento       : " << axis.length() << '\n';
    std::cout << "arquivo CSV       : " << csv_path.string() << '\n';
}

void print_csv_preview(const Axis& axis) {
    constexpr int id_width = 6;
    constexpr int value_width = 14;

    std::cout << "\nInformações geométricas dos volumes\n";
    std::cout << "===================================\n";
    std::cout << "O bloco abaixo mostra no terminal as colunas geométricas\n";
    std::cout << "principais gravadas no arquivo CSV: face oeste, centro,\n";
    std::cout << "face leste e comprimento do volume.\n\n";

    std::cout << std::fixed << std::setprecision(6);

    std::cout << std::right
              << std::setw(id_width) << "p"
              << std::setw(value_width) << "face_oeste"
              << std::setw(value_width) << "centro"
              << std::setw(value_width) << "face_leste"
              << std::setw(value_width) << "dx"
              << '\n';

    std::cout << std::string(id_width + 4 * value_width, '-') << '\n';

    for (fvgrid::Size p = 0; p < axis.num_cells(); ++p) {
        std::cout << std::right
                  << std::setw(id_width) << p
                  << std::setw(value_width) << axis.west_face(p)
                  << std::setw(value_width) << axis.center(p)
                  << std::setw(value_width) << axis.east_face(p)
                  << std::setw(value_width) << axis.cell_length(p)
                  << '\n';
    }
}

}  // namespace

int main() {
    //
    // Este exemplo mostra como exportar uma malha 1D para CSV.
    //
    // O CSV é útil para inspeção rápida em planilhas, scripts Python, gnuplot ou
    // ferramentas simples de comparação. Ele não substitui formatos próprios
    // para visualização científica, mas é adequado para conferência textual.
    //
    // A malha escolhida é uma malha de Roberts com padrão VolumeCentered1D.
    // Assim, as faces são não uniformes e os centros ficam nos pontos médios
    // dos volumes.
    //
    // O arquivo CSV é gravado no diretório corrente de execução. Quando este
    // programa é executado pelo alvo run_man_axis1DCSV do CMake, esse diretório
    // normalmente é a pasta build do projeto.
    //

    const fvgrid::Size nvol = 10;
    const Scalar xinit = Scalar{0.0};
    const Scalar xfinal = Scalar{1.0};
    const Scalar beta = Scalar{2.0};

    const std::filesystem::path csv_filename{"axis1d_roberts.csv"};
    const std::filesystem::path csv_path =
        std::filesystem::absolute(csv_filename);

    const Axis axis = fvgrid::roberts_axis_1d(
        fvgrid::NVol{nvol},
        fvgrid::Length{xfinal - xinit},
        fvgrid::XInit{xinit},
        fvgrid::Beta{beta},
        fvgrid::VolumeCentered1D{}
    );

    print_axis_summary(axis, csv_path);
    print_csv_preview(axis);

    if (!check_geometric_consistency(axis)) {
        return 1;
    }

    write_axis_csv(axis, csv_path);

    std::cout << "\nExportação CSV\n";
    std::cout << "==============\n";
    std::cout << "Arquivo gravado com sucesso.\n";
    std::cout << "Caminho completo: " << csv_path.string() << '\n';

    return 0;
}