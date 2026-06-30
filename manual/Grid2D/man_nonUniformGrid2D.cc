// ============================================================================
// Arquivo: man_nonUniformGrid2D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Programa de manual para gerar uma malha 2D não uniforme.
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

int main() {
    //
    // Neste programa, a malha 2D não é gerada por espaçamento uniforme.
    //
    // Em vez disso, fornecemos explicitamente as coordenadas das faces nas duas
    // direções. Cada eixo 1D é construído a partir de um vetor de faces.
    //
    // O número de volumes em cada direção é uma unidade menor que o número de
    // faces nessa direção.
    //

    using Scalar = fvgrid::Real;
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
    // Usamos formato fixo para manter a saída numérica estável no manual.
    //

    std::cout << std::fixed << std::setprecision(6);

    //
    // Primeiro, imprimimos um resumo compacto da malha.
    //

    std::cout << "\nMalha 2D não uniforme por faces explícitas\n";
    std::cout << "==========================================\n";
    std::cout << "O bloco abaixo apresenta uma malha cartesiana 2D construída\n";
    std::cout << "a partir de dois vetores de faces. Como os espaçamentos não\n";
    std::cout << "são constantes, os comprimentos das células variam com o índice.\n\n";

    std::cout << "sistema de coordenadas : "
              << grid.coordinate_system_name() << '\n';
    std::cout << "coordenadas lógicas    : "
              << grid.first_coordinate_name() << ", "
              << grid.second_coordinate_name() << '\n';
    std::cout << "padrão do eixo x       : "
              << grid.first_axis().pattern_name() << '\n';
    std::cout << "padrão do eixo y       : "
              << grid.second_axis().pattern_name() << '\n';
    std::cout << "número de faces em x   : " << grid.num_faces_x() << '\n';
    std::cout << "número de faces em y   : " << grid.num_faces_y() << '\n';
    std::cout << "número de células em x : " << grid.num_cells_x() << '\n';
    std::cout << "número de células em y : " << grid.num_cells_y() << '\n';
    std::cout << "número total de células: " << grid.num_cells() << '\n';
    std::cout << "xmin                   : " << grid.xmin() << '\n';
    std::cout << "xmax                   : " << grid.xmax() << '\n';
    std::cout << "ymin                   : " << grid.ymin() << '\n';
    std::cout << "ymax                   : " << grid.ymax() << '\n';

    //
    // Agora imprimimos a geometria de cada eixo separadamente.
    //
    // Esta etapa é importante porque a malha 2D é formada pela composição de
    // dois eixos 1D. A não uniformidade aparece primeiro nos comprimentos
    // locais de cada direção.
    //

    constexpr int id_width = 6;
    constexpr int value_width = 14;

    std::cout << "\nGeometria do eixo x\n";
    std::cout << "===================\n";
    std::cout << "Cada volume em x é delimitado por duas faces consecutivas.\n\n";

    std::cout << std::right
              << std::setw(id_width) << "i"
              << std::setw(value_width) << "face_oeste"
              << std::setw(value_width) << "centro"
              << std::setw(value_width) << "face_leste"
              << std::setw(value_width) << "dx"
              << '\n';

    std::cout << std::string(id_width + 4 * value_width, '-') << '\n';

    for (fvgrid::Size i = 0; i < grid.num_cells_x(); ++i) {
        std::cout << std::right
                  << std::setw(id_width) << i
                  << std::setw(value_width) << grid.x_face(i)
                  << std::setw(value_width) << grid.x_center(i)
                  << std::setw(value_width) << grid.x_face(i + 1)
                  << std::setw(value_width) << grid.x_cell_length(i)
                  << '\n';
    }

    std::cout << "\nGeometria do eixo y\n";
    std::cout << "===================\n";
    std::cout << "Cada volume em y é delimitado por duas faces consecutivas.\n\n";

    std::cout << std::right
              << std::setw(id_width) << "j"
              << std::setw(value_width) << "face_sul"
              << std::setw(value_width) << "centro"
              << std::setw(value_width) << "face_norte"
              << std::setw(value_width) << "dy"
              << '\n';

    std::cout << std::string(id_width + 4 * value_width, '-') << '\n';

    for (fvgrid::Size j = 0; j < grid.num_cells_y(); ++j) {
        std::cout << std::right
                  << std::setw(id_width) << j
                  << std::setw(value_width) << grid.y_face(j)
                  << std::setw(value_width) << grid.y_center(j)
                  << std::setw(value_width) << grid.y_face(j + 1)
                  << std::setw(value_width) << grid.y_cell_length(j)
                  << '\n';
    }

    //
    // Em seguida, imprimimos as células 2D.
    //
    // A medida física de cada célula cartesiana é dx * dy. Como dx e dy variam,
    // as áreas das células também variam.
    //

    Scalar sum_measure = Scalar{0.0};

    std::cout << "\nInformações geométricas dos volumes 2D\n";
    std::cout << "======================================\n";
    std::cout << "Cada linha mostra uma célula da malha não uniforme, seu índice\n";
    std::cout << "linear, seus comprimentos locais, seu centro e sua medida física.\n\n";

    std::cout << std::right
              << std::setw(id_width) << "i"
              << std::setw(id_width) << "j"
              << std::setw(id_width) << "k"
              << std::setw(value_width) << "dx"
              << std::setw(value_width) << "dy"
              << std::setw(value_width) << "x_centro"
              << std::setw(value_width) << "y_centro"
              << std::setw(value_width) << "medida"
              << '\n';

    std::cout << std::string(3 * id_width + 5 * value_width, '-') << '\n';

    for (fvgrid::Size j = 0; j < grid.num_cells_y(); ++j) {
        for (fvgrid::Size i = 0; i < grid.num_cells_x(); ++i) {
            const fvgrid::Size k = grid.linear_cell_index(i, j);

            const Scalar dx = grid.x_cell_length(i);
            const Scalar dy = grid.y_cell_length(j);
            const Scalar measure = grid.cell_measure(i, j);

            sum_measure += measure;

            std::cout << std::right
                      << std::setw(id_width) << i
                      << std::setw(id_width) << j
                      << std::setw(id_width) << k
                      << std::setw(value_width) << dx
                      << std::setw(value_width) << dy
                      << std::setw(value_width) << grid.x_center(i)
                      << std::setw(value_width) << grid.y_center(j)
                      << std::setw(value_width) << measure
                      << '\n';
        }
    }

    //
    // Teste simples de consistência geométrica.
    //
    // Mesmo em uma malha não uniforme, a soma das medidas físicas das células
    // deve reproduzir a área total do domínio retangular.
    //
    // Isto é,
    //
    //     soma(cell_measure(i, j)) = (xmax - xmin) * (ymax - ymin).
    //

    const Scalar expected_area = grid.length_x() * grid.length_y();
    const Scalar error = std::abs(sum_measure - expected_area);
    const Scalar tolerance = Scalar{1.0e-12};

    std::cout << "\nTeste de consistência geométrica\n";
    std::cout << "================================\n";
    std::cout << "O bloco abaixo verifica se a soma das medidas físicas das\n";
    std::cout << "células reproduz a área total do domínio, mesmo com espaçamentos\n";
    std::cout << "não uniformes nas duas direções.\n\n";

    std::cout << "soma das medidas : " << sum_measure << '\n';
    std::cout << "área esperada    : " << expected_area << '\n';

    std::cout << std::scientific;
    std::cout << "erro absoluto    : " << error << '\n';
    std::cout << "tolerância       : " << tolerance << '\n';

    if (error <= tolerance) {
        std::cout << "resultado        : OK\n";
    } else {
        std::cout << "resultado        : FALHOU\n";
        return 1;
    }

    return 0;
}