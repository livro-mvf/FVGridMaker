// ============================================================================
// Arquivo: man_axisymmetricGrid2D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Programa de manual para uma malha 2D axisimétrica cilíndrica.
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
    // Neste programa, a malha lógica é construída nas coordenadas (r, z).
    //
    // O domínio lógico é:
    //
    //     r em [0.5, 1.5],
    //     z em [-1, 2].
    //
    // Em coordenadas axisimétricas cilíndricas, cada célula retangular no plano
    // (r, z) representa um volume de revolução em torno do eixo z.
    //
    // Por isso, a área lógica dr * dz não coincide com a medida física da célula.
    // A medida física é um volume:
    //
    //     V = pi * (r_east^2 - r_west^2) * dz.
    //

    using Scalar = fvgrid::Real;
    using Axis = fvgrid::BasicAxis1D<Scalar>;
    using Grid = fvgrid::BasicStructuredGrid2D<Scalar>;

    const Scalar pi = std::numbers::pi_v<Scalar>;

    const fvgrid::Size nvol_r = 4;
    const fvgrid::Size nvol_z = 3;

    const Scalar r_inner = Scalar{0.5};
    const Scalar r_outer = Scalar{1.5};

    const Scalar z_begin = Scalar{-1.0};
    const Scalar z_end = Scalar{2.0};

    const Axis r_axis = fvgrid::uniform_axis_1d<Scalar>(
        nvol_r,
        r_inner,
        r_outer
    );

    const Axis z_axis = fvgrid::uniform_axis_1d<Scalar>(
        nvol_z,
        z_begin,
        z_end
    );

    //
    // O terceiro argumento define o sistema de coordenadas usado pela malha.
    // Aqui, as coordenadas lógicas são r e z. O ponto mapeado permanece no plano
    // meridional (r, z), mas a medida física é o volume obtido por revolução.
    //

    const Grid grid{
        r_axis,
        z_axis,
        fvgrid::BasicAxisymmetricCoordinates2D<Scalar>{}
    };

    //
    // Usamos formato fixo para manter a saída principal estável no manual.
    //

    std::cout << std::fixed << std::setprecision(6);

    //
    // Primeiro, imprimimos um resumo compacto da malha axisimétrica.
    //

    std::cout << "\nVolume axisimétrico cilíndrico\n";
    std::cout << "==============================\n";
    std::cout << "O bloco abaixo apresenta uma malha estruturada em coordenadas\n";
    std::cout << "axisimétricas cilíndricas. A malha lógica é retangular em\n";
    std::cout << "(r, z), mas a medida física representa volume de revolução.\n\n";

    std::cout << "sistema de coordenadas : "
              << grid.coordinate_system_name() << '\n';
    std::cout << "coordenadas lógicas    : "
              << grid.first_coordinate_name() << ", "
              << grid.second_coordinate_name() << '\n';
    std::cout << "vtk rectilinear        : "
              << (grid.vtk_rectilinear() ? "sim" : "nao") << '\n';
    std::cout << "número de células em r : " << grid.num_cells_x() << '\n';
    std::cout << "número de células em z : " << grid.num_cells_y() << '\n';
    std::cout << "número total de células: " << grid.num_cells() << '\n';
    std::cout << "r mínimo               : " << grid.xmin() << '\n';
    std::cout << "r máximo               : " << grid.xmax() << '\n';
    std::cout << "z mínimo               : " << grid.ymin() << '\n';
    std::cout << "z máximo               : " << grid.ymax() << '\n';

    //
    // Agora percorremos as células.
    //
    // Para cada célula, comparamos:
    //
    //     area_logica = dr * dz,
    //
    // com
    //
    //     medida = volume de revolução da célula.
    //
    // Em coordenadas axisimétricas, essas grandezas não coincidem.
    //

    constexpr int id_width = 6;
    constexpr int value_width = 14;

    Scalar sum_logical_area = Scalar{0.0};
    Scalar sum_measure = Scalar{0.0};

    std::cout << "\nInformações geométricas das células axisimétricas\n";
    std::cout << "=================================================\n";
    std::cout << "Cada linha mostra uma célula no plano lógico (r, z).\n";
    std::cout << "A área lógica é dr * dz. A medida física é o volume\n";
    std::cout << "gerado pela rotação da célula em torno do eixo z.\n\n";

    std::cout << std::right
              << std::setw(id_width) << "i"
              << std::setw(id_width) << "j"
              << std::setw(id_width) << "k"
              << std::setw(value_width) << "r_centro"
              << std::setw(value_width) << "z_centro"
              << std::setw(value_width) << "r_mapeado"
              << std::setw(value_width) << "z_mapeado"
              << std::setw(value_width) << "area_logica"
              << std::setw(value_width) << "medida"
              << '\n';

    std::cout << std::string(3 * id_width + 6 * value_width, '-') << '\n';

    for (fvgrid::Size j = 0; j < grid.num_cells_y(); ++j) {
        for (fvgrid::Size i = 0; i < grid.num_cells_x(); ++i) {
            const fvgrid::Size k = grid.linear_cell_index(i, j);

            const Scalar r = grid.first_center(i);
            const Scalar z = grid.second_center(j);

            const auto mapped_point = grid.physical_cell_center(i, j);

            const Scalar logical_area = grid.cell_logical_area(i, j);
            const Scalar measure = grid.cell_measure(i, j);

            sum_logical_area += logical_area;
            sum_measure += measure;

            std::cout << std::right
                      << std::setw(id_width) << i
                      << std::setw(id_width) << j
                      << std::setw(id_width) << k
                      << std::setw(value_width) << r
                      << std::setw(value_width) << z
                      << std::setw(value_width) << mapped_point.x
                      << std::setw(value_width) << mapped_point.y
                      << std::setw(value_width) << logical_area
                      << std::setw(value_width) << measure
                      << '\n';
        }
    }

    //
    // Teste simples de consistência geométrica.
    //
    // O volume analítico da casca cilíndrica é:
    //
    //     V = pi * (r_outer^2 - r_inner^2) * (z_end - z_begin).
    //
    // A soma das medidas físicas das células deve recuperar esse volume.
    // A soma das áreas lógicas, por outro lado, mede apenas a área do retângulo
    // lógico em (r, z).
    //

    const Scalar expected_measure =
        pi
        * (r_outer * r_outer - r_inner * r_inner)
        * (z_end - z_begin);

    const Scalar expected_logical_area =
        (r_outer - r_inner) * (z_end - z_begin);

    const Scalar measure_error = std::abs(sum_measure - expected_measure);
    const Scalar logical_error =
        std::abs(sum_logical_area - expected_logical_area);

    const Scalar difference_between_sums =
        std::abs(sum_measure - sum_logical_area);

    const Scalar tolerance = Scalar{1.0e-12};

    std::cout << "\nTeste de consistência geométrica\n";
    std::cout << "================================\n";
    std::cout << "O bloco abaixo compara a soma das áreas lógicas com a área\n";
    std::cout << "do retângulo lógico e a soma das medidas físicas com o volume\n";
    std::cout << "analítico da casca cilíndrica.\n\n";

    std::cout << "soma das áreas lógicas       : " << sum_logical_area << '\n';
    std::cout << "área lógica esperada         : " << expected_logical_area << '\n';
    std::cout << "soma das medidas físicas     : " << sum_measure << '\n';
    std::cout << "volume físico esperado       : " << expected_measure << '\n';
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