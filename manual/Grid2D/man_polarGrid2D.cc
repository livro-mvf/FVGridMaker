// ============================================================================
// Arquivo: man_polarGrid2D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Programa de manual para gerar uma malha 2D em coordenadas polares.
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
#include <numbers>
#include <string>

#include <FVGridMaker/FVGridMaker.h>

int main() {
    //
    // Neste programa, a malha lógica é construída nas coordenadas (r, theta).
    //
    // O domínio lógico é:
    //
    //     r     em [1, 2],
    //     theta em [0, pi/2].
    //
    // A malha representa um setor anular no plano físico. Por isso, a área
    // lógica dr * dtheta não coincide com a área física da célula.
    //

    using Scalar = fvgrid::Real;
    using Axis = fvgrid::BasicAxis1D<Scalar>;
    using Grid = fvgrid::BasicStructuredGrid2D<Scalar>;

    const Scalar pi = std::numbers::pi_v<Scalar>;

    const fvgrid::Size nvol_r = 4;
    const fvgrid::Size nvol_theta = 6;

    const Scalar r_inner = Scalar{1.0};
    const Scalar r_outer = Scalar{2.0};

    const Scalar theta_begin = Scalar{0.0};
    const Scalar theta_end = Scalar{0.5} * pi;

    const Axis r_axis = fvgrid::uniform_axis_1d<Scalar>(
        nvol_r,
        r_inner,
        r_outer
    );

    const Axis theta_axis = fvgrid::uniform_axis_1d<Scalar>(
        nvol_theta,
        theta_begin,
        theta_end
    );

    //
    // O terceiro argumento define o sistema de coordenadas usado pela malha.
    // Aqui, as coordenadas lógicas são r e theta, mas os pontos físicos são
    // obtidos pelo mapeamento polar:
    //
    //     x = r cos(theta),
    //     y = r sin(theta).
    //

    const Grid grid{
        r_axis,
        theta_axis,
        fvgrid::BasicPolarCoordinates2D<Scalar>{}
    };

    //
    // Usamos formato fixo para manter a saída principal estável no manual.
    //

    std::cout << std::fixed << std::setprecision(6);

    //
    // Primeiro, imprimimos um resumo compacto da malha polar.
    //

    std::cout << "\nSetores anulares em coordenadas polares\n";
    std::cout << "=======================================\n";
    std::cout << "O bloco abaixo apresenta uma malha estruturada em coordenadas\n";
    std::cout << "polares. A malha lógica é retangular em (r, theta), mas a malha\n";
    std::cout << "física representa um setor anular no plano xy.\n\n";

    std::cout << "sistema de coordenadas : "
              << grid.coordinate_system_name() << '\n';
    std::cout << "coordenadas lógicas    : "
              << grid.first_coordinate_name() << ", "
              << grid.second_coordinate_name() << '\n';
    std::cout << "vtk rectilinear        : "
              << (grid.vtk_rectilinear() ? "sim" : "nao") << '\n';
    std::cout << "número de células em r : " << grid.num_cells_x() << '\n';
    std::cout << "número de células em t : " << grid.num_cells_y() << '\n';
    std::cout << "número total de células: " << grid.num_cells() << '\n';
    std::cout << "r mínimo               : " << grid.xmin() << '\n';
    std::cout << "r máximo               : " << grid.xmax() << '\n';
    std::cout << "theta mínimo           : " << grid.ymin() << '\n';
    std::cout << "theta máximo           : " << grid.ymax() << '\n';

    //
    // Agora percorremos as células.
    //
    // Para cada célula, comparamos:
    //
    //     area_logica = dr * dtheta,
    //
    // com
    //
    //     medida = área física do setor anular da célula.
    //
    // Em coordenadas polares, essas grandezas não coincidem.
    //

    constexpr int id_width = 6;
    constexpr int value_width = 14;

    Scalar sum_logical_area = Scalar{0.0};
    Scalar sum_measure = Scalar{0.0};

    std::cout << "\nInformações geométricas dos setores\n";
    std::cout << "===================================\n";
    std::cout << "Cada linha mostra uma célula polar. O centro lógico é dado\n";
    std::cout << "por (r, theta), enquanto o centro físico é o ponto correspondente\n";
    std::cout << "no plano xy. A área lógica e a medida física são diferentes.\n\n";

    std::cout << std::right
              << std::setw(id_width) << "i"
              << std::setw(id_width) << "j"
              << std::setw(id_width) << "k"
              << std::setw(value_width) << "r_centro"
              << std::setw(value_width) << "theta"
              << std::setw(value_width) << "x_fisico"
              << std::setw(value_width) << "y_fisico"
              << std::setw(value_width) << "area_logica"
              << std::setw(value_width) << "medida"
              << '\n';

    std::cout << std::string(3 * id_width + 6 * value_width, '-') << '\n';

    for (fvgrid::Size j = 0; j < grid.num_cells_y(); ++j) {
        for (fvgrid::Size i = 0; i < grid.num_cells_x(); ++i) {
            const fvgrid::Size k = grid.linear_cell_index(i, j);

            const Scalar r = grid.first_center(i);
            const Scalar theta = grid.second_center(j);

            const auto physical_center = grid.physical_cell_center(i, j);

            const Scalar logical_area = grid.cell_logical_area(i, j);
            const Scalar measure = grid.cell_measure(i, j);

            sum_logical_area += logical_area;
            sum_measure += measure;

            std::cout << std::right
                      << std::setw(id_width) << i
                      << std::setw(id_width) << j
                      << std::setw(id_width) << k
                      << std::setw(value_width) << r
                      << std::setw(value_width) << theta
                      << std::setw(value_width) << physical_center.x
                      << std::setw(value_width) << physical_center.y
                      << std::setw(value_width) << logical_area
                      << std::setw(value_width) << measure
                      << '\n';
        }
    }

    //
    // Teste simples de consistência geométrica.
    //
    // A área analítica de um setor anular é:
    //
    //     A = 0.5 * (r_outer^2 - r_inner^2) * (theta_end - theta_begin).
    //
    // A soma das medidas físicas das células deve recuperar essa área.
    // A soma das áreas lógicas, por outro lado, mede apenas a área do retângulo
    // lógico em (r, theta).
    //

    const Scalar expected_measure =
        Scalar{0.5}
        * (r_outer * r_outer - r_inner * r_inner)
        * (theta_end - theta_begin);

    const Scalar expected_logical_area =
        (r_outer - r_inner) * (theta_end - theta_begin);

    const Scalar measure_error = std::abs(sum_measure - expected_measure);
    const Scalar logical_error =
        std::abs(sum_logical_area - expected_logical_area);

    const Scalar difference_between_sums =
        std::abs(sum_measure - sum_logical_area);

    const Scalar tolerance = Scalar{1.0e-12};

    std::cout << "\nTeste de consistência geométrica\n";
    std::cout << "================================\n";
    std::cout << "O bloco abaixo compara a soma das áreas lógicas com a área\n";
    std::cout << "do retângulo lógico e a soma das medidas físicas com a área\n";
    std::cout << "analítica do setor anular.\n\n";

    std::cout << "soma das áreas lógicas       : " << sum_logical_area << '\n';
    std::cout << "área lógica esperada         : " << expected_logical_area << '\n';
    std::cout << "soma das medidas físicas     : " << sum_measure << '\n';
    std::cout << "área física esperada         : " << expected_measure << '\n';
    std::cout << "diferença entre as duas somas: "
              << difference_between_sums << '\n';

    std::cout << std::scientific;
    std::cout << "erro na área lógica          : " << logical_error << '\n';
    std::cout << "erro na medida física        : " << measure_error << '\n';
    std::cout << "tolerância                   : " << tolerance << '\n';

    const bool passed =
        logical_error <= tolerance &&
        measure_error <= tolerance &&
        difference_between_sums > tolerance;

    if (passed) {
        std::cout << "resultado                    : OK\n";
    } else {
        std::cout << "resultado                    : FALHOU\n";
        return 1;
    }

    return 0;
}

