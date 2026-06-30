// ============================================================================
// Arquivo: man_clipGrid2D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Programa de manual para interseção e recorte de malhas 2D.
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

#include <FVGridMaker/FVGridMaker.h>

int main() {
    //
    // Este programa cria duas malhas cartesianas estruturadas parcialmente
    // sobrepostas. Em seguida, calcula a interseção lógica dos domínios e
    // recorta a primeira malha para essa caixa comum.
    //
    // Os limites foram escolhidos para coincidir com faces da primeira malha.
    // Isso torna o recorte exato e simples de verificar.
    //

    using Scalar = fvgrid::Real;
    using Axis = fvgrid::BasicAxis1D<Scalar>;
    using Grid = fvgrid::BasicStructuredGrid2D<Scalar>;

    const Grid grid_a{
        fvgrid::uniform_axis_1d<Scalar>(6, Scalar{0.0}, Scalar{3.0}),
        fvgrid::uniform_axis_1d<Scalar>(4, Scalar{0.0}, Scalar{2.0})
    };

    const Grid grid_b{
        fvgrid::uniform_axis_1d<Scalar>(3, Scalar{1.0}, Scalar{2.5}),
        fvgrid::uniform_axis_1d<Scalar>(2, Scalar{0.5}, Scalar{1.5})
    };

    const Scalar tolerance = Scalar{1.0e-12};

    const auto box_a = fvgrid::Operations2D::domain_box(grid_a);
    const auto box_b = fvgrid::Operations2D::domain_box(grid_b);

    const auto intersection_box = fvgrid::Operations2D::require_box_intersection(
        grid_a,
        grid_b,
        tolerance
    );

    const Grid clipped_grid = fvgrid::Operations2D::clip_to_logical_box(
        grid_a,
        intersection_box,
        tolerance
    );

    std::cout << std::fixed << std::setprecision(6);

    std::cout << "\nInterseção e recorte de malhas 2D\n";
    std::cout << "=================================\n";
    std::cout << "O bloco abaixo cria duas malhas cartesianas parcialmente\n";
    std::cout << "sobrepostas, calcula a caixa lógica comum e recorta a primeira\n";
    std::cout << "malha para essa interseção.\n\n";

    std::cout << "malha A, xmin : " << grid_a.xmin() << '\n';
    std::cout << "malha A, xmax : " << grid_a.xmax() << '\n';
    std::cout << "malha A, ymin : " << grid_a.ymin() << '\n';
    std::cout << "malha A, ymax : " << grid_a.ymax() << '\n';
    std::cout << "malha B, xmin : " << grid_b.xmin() << '\n';
    std::cout << "malha B, xmax : " << grid_b.xmax() << '\n';
    std::cout << "malha B, ymin : " << grid_b.ymin() << '\n';
    std::cout << "malha B, ymax : " << grid_b.ymax() << '\n';

    std::cout << "\nCaixas lógicas\n";
    std::cout << "==============\n";

    std::cout << "domínio A, primeira direção : ["
              << box_a.first_interval().lower() << ", "
              << box_a.first_interval().upper() << "]\n";
    std::cout << "domínio A, segunda direção  : ["
              << box_a.second_interval().lower() << ", "
              << box_a.second_interval().upper() << "]\n";

    std::cout << "domínio B, primeira direção : ["
              << box_b.first_interval().lower() << ", "
              << box_b.first_interval().upper() << "]\n";
    std::cout << "domínio B, segunda direção  : ["
              << box_b.second_interval().lower() << ", "
              << box_b.second_interval().upper() << "]\n";

    std::cout << "interseção, primeira direção: ["
              << intersection_box.first_interval().lower() << ", "
              << intersection_box.first_interval().upper() << "]\n";
    std::cout << "interseção, segunda direção : ["
              << intersection_box.second_interval().lower() << ", "
              << intersection_box.second_interval().upper() << "]\n";
    std::cout << "área computacional comum    : "
              << intersection_box.computational_area() << '\n';

    std::cout << "\nMalha recortada\n";
    std::cout << "===============\n";
    std::cout << "número de células em x : "
              << clipped_grid.num_cells_x() << '\n';
    std::cout << "número de células em y : "
              << clipped_grid.num_cells_y() << '\n';
    std::cout << "número total de células: "
              << clipped_grid.num_cells() << '\n';
    std::cout << "xmin                   : "
              << clipped_grid.xmin() << '\n';
    std::cout << "xmax                   : "
              << clipped_grid.xmax() << '\n';
    std::cout << "ymin                   : "
              << clipped_grid.ymin() << '\n';
    std::cout << "ymax                   : "
              << clipped_grid.ymax() << '\n';

    constexpr int id_width = 6;
    constexpr int value_width = 14;

    Scalar sum_measure = Scalar{0.0};

    std::cout << "\nCélulas da malha recortada\n";
    std::cout << "==========================\n";

    std::cout << std::right
              << std::setw(id_width) << "i"
              << std::setw(id_width) << "j"
              << std::setw(id_width) << "k"
              << std::setw(value_width) << "x_centro"
              << std::setw(value_width) << "y_centro"
              << std::setw(value_width) << "medida"
              << '\n';

    std::cout << std::string(3 * id_width + 3 * value_width, '-') << '\n';

    for (fvgrid::Size j = 0; j < clipped_grid.num_cells_y(); ++j) {
        for (fvgrid::Size i = 0; i < clipped_grid.num_cells_x(); ++i) {
            const fvgrid::Size k = clipped_grid.linear_cell_index(i, j);
            const Scalar measure = clipped_grid.cell_measure(i, j);

            sum_measure += measure;

            std::cout << std::right
                      << std::setw(id_width) << i
                      << std::setw(id_width) << j
                      << std::setw(id_width) << k
                      << std::setw(value_width) << clipped_grid.x_center(i)
                      << std::setw(value_width) << clipped_grid.y_center(j)
                      << std::setw(value_width) << measure
                      << '\n';
        }
    }

    const Scalar expected_area = intersection_box.computational_area();
    const Scalar error = std::abs(sum_measure - expected_area);

    std::cout << "\nTeste de consistência geométrica\n";
    std::cout << "================================\n";
    std::cout << "O bloco abaixo verifica se a soma das medidas da malha\n";
    std::cout << "recortada coincide com a área da caixa lógica comum.\n\n";

    std::cout << "soma das medidas : " << sum_measure << '\n';
    std::cout << "área esperada    : " << expected_area << '\n';

    std::cout << std::scientific;
    std::cout << "erro absoluto    : " << error << '\n';
    std::cout << "tolerância       : " << tolerance << '\n';

    fvgrid::write_vtk(clipped_grid, "malha_recortada.vtk");

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "arquivo VTK      : malha_recortada.vtk\n";

    if (error <= tolerance) {
        std::cout << "resultado        : OK\n";
    } else {
        std::cout << "resultado        : FALHOU\n";
        return 1;
    }

    return 0;
}