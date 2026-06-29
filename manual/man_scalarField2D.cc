// ============================================================================
// Arquivo: man_scalarField2D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Programa de manual para armazenar um campo escalar em malha 2D.
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
#include <vector>

#include <FVGridMaker/FVGridMaker.h>

namespace {

using Scalar = fvgrid::Real;

Scalar field_value(Scalar x, Scalar y) {
    return Scalar{1.0} + x + Scalar{2.0} * y;
}

}  // namespace

int main() {
    //
    // Neste programa, usamos novamente uma malha 2D cartesiana não uniforme.
    //
    // O objetivo agora é armazenar um campo escalar definido por célula em um
    // vetor linear. A ligação entre o par de índices (i, j) e a posição k do
    // vetor é feita por:
    //
    //     k = grid.linear_cell_index(i, j).
    //
    // Esse padrão será usado sempre que um campo por célula for armazenado em
    // uma estrutura linear.
    //

    using Axis = fvgrid::BasicAxis1D<Scalar>;
    using Grid = fvgrid::BasicStructuredGrid2D<Scalar>;

    const std::vector<Scalar> x_faces{
        Scalar{0.00},
        Scalar{0.15},
        Scalar{0.50},
        Scalar{1.10},
        Scalar{2.00}
    };

    const std::vector<Scalar> y_faces{
        Scalar{-0.50},
        Scalar{-0.10},
        Scalar{0.35},
        Scalar{1.00}
    };

    const Axis x_axis{x_faces};
    const Axis y_axis{y_faces};

    const Grid grid{x_axis, y_axis};

    //
    // Criamos um vetor com uma entrada por célula. A posição k do vetor
    // corresponde ao índice linear da célula.
    //

    std::vector<Scalar> phi(grid.num_cells(), Scalar{0.0});

    for (fvgrid::Size j = 0; j < grid.num_cells_y(); ++j) {
        for (fvgrid::Size i = 0; i < grid.num_cells_x(); ++i) {
            const fvgrid::Size k = grid.linear_cell_index(i, j);

            const Scalar x = grid.x_center(i);
            const Scalar y = grid.y_center(j);

            phi[k] = field_value(x, y);
        }
    }

    //
    // Usamos formato fixo para manter a saída principal estável no manual.
    //

    std::cout << std::fixed << std::setprecision(6);

    //
    // Primeiro, imprimimos o resumo da malha e do campo.
    //

    std::cout << "\nCampo escalar em armazenamento row-major\n";
    std::cout << "========================================\n";
    std::cout << "O bloco abaixo usa uma malha cartesiana 2D não uniforme e\n";
    std::cout << "armazena um campo escalar por célula em um vetor linear.\n";
    std::cout << "A posição do vetor é obtida por k = grid.linear_cell_index(i, j).\n\n";

    std::cout << "sistema de coordenadas : "
              << grid.coordinate_system_name() << '\n';
    std::cout << "número de células em x : " << grid.num_cells_x() << '\n';
    std::cout << "número de células em y : " << grid.num_cells_y() << '\n';
    std::cout << "número total de células: " << grid.num_cells() << '\n';
    std::cout << "campo escalar          : phi(x, y) = 1 + x + 2y\n";

    //
    // Agora imprimimos o conteúdo do vetor phi associado à geometria da célula.
    //
    // A contribuição phi_P * A_P é a parcela da célula na integral discreta do
    // campo sobre o domínio.
    //

    constexpr int id_width = 6;
    constexpr int value_width = 14;

    Scalar sum_measure = Scalar{0.0};
    Scalar discrete_integral = Scalar{0.0};

    std::cout << "\nValores por célula\n";
    std::cout << "==================\n";
    std::cout << "Cada linha mostra uma célula, sua posição no vetor linear,\n";
    std::cout << "o centro geométrico, a medida física, o valor armazenado de\n";
    std::cout << "phi e a contribuição phi * medida.\n\n";

    std::cout << std::right
              << std::setw(id_width) << "i"
              << std::setw(id_width) << "j"
              << std::setw(id_width) << "k"
              << std::setw(value_width) << "x_centro"
              << std::setw(value_width) << "y_centro"
              << std::setw(value_width) << "medida"
              << std::setw(value_width) << "phi[k]"
              << std::setw(value_width) << "phi*medida"
              << '\n';

    std::cout << std::string(3 * id_width + 5 * value_width, '-') << '\n';

    for (fvgrid::Size j = 0; j < grid.num_cells_y(); ++j) {
        for (fvgrid::Size i = 0; i < grid.num_cells_x(); ++i) {
            const fvgrid::Size k = grid.linear_cell_index(i, j);

            const Scalar measure = grid.cell_measure(i, j);
            const Scalar contribution = phi[k] * measure;

            sum_measure += measure;
            discrete_integral += contribution;

            std::cout << std::right
                      << std::setw(id_width) << i
                      << std::setw(id_width) << j
                      << std::setw(id_width) << k
                      << std::setw(value_width) << grid.x_center(i)
                      << std::setw(value_width) << grid.y_center(j)
                      << std::setw(value_width) << measure
                      << std::setw(value_width) << phi[k]
                      << std::setw(value_width) << contribution
                      << '\n';
        }
    }

    //
    // Teste simples de consistência da integração discreta.
    //
    // Para o campo phi(x, y) = 1 + x + 2y, a média analítica sobre o retângulo é
    //
    //     phi_bar = 1 + x_bar + 2 * y_bar,
    //
    // em que x_bar e y_bar são os pontos médios do domínio. Como o campo é
    // linear, a regra do ponto médio por célula integra exatamente em cada
    // retângulo da malha.
    //

    const Scalar xmin = grid.xmin();
    const Scalar xmax = grid.xmax();
    const Scalar ymin = grid.ymin();
    const Scalar ymax = grid.ymax();

    const Scalar expected_area = grid.length_x() * grid.length_y();

    const Scalar x_average = Scalar{0.5} * (xmin + xmax);
    const Scalar y_average = Scalar{0.5} * (ymin + ymax);

    const Scalar expected_average = field_value(x_average, y_average);
    const Scalar expected_integral = expected_average * expected_area;

    const Scalar discrete_average = discrete_integral / sum_measure;

    const Scalar integral_error =
        std::abs(discrete_integral - expected_integral);
    const Scalar average_error =
        std::abs(discrete_average - expected_average);
    const Scalar area_error = std::abs(sum_measure - expected_area);

    const Scalar tolerance = Scalar{1.0e-12};

    std::cout << "\nTeste de consistência da integral discreta\n";
    std::cout << "==========================================\n";
    std::cout << "O bloco abaixo compara a integral discreta e a média ponderada\n";
    std::cout << "por célula com os valores analíticos no domínio retangular.\n";
    std::cout << "Para o campo linear escolhido, a regra do ponto médio por célula\n";
    std::cout << "é exata, salvo erro de arredondamento.\n\n";

    std::cout << "área discreta         : " << sum_measure << '\n';
    std::cout << "área esperada         : " << expected_area << '\n';
    std::cout << "integral discreta     : " << discrete_integral << '\n';
    std::cout << "integral esperada     : " << expected_integral << '\n';
    std::cout << "média discreta        : " << discrete_average << '\n';
    std::cout << "média esperada        : " << expected_average << '\n';

    std::cout << std::scientific;
    std::cout << "erro na área          : " << area_error << '\n';
    std::cout << "erro na integral      : " << integral_error << '\n';
    std::cout << "erro na média         : " << average_error << '\n';
    std::cout << "tolerância            : " << tolerance << '\n';

    const bool passed =
        area_error <= tolerance &&
        integral_error <= tolerance &&
        average_error <= tolerance;

    if (passed) {
        std::cout << "resultado             : OK\n";
    } else {
        std::cout << "resultado             : FALHOU\n";
        return 1;
    }

    return 0;
}

