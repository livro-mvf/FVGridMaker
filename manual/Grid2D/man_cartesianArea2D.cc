// ============================================================================
// Arquivo: man_cartesianArea2D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Programa de manual para comparar áreas e medidas em malha 2D.
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
    // Este programa usa uma malha cartesiana estruturada simples.
    //
    // O objetivo é comparar três grandezas:
    //
    //     cell_logical_area(i, j)
    //     cell_area(i, j)
    //     cell_measure(i, j)
    //
    // Em coordenadas cartesianas, as três representam a mesma área física da
    // célula. Em outros sistemas de coordenadas, essa igualdade não precisa
    // permanecer válida.
    //

    using Scalar = fvgrid::Real;
    using Axis = fvgrid::BasicAxis1D<Scalar>;
    using Grid = fvgrid::BasicStructuredGrid2D<Scalar>;

    const fvgrid::Size nvol_x = 4;
    const fvgrid::Size nvol_y = 3;

    const Scalar xinit = Scalar{0.0};
    const Scalar xfinal = Scalar{2.0};

    const Scalar yinit = Scalar{-0.5};
    const Scalar yfinal = Scalar{1.0};

    const Axis x_axis = fvgrid::uniform_axis_1d<Scalar>(
        nvol_x,
        xinit,
        xfinal
    );

    const Axis y_axis = fvgrid::uniform_axis_1d<Scalar>(
        nvol_y,
        yinit,
        yfinal
    );

    const Grid grid{x_axis, y_axis};

    //
    // Usamos formato fixo para manter a saída numérica estável no manual.
    //

    std::cout << std::fixed << std::setprecision(6);

    //
    // Primeiro, imprimimos o resumo mínimo da malha e do sistema de coordenadas.
    //

    std::cout << "\nÁrea lógica e medida física em coordenadas cartesianas\n";
    std::cout << "======================================================\n";
    std::cout << "O bloco abaixo compara área lógica, área cartesiana e\n";
    std::cout << "medida física em uma malha 2D cartesiana estruturada.\n";
    std::cout << "Neste sistema, as três grandezas devem coincidir.\n\n";

    std::cout << "sistema de coordenadas : "
              << grid.coordinate_system_name() << '\n';
    std::cout << "coordenadas lógicas    : "
              << grid.first_coordinate_name() << ", "
              << grid.second_coordinate_name() << '\n';
    std::cout << "número de células em x : " << grid.num_cells_x() << '\n';
    std::cout << "número de células em y : " << grid.num_cells_y() << '\n';
    std::cout << "número total de células: " << grid.num_cells() << '\n';
    std::cout << "xmin                   : " << grid.xmin() << '\n';
    std::cout << "xmax                   : " << grid.xmax() << '\n';
    std::cout << "ymin                   : " << grid.ymin() << '\n';
    std::cout << "ymax                   : " << grid.ymax() << '\n';

    //
    // Agora percorremos todas as células.
    //
    // Para cada célula, imprimimos os comprimentos locais em x e y, a área
    // lógica, a área cartesiana e a medida física.
    //

    constexpr int id_width = 6;
    constexpr int value_width = 15;

    Scalar sum_logical_area = Scalar{0.0};
    Scalar sum_cartesian_area = Scalar{0.0};
    Scalar sum_measure = Scalar{0.0};

    Scalar max_difference = Scalar{0.0};

    std::cout << "\nÁreas por célula\n";
    std::cout << "================\n";
    std::cout << "Cada linha mostra uma célula da malha. No sistema cartesiano,\n";
    std::cout << "area_logica, area_cartesiana e medida devem ter o mesmo valor.\n\n";

    std::cout << std::right
              << std::setw(id_width) << "i"
              << std::setw(id_width) << "j"
              << std::setw(value_width) << "dx"
              << std::setw(value_width) << "dy"
              << std::setw(value_width) << "area_logica"
              << std::setw(value_width) << "area_cart"
              << std::setw(value_width) << "medida"
              << '\n';

    std::cout << std::string(2 * id_width + 5 * value_width, '-') << '\n';

    for (fvgrid::Size j = 0; j < grid.num_cells_y(); ++j) {
        for (fvgrid::Size i = 0; i < grid.num_cells_x(); ++i) {
            const Scalar dx = grid.x_cell_length(i);
            const Scalar dy = grid.y_cell_length(j);

            const Scalar logical_area = grid.cell_logical_area(i, j);
            const Scalar cartesian_area = grid.cell_area(i, j);
            const Scalar measure = grid.cell_measure(i, j);

            sum_logical_area += logical_area;
            sum_cartesian_area += cartesian_area;
            sum_measure += measure;

            const Scalar difference_1 = std::abs(logical_area - cartesian_area);
            const Scalar difference_2 = std::abs(logical_area - measure);
            const Scalar difference_3 = std::abs(cartesian_area - measure);

            max_difference = std::max(max_difference, difference_1);
            max_difference = std::max(max_difference, difference_2);
            max_difference = std::max(max_difference, difference_3);

            std::cout << std::right
                      << std::setw(id_width) << i
                      << std::setw(id_width) << j
                      << std::setw(value_width) << dx
                      << std::setw(value_width) << dy
                      << std::setw(value_width) << logical_area
                      << std::setw(value_width) << cartesian_area
                      << std::setw(value_width) << measure
                      << '\n';
        }
    }

    //
    // Teste simples de consistência geométrica.
    //
    // Em coordenadas cartesianas, a soma das áreas lógicas, das áreas
    // cartesianas e das medidas físicas deve reproduzir a área do retângulo:
    //
    //     A = (xmax - xmin) * (ymax - ymin).
    //
    // Além disso, para cada célula, as três medidas devem coincidir entre si.
    //

    const Scalar expected_area = grid.length_x() * grid.length_y();

    const Scalar error_logical_area = std::abs(sum_logical_area - expected_area);
    const Scalar error_cartesian_area =
        std::abs(sum_cartesian_area - expected_area);
    const Scalar error_measure = std::abs(sum_measure - expected_area);

    const Scalar tolerance = Scalar{1.0e-12};

    std::cout << "\nTeste de consistência geométrica\n";
    std::cout << "================================\n";
    std::cout << "O bloco abaixo verifica se as três somas recuperam a área\n";
    std::cout << "analítica do domínio retangular e se as três medidas coincidem\n";
    std::cout << "célula a célula.\n\n";

    std::cout << "área esperada             : " << expected_area << '\n';
    std::cout << "soma das áreas lógicas    : " << sum_logical_area << '\n';
    std::cout << "soma das áreas cartesianas: " << sum_cartesian_area << '\n';
    std::cout << "soma das medidas físicas  : " << sum_measure << '\n';

    std::cout << std::scientific;
    std::cout << "erro áreas lógicas        : " << error_logical_area << '\n';
    std::cout << "erro áreas cartesianas    : " << error_cartesian_area << '\n';
    std::cout << "erro medidas físicas      : " << error_measure << '\n';
    std::cout << "maior diferença local     : " << max_difference << '\n';
    std::cout << "tolerância                : " << tolerance << '\n';

    const bool passed =
        error_logical_area <= tolerance &&
        error_cartesian_area <= tolerance &&
        error_measure <= tolerance &&
        max_difference <= tolerance;

    if (passed) {
        std::cout << "resultado                 : OK\n";
    } else {
        std::cout << "resultado                 : FALHOU\n";
        return 1;
    }

    return 0;
}

