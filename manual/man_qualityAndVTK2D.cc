// ============================================================================
// Arquivo: man_qualityAndVTK2D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Programa de manual para qualidade de malha 2D e exportação VTK.
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
#include <iomanip>
#include <iostream>
#include <numbers>
#include <string>

#include <FVGridMaker/FVGridMaker.h>

int main() {
    //
    // Este programa fecha a sequência 2D com duas tarefas práticas:
    //
    //     1. calcular um relatório de qualidade da malha;
    //     2. exportar a geometria para VTK legacy.
    //
    // Usamos novamente uma arruela com setor angular removido, pois ela produz
    // uma malha não retangular no plano físico e é adequada para inspeção visual
    // no ParaView.
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

    const auto report = fvgrid::Quality2D::evaluate(grid);

    const std::filesystem::path vtk_file{"malha_qualidade_2d.vtk"};
    fvgrid::write_vtk(grid, vtk_file);

    std::cout << std::fixed << std::setprecision(6);

    std::cout << "\nRelatório de qualidade e saída VTK\n";
    std::cout << "==================================\n";
    std::cout << "O bloco abaixo avalia a qualidade de uma malha polar parcial\n";
    std::cout << "e grava um arquivo VTK legacy para inspeção visual no ParaView.\n\n";

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

    std::cout << "\nMedidas das células\n";
    std::cout << "===================\n";
    std::cout << "menor medida física : "
              << report.min_cell_measure << '\n';
    std::cout << "maior medida física : "
              << report.max_cell_measure << '\n';
    std::cout << "razão max/min       : "
              << report.cell_measure_ratio << '\n';

    std::cout << "\nÁreas computacionais\n";
    std::cout << "====================\n";
    std::cout << "menor área lógica   : "
              << report.min_computational_area << '\n';
    std::cout << "maior área lógica   : "
              << report.max_computational_area << '\n';
    std::cout << "razão max/min       : "
              << report.computational_area_ratio << '\n';

    std::cout << "\nRazões entre células adjacentes\n";
    std::cout << "===============================\n";
    std::cout << "direção radial       : "
              << report.max_adjacent_cell_measure_ratio_first_direction
              << '\n';
    std::cout << "direção angular      : "
              << report.max_adjacent_cell_measure_ratio_second_direction
              << '\n';

    std::cout << "\nMedidas das faces\n";
    std::cout << "=================\n";
    std::cout << "menor face da primeira família : "
              << report.min_first_face_measure << '\n';
    std::cout << "maior face da primeira família : "
              << report.max_first_face_measure << '\n';
    std::cout << "menor face da segunda família  : "
              << report.min_second_face_measure << '\n';
    std::cout << "maior face da segunda família  : "
              << report.max_second_face_measure << '\n';

    const bool vtk_exists = std::filesystem::exists(vtk_file);
    const bool vtk_non_empty = vtk_exists &&
        std::filesystem::file_size(vtk_file) > static_cast<std::uintmax_t>(0);

    const bool positive_measures =
        report.min_cell_measure > Scalar{0.0} &&
        report.max_cell_measure >= report.min_cell_measure &&
        report.min_computational_area > Scalar{0.0} &&
        report.max_computational_area >= report.min_computational_area;

    const bool meaningful_ratios =
        report.cell_measure_ratio >= Scalar{1.0} &&
        report.computational_area_ratio >= Scalar{1.0} &&
        report.max_adjacent_cell_measure_ratio_first_direction >= Scalar{1.0} &&
        report.max_adjacent_cell_measure_ratio_second_direction >= Scalar{1.0};

    std::cout << "\nTeste de consistência do relatório\n";
    std::cout << "==================================\n";
    std::cout << "O bloco abaixo verifica se o relatório possui medidas positivas,\n";
    std::cout << "razões maiores ou iguais a 1 e se o arquivo VTK foi criado.\n\n";

    std::cout << "arquivo VTK      : " << vtk_file.string() << '\n';
    std::cout << "arquivo existe   : " << (vtk_exists ? "sim" : "nao") << '\n';
    std::cout << "arquivo não vazio: " << (vtk_non_empty ? "sim" : "nao") << '\n';

    const bool passed =
        positive_measures &&
        meaningful_ratios &&
        vtk_non_empty;

    if (passed) {
        std::cout << "resultado        : OK\n";
    } else {
        std::cout << "resultado        : FALHOU\n";
        return 1;
    }

    return 0;
}