// ============================================================================
// Arquivo: man_incompleteAnnulus2D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Programa de manual para gerar uma arruela com setor removido.
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

namespace {

bool is_sample_cell(
    fvgrid::Size i,
    fvgrid::Size j,
    fvgrid::Size nx,
    fvgrid::Size ny
) {
    const bool radial_sample = (i == 0) || (i + 1 == nx);
    const bool angular_sample = (j == 0) || (j == ny / 2) || (j + 1 == ny);

    return radial_sample && angular_sample;
}

}  // namespace

int main() {
    //
    // Este programa gera uma arruela com um setor angular removido.
    //
    // A geometria não exige um novo gerador: ela é uma malha polar parcial.
    // Em vez de usar theta em [0, 2*pi], usamos apenas o intervalo angular
    // correspondente à parte restante da arruela.
    //
    // Neste exemplo, removemos um setor de 60 graus centrado no eixo x positivo.
    // A região mantida é:
    //
    //     r     em [1, 2],
    //     theta em [pi/6, 11*pi/6].
    //

    using Scalar = fvgrid::Real;
    using Axis = fvgrid::BasicAxis1D<Scalar>;
    using Grid = fvgrid::BasicStructuredGrid2D<Scalar>;

    const Scalar pi = std::numbers::pi_v<Scalar>;

    const fvgrid::Size nvol_r = 5;
    const fvgrid::Size nvol_theta = 18;

    const Scalar r_inner = Scalar{1.0};
    const Scalar r_outer = Scalar{2.0};

    const Scalar removed_angle = pi / Scalar{3.0};

    const Scalar theta_begin = Scalar{0.5} * removed_angle;
    const Scalar theta_end = Scalar{2.0} * pi - Scalar{0.5} * removed_angle;

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

    const Grid grid{
        r_axis,
        theta_axis,
        fvgrid::BasicPolarCoordinates2D<Scalar>{}
    };

    std::cout << std::fixed << std::setprecision(6);

    std::cout << "\nArruela com setor angular removido\n";
    std::cout << "==================================\n";
    std::cout << "O bloco abaixo apresenta uma arruela parcial gerada como\n";
    std::cout << "malha polar. O setor removido tem abertura de 60 graus e\n";
    std::cout << "está centrado no eixo x positivo.\n\n";

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
    std::cout << "theta inicial          : " << grid.ymin() << '\n';
    std::cout << "theta final            : " << grid.ymax() << '\n';
    std::cout << "abertura removida      : " << removed_angle << '\n';

    constexpr int id_width = 6;
    constexpr int value_width = 14;

    Scalar sum_logical_area = Scalar{0.0};
    Scalar sum_measure = Scalar{0.0};

    std::cout << "\nAmostra de células\n";
    std::cout << "==================\n";
    std::cout << "A tabela abaixo imprime apenas algumas células, para manter\n";
    std::cout << "a saída compacta. A consistência geométrica é testada usando\n";
    std::cout << "todas as células da malha.\n\n";

    std::cout << std::right
              << std::setw(id_width) << "i"
              << std::setw(id_width) << "j"
              << std::setw(id_width) << "k"
              << std::setw(value_width) << "r_centro"
              << std::setw(value_width) << "theta"
              << std::setw(value_width) << "x_map"
              << std::setw(value_width) << "y_map"
              << std::setw(value_width) << "area_logica"
              << std::setw(value_width) << "medida"
              << '\n';

    std::cout << std::string(3 * id_width + 6 * value_width, '-') << '\n';

    for (fvgrid::Size j = 0; j < grid.num_cells_y(); ++j) {
        for (fvgrid::Size i = 0; i < grid.num_cells_x(); ++i) {
            const fvgrid::Size k = grid.linear_cell_index(i, j);

            const Scalar logical_area = grid.cell_logical_area(i, j);
            const Scalar measure = grid.cell_measure(i, j);

            sum_logical_area += logical_area;
            sum_measure += measure;

            if (is_sample_cell(i, j, grid.num_cells_x(), grid.num_cells_y())) {
                const Scalar r = grid.first_center(i);
                const Scalar theta = grid.second_center(j);
                const auto mapped_point = grid.physical_cell_center(i, j);

                std::cout << std::right
                          << std::setw(id_width) << i
                          << std::setw(id_width) << j
                          << std::setw(id_width) << k
                          << std::setw(value_width) << r
                          << std::setw(value_width) << theta
                          << std::setw(value_width) << mapped_point.x
                          << std::setw(value_width) << mapped_point.y
                          << std::setw(value_width) << logical_area
                          << std::setw(value_width) << measure
                          << '\n';
            }
        }
    }

    const Scalar expected_measure =
        Scalar{0.5}
        * (r_outer * r_outer - r_inner * r_inner)
        * (theta_end - theta_begin);

    const Scalar expected_logical_area =
        (r_outer - r_inner) * (theta_end - theta_begin);

    const Scalar measure_error = std::abs(sum_measure - expected_measure);
    const Scalar logical_error =
        std::abs(sum_logical_area - expected_logical_area);

    const Scalar tolerance = Scalar{1.0e-12};

    std::cout << "\nTeste de consistência geométrica\n";
    std::cout << "================================\n";
    std::cout << "O bloco abaixo verifica se a soma das medidas físicas recupera\n";
    std::cout << "a área da arruela restante depois da remoção do setor angular.\n\n";

    std::cout << "soma das áreas lógicas   : " << sum_logical_area << '\n';
    std::cout << "área lógica esperada     : " << expected_logical_area << '\n';
    std::cout << "soma das medidas físicas : " << sum_measure << '\n';
    std::cout << "área física esperada     : " << expected_measure << '\n';

    std::cout << std::scientific;
    std::cout << "erro na área lógica      : " << logical_error << '\n';
    std::cout << "erro na medida física    : " << measure_error << '\n';
    std::cout << "tolerância               : " << tolerance << '\n';

    fvgrid::write_vtk(grid, "malha_arruela_recortada.vtk");

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "arquivo VTK              : malha_arruela_recortada.vtk\n";

    const bool passed =
        logical_error <= tolerance &&
        measure_error <= tolerance;

    if (passed) {
        std::cout << "resultado                : OK\n";
    } else {
        std::cout << "resultado                : FALHOU\n";
        return 1;
    }

    return 0;
}