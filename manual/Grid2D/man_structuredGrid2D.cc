// ============================================================================
// Arquivo: man_structuredGrid2D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Programa de manual para gerar e imprimir uma malha 2D estruturada.
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
    // A malha é intencionalmente simples.
    //
    // Definimos o domínio cartesiano bidimensional [0, 2] x [-0.5, 1.0]
    // e o dividimos em 4 volumes na direção x e 3 volumes na direção y.
    //
    // A malha 2D é construída como produto tensorial de dois eixos 1D:
    // um eixo para x e outro eixo para y.
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

    //
    // uniform_axis_1d<Scalar> cria as coordenadas das faces e constrói o objeto
    // BasicAxis1D<Scalar> correspondente. Como usaremos dois eixos, a malha 2D
    // preserva a separação entre as direções x e y.
    //

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

    //
    // O construtor abaixo usa o sistema cartesiano padrão.
    //
    // Em coordenadas cartesianas, a área lógica da célula coincide com a medida
    // física da célula.
    //

    const Grid grid{x_axis, y_axis};

    //
    // Usamos um formato numérico fixo para tornar a saída estável e fácil de
    // comparar com o resultado impresso no manual.
    //

    std::cout << std::fixed << std::setprecision(6);

    //
    // Primeiro, imprimimos uma visão compacta da malha 2D.
    //

    std::cout << "\nResumo compacto da malha 2D\n";
    std::cout << "===========================\n";
    std::cout << "O bloco abaixo apresenta uma visão compacta da malha gerada:\n";
    std::cout << "sistema de coordenadas, nomes das coordenadas lógicas,\n";
    std::cout << "quantidade de células e faces, limites físicos e comprimentos.\n\n";

    std::cout << "sistema de coordenadas : "
              << grid.coordinate_system_name() << '\n';
    std::cout << "coordenadas lógicas    : "
              << grid.first_coordinate_name() << ", "
              << grid.second_coordinate_name() << '\n';
    std::cout << "padrão do eixo x       : "
              << grid.first_axis().pattern_name() << '\n';
    std::cout << "padrão do eixo y       : "
              << grid.second_axis().pattern_name() << '\n';
    std::cout << "vtk rectilinear        : "
              << (grid.vtk_rectilinear() ? "sim" : "nao") << '\n';
    std::cout << "número de células em x : " << grid.num_cells_x() << '\n';
    std::cout << "número de células em y : " << grid.num_cells_y() << '\n';
    std::cout << "número total de células: " << grid.num_cells() << '\n';
    std::cout << "número de faces em x   : " << grid.num_faces_x() << '\n';
    std::cout << "número de faces em y   : " << grid.num_faces_y() << '\n';
    std::cout << "xmin                   : " << grid.xmin() << '\n';
    std::cout << "xmax                   : " << grid.xmax() << '\n';
    std::cout << "ymin                   : " << grid.ymin() << '\n';
    std::cout << "ymax                   : " << grid.ymax() << '\n';
    std::cout << "comprimento em x       : " << grid.length_x() << '\n';
    std::cout << "comprimento em y       : " << grid.length_y() << '\n';

    //
    // Agora imprimimos as coordenadas das faces em cada direção.
    //
    // Como foram gerados 4 volumes em x, existem 5 faces em x.
    // Como foram gerados 3 volumes em y, existem 4 faces em y.
    //

    constexpr int id_width = 6;
    constexpr int value_width = 14;

    std::cout << "\nCoordenadas das faces em x\n";
    std::cout << "==========================\n";
    std::cout << "O bloco abaixo imprime a coordenada de cada face do eixo x.\n";
    std::cout << "Os índices das faces em x variam de 0 até 4.\n\n";

    std::cout << std::right
              << std::setw(id_width) << "i"
              << std::setw(value_width) << "x_face"
              << '\n';

    std::cout << std::string(id_width + value_width, '-') << '\n';

    for (fvgrid::Size i = 0; i < grid.num_faces_x(); ++i) {
        std::cout << std::right
                  << std::setw(id_width) << i
                  << std::setw(value_width) << grid.x_face(i)
                  << '\n';
    }

    std::cout << "\nCoordenadas das faces em y\n";
    std::cout << "==========================\n";
    std::cout << "O bloco abaixo imprime a coordenada de cada face do eixo y.\n";
    std::cout << "Os índices das faces em y variam de 0 até 3.\n\n";

    std::cout << std::right
              << std::setw(id_width) << "j"
              << std::setw(value_width) << "y_face"
              << '\n';

    std::cout << std::string(id_width + value_width, '-') << '\n';

    for (fvgrid::Size j = 0; j < grid.num_faces_y(); ++j) {
        std::cout << std::right
                  << std::setw(id_width) << j
                  << std::setw(value_width) << grid.y_face(j)
                  << '\n';
    }

    //
    // Em seguida, imprimimos as informações geométricas associadas aos volumes.
    //
    // Para cada volume finito, mostramos os índices (i, j), o índice linear k,
    // as faces oeste, leste, sul e norte, o centro da célula, a área lógica e
    // a medida física.
    //
    // Neste primeiro exemplo, como o sistema de coordenadas é cartesiano,
    // cell_logical_area(i, j) e cell_measure(i, j) devem coincidir.
    //

    std::cout << "\nInformações geométricas dos volumes 2D\n";
    std::cout << "======================================\n";
    std::cout << "O bloco abaixo imprime, para cada volume finito, os índices\n";
    std::cout << "lógicos, o índice linear row-major, as faces que delimitam a\n";
    std::cout << "célula, o centro, a área lógica e a medida física.\n\n";

    std::cout << std::right
              << std::setw(id_width) << "i"
              << std::setw(id_width) << "j"
              << std::setw(id_width) << "k"
              << std::setw(value_width) << "x_oeste"
              << std::setw(value_width) << "x_centro"
              << std::setw(value_width) << "x_leste"
              << std::setw(value_width) << "y_sul"
              << std::setw(value_width) << "y_centro"
              << std::setw(value_width) << "y_norte"
              << std::setw(value_width) << "area_logica"
              << std::setw(value_width) << "medida"
              << '\n';

    std::cout << std::string(3 * id_width + 8 * value_width, '-') << '\n';

    for (fvgrid::Size j = 0; j < grid.num_cells_y(); ++j) {
        for (fvgrid::Size i = 0; i < grid.num_cells_x(); ++i) {
            const fvgrid::Size k = grid.linear_cell_index(i, j);

            std::cout << std::right
                      << std::setw(id_width) << i
                      << std::setw(id_width) << j
                      << std::setw(id_width) << k
                      << std::setw(value_width) << grid.x_face(i)
                      << std::setw(value_width) << grid.x_center(i)
                      << std::setw(value_width) << grid.x_face(i + 1)
                      << std::setw(value_width) << grid.y_face(j)
                      << std::setw(value_width) << grid.y_center(j)
                      << std::setw(value_width) << grid.y_face(j + 1)
                      << std::setw(value_width)
                      << grid.cell_logical_area(i, j)
                      << std::setw(value_width)
                      << grid.cell_measure(i, j)
                      << '\n';
        }
    }

    //
    // Teste simples de consistência geométrica.
    //
    // Em uma malha 2D cartesiana, a soma das medidas físicas das células deve
    // reproduzir a área do retângulo.
    //
    // Isto é,
    //
    //     soma(cell_measure(i, j)) = (xmax - xmin) * (ymax - ymin).
    //
    // Como estamos usando números de ponto flutuante, a comparação é feita com
    // uma pequena tolerância.
    //

    Scalar sum_measure = Scalar{0.0};

    for (fvgrid::Size j = 0; j < grid.num_cells_y(); ++j) {
        for (fvgrid::Size i = 0; i < grid.num_cells_x(); ++i) {
            sum_measure += grid.cell_measure(i, j);
        }
    }

    const Scalar expected_area = grid.length_x() * grid.length_y();
    const Scalar error = std::abs(sum_measure - expected_area);
    const Scalar tolerance = Scalar{1.0e-12};

    std::cout << "\nTeste de consistência geométrica\n";
    std::cout << "================================\n";
    std::cout << "O bloco abaixo verifica se a soma das medidas físicas das\n";
    std::cout << "células reproduz a área total do domínio retangular.\n";
    std::cout << "No sistema cartesiano, essa medida é simplesmente a área.\n\n";

    std::cout << "soma das medidas : " << sum_measure << "\n";
    std::cout << "área esperada    : " << expected_area << "\n";
    std::cout << "erro absoluto    : " << error << "\n";
    std::cout << "tolerância       : " << tolerance << "\n";

    if (error <= tolerance) {
        std::cout << "resultado        : OK\n";
    } else {
        std::cout << "resultado        : FALHOU\n";
        return 1;
    }

    return 0;
}

