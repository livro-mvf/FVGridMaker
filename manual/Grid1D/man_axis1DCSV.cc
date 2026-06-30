// ============================================================================
// Arquivo: man_axis1DCSV.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Programa de manual para exportar uma malha 1D em CSV. Mostra
//            primeiro o escritor oficial da biblioteca e, em seguida, um CSV
//            customizado por volume escrito pelo usuário.
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

#include <FVGridMaker/FVGridMaker.h>

namespace {

using Scalar = fvgrid::Real;
using Axis = fvgrid::BasicAxis1D<Scalar>;

[[nodiscard]] bool check_geometric_consistency(const Axis& axis) {
    const std::span<const Scalar> dx = axis.cell_lengths();

    Scalar sum_dx = Scalar{0.0};

    for (const Scalar value : dx) {
        sum_dx += value;
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

//
// CSV customizado, escrito pelo usuário.
//
// Este formato é diferente do produzido pelo Axis1DCSVWriter da biblioteca.
// Aqui, cada linha representa um VOLUME e inclui colunas extras com as duas
// distâncias entre o centro e as faces. É um exemplo de como estender a saída
// quando o formato oficial não atende a uma necessidade específica.
//
void write_custom_volume_csv(
    const Axis& axis,
    const std::filesystem::path& filepath
) {
    std::ofstream file{filepath};

    if (!file) {
        throw std::runtime_error("Não foi possível abrir o arquivo CSV customizado.");
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
        throw std::runtime_error("Erro ao escrever o arquivo CSV customizado.");
    }
}

void print_axis_summary(const Axis& axis) {
    std::cout << std::fixed << std::setprecision(6);

    std::cout << "\nResumo compacto da malha\n";
    std::cout << "========================\n";
    std::cout << "padrão            : " << axis.pattern_name() << '\n';
    std::cout << "número de faces   : " << axis.num_faces() << '\n';
    std::cout << "número de volumes : " << axis.num_cells() << '\n';
    std::cout << "xmin              : " << axis.xmin() << '\n';
    std::cout << "xmax              : " << axis.xmax() << '\n';
    std::cout << "comprimento       : " << axis.length() << '\n';
}

}  // namespace

int main() {
    //
    // Este exemplo mostra duas formas de exportar uma malha 1D para CSV.
    //
    // A primeira é o caminho oficial: o escritor Axis1DCSVWriter, que já vem
    // com a biblioteca. Ele produz um CSV centrado em faces, com colunas
    // i, xF, xC, Dx, dx.
    //
    // A segunda é um CSV customizado, escrito pelo próprio programa, com uma
    // linha por volume e colunas extras. Ela existe para mostrar como gerar
    // outro layout quando o formato oficial não atende ao que se precisa.
    //
    // A malha escolhida é uma malha de Roberts com padrão VolumeCentered1D.
    // Assim, as faces são não uniformes e o CSV fica mais informativo do que
    // seria com uma malha uniforme.
    //
    // Os dois arquivos são gravados no diretório corrente de execução. Quando
    // este programa é executado pelo alvo run_man_axis1DCSV do CMake, esse
    // diretório normalmente é a pasta build do projeto.
    //

    const fvgrid::Size nvol = 10;
    const Scalar xinit = Scalar{0.0};
    const Scalar xfinal = Scalar{1.0};
    const Scalar beta = Scalar{2.0};

    const std::filesystem::path official_csv_path =
        std::filesystem::absolute("axis1d_roberts_oficial.csv");

    const std::filesystem::path custom_csv_path =
        std::filesystem::absolute("axis1d_roberts_volume.csv");

    const Axis axis = fvgrid::roberts_axis_1d(
        fvgrid::NVol{nvol},
        fvgrid::Length{xfinal - xinit},
        fvgrid::XInit{xinit},
        fvgrid::Beta{beta},
        fvgrid::VolumeCentered1D{}
    );

    print_axis_summary(axis);

    if (!check_geometric_consistency(axis)) {
        return 1;
    }

    //
    // Exportação 1: escritor oficial da biblioteca.
    //
    // Axis1DCSVWriter::write recebe o eixo e o caminho do arquivo. A biblioteca
    // cuida da abertura do arquivo, da precisão numérica e do cabeçalho. Não é
    // preciso montar o CSV à mão.
    //
    fvgrid::Axis1DCSVWriter::write(axis, official_csv_path);

    std::cout << "\nExportação 1: CSV oficial (Axis1DCSVWriter)\n";
    std::cout << "===========================================\n";
    std::cout << "Formato centrado em faces: i, xF, xC, Dx, dx.\n";
    std::cout << "Caminho completo: " << official_csv_path.string() << '\n';

    //
    // Exportação 2: CSV customizado por volume.
    //
    // Quando o formato oficial não basta, o usuário pode escrever o próprio CSV
    // a partir dos dados do eixo, como na função write_custom_volume_csv.
    //
    write_custom_volume_csv(axis, custom_csv_path);

    std::cout << "\nExportação 2: CSV customizado por volume\n";
    std::cout << "========================================\n";
    std::cout << "Uma linha por volume, com distâncias centro-face extras.\n";
    std::cout << "Caminho completo: " << custom_csv_path.string() << '\n';

    return 0;
}