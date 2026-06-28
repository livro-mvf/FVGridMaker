// ============================================================================
// Arquivo: man_customCoordinates1D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Programa de manual para construir malhas 1D com coordenadas dadas.
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
#include <iomanip>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <FVGridMaker/FVGridMaker.h>

namespace {

using Scalar = fvgrid::Real;
using Axis = fvgrid::BasicAxis1D<Scalar>;

constexpr int id_width = 6;
constexpr int value_width = 14;

void print_axis_report(
    const Axis& axis,
    std::string_view title,
    std::string_view explanation
) {
    std::cout << std::fixed << std::setprecision(6);

    std::cout << "\n" << title << "\n";
    std::cout << std::string(title.size(), '=') << "\n";
    std::cout << explanation << "\n\n";

    std::cout << "Resumo automático gerado pelo operador <<\n";
    std::cout << "========================================\n\n";

    std::cout << axis << '\n';

    std::cout << "\nResumo manual da malha gerada\n";
    std::cout << "=============================\n";
    std::cout << "O bloco abaixo imprime algumas propriedades globais da malha.\n\n";

    std::cout << "número de volumes : " << axis.num_cells() << '\n';
    std::cout << "número de faces   : " << axis.num_faces() << '\n';
    std::cout << "xmin              : " << axis.xmin() << '\n';
    std::cout << "xmax              : " << axis.xmax() << '\n';
    std::cout << "comprimento       : " << axis.length() << '\n';
    std::cout << "pattern           : " << axis.pattern_name() << '\n';

    std::cout << "\nCoordenadas das faces\n";
    std::cout << "=====================\n\n";

    std::cout << std::right
              << std::setw(id_width) << "i"
              << std::setw(value_width) << "x_face"
              << '\n';

    std::cout << std::string(id_width + value_width, '-') << '\n';

    for (fvgrid::Size i = 0; i < axis.num_faces(); ++i) {
        std::cout << std::right
                  << std::setw(id_width) << i
                  << std::setw(value_width) << axis.face(i)
                  << '\n';
    }

    std::cout << "\nInformações geométricas dos volumes\n";
    std::cout << "===================================\n\n";

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

[[nodiscard]] bool check_geometric_consistency(
    const Axis& axis,
    std::string_view label
) {
    Scalar sum_dx = Scalar{0.0};

    for (fvgrid::Size p = 0; p < axis.num_cells(); ++p) {
        sum_dx += axis.cell_length(p);
    }

    const Scalar expected_length = axis.length();
    const Scalar error = std::abs(sum_dx - expected_length);
    const Scalar tolerance = Scalar{1.0e-12};

    std::cout << "\nTeste de consistência geométrica\n";
    std::cout << "================================\n";
    std::cout << "Caso testado      : " << label << '\n';
    std::cout << "soma dos dx       : " << std::fixed << std::setprecision(6)
              << sum_dx << '\n';
    std::cout << "comprimento eixo  : " << expected_length << '\n';

    const std::ios_base::fmtflags old_flags = std::cout.flags();
    const std::streamsize old_precision = std::cout.precision();

    std::cout << std::scientific << std::setprecision(6);
    std::cout << "erro absoluto     : " << error << '\n';
    std::cout << "tolerância        : " << tolerance << '\n';

    std::cout.flags(old_flags);
    std::cout.precision(old_precision);

    if (error <= tolerance) {
        std::cout << "resultado         : OK\n";
        return true;
    }

    std::cout << "resultado         : FALHOU\n";
    return false;
}

}  // namespace

int main() {
    //
    // Este exemplo mostra dois modos de construir uma malha 1D customizada.
    //
    // No primeiro caso, o usuário fornece as faces. A biblioteca reconstrói os
    // centros com o padrão VolumeCentered1D.
    //
    // No segundo caso, o usuário fornece os centros. A biblioteca reconstrói as
    // faces com o padrão FaceCentered1D. Nesse caso, é necessário informar
    // explicitamente os limites do domínio.
    //

    const Scalar xinit = Scalar{0.0};
    const Scalar xfinal = Scalar{1.0};

    //
    // Caso 1: faces fornecidas pelo usuário.
    //
    // As faces são as coordenadas primárias. O padrão VolumeCentered1D calcula
    // os centros como pontos médios dos volumes.
    //

    std::vector<Scalar> user_faces{
        Scalar{0.00},
        Scalar{0.04},
        Scalar{0.10},
        Scalar{0.19},
        Scalar{0.31},
        Scalar{0.46},
        Scalar{0.64},
        Scalar{0.79},
        Scalar{0.90},
        Scalar{0.97},
        Scalar{1.00}
    };

    const Axis axis_from_faces = fvgrid::custom_axis_1d(
        fvgrid::BasicCoordinates1D<Scalar>::faces(std::move(user_faces)),
        fvgrid::VolumeCentered1D{}
    );

    print_axis_report(
        axis_from_faces,
        "Caso 1: malha criada a partir de faces fornecidas",
        "As faces foram dadas diretamente pelo usuário. "
        "Os centros foram reconstruídos pelo padrão VolumeCentered1D."
    );

    if (!check_geometric_consistency(axis_from_faces, "faces fornecidas")) {
        return 1;
    }

    //
    // Caso 2: centros fornecidos pelo usuário.
    //
    // Os centros são as coordenadas primárias. O padrão FaceCentered1D calcula
    // as faces internas a partir dos centros e usa os limites do domínio como
    // faces externas.
    //

    std::vector<Scalar> user_centers{
        Scalar{0.03},
        Scalar{0.09},
        Scalar{0.17},
        Scalar{0.28},
        Scalar{0.42},
        Scalar{0.58},
        Scalar{0.72},
        Scalar{0.83},
        Scalar{0.91},
        Scalar{0.97}
    };

    const auto domain = fvgrid::BasicDomain1D<Scalar>::from_bounds(
        fvgrid::BasicXInit<Scalar>{xinit},
        fvgrid::BasicXFinal<Scalar>{xfinal}
    );

    const Axis axis_from_centers = fvgrid::custom_axis_1d(
        fvgrid::BasicCoordinates1D<Scalar>::centers(std::move(user_centers)),
        fvgrid::FaceCentered1D{},
        domain
    );

    print_axis_report(
        axis_from_centers,
        "Caso 2: malha criada a partir de centros fornecidos",
        "Os centros foram dados diretamente pelo usuário. "
        "As faces foram reconstruídas pelo padrão FaceCentered1D."
    );

    if (!check_geometric_consistency(axis_from_centers, "centros fornecidos")) {
        return 1;
    }

    return 0;
}