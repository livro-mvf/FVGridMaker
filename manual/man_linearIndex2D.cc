
// ============================================================================
// Arquivo: man_linearIndex2D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Programa de manual para demonstrar a indexação linear em malhas 2D.
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

#include <iomanip>
#include <iostream>
#include <string>

#include <FVGridMaker/FVGridMaker.h>

int main() {
    //
    // Este programa usa a mesma malha cartesiana estruturada do primeiro exemplo
    // 2D do manual.
    //
    // O objetivo agora não é discutir faces, áreas ou medidas físicas, mas apenas
    // mostrar como uma célula identificada por (i, j) é convertida em um índice
    // linear k.
    //
    // A ordem adotada pela biblioteca é row-major:
    //
    //     k = j * Nx + i.
    //
    // Assim, o índice i varia mais rapidamente, enquanto j identifica a linha da
    // malha.
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
    // Primeiro, imprimimos o resumo mínimo necessário para interpretar a tabela.
    //

    std::cout << "\nIndexação linear das células 2D\n";
    std::cout << "================================\n";
    std::cout << "O bloco abaixo mostra como a malha estruturada 2D converte\n";
    std::cout << "o par de índices lógicos (i, j) no índice linear k.\n";
    std::cout << "A ordem usada é row-major, isto é, k = j * Nx + i.\n\n";

    std::cout << "número de células em x : " << grid.num_cells_x() << '\n';
    std::cout << "número de células em y : " << grid.num_cells_y() << '\n';
    std::cout << "número total de células: " << grid.num_cells() << '\n';

    //
    // Agora percorremos as células e comparamos o índice retornado pela
    // biblioteca com o índice calculado diretamente pela fórmula row-major.
    //

    constexpr int id_width = 8;
    constexpr int value_width = 14;

    bool all_indices_match = true;

    std::cout << "\nTabela de índices lineares\n";
    std::cout << "==========================\n";
    std::cout << "Cada linha da tabela corresponde a uma célula. A coluna\n";
    std::cout << "k_biblioteca vem de grid.linear_cell_index(i, j), enquanto\n";
    std::cout << "k_formula é calculada manualmente por j * Nx + i.\n\n";

    std::cout << std::right
              << std::setw(id_width) << "i"
              << std::setw(id_width) << "j"
              << std::setw(id_width) << "linha"
              << std::setw(id_width) << "coluna"
              << std::setw(value_width) << "k_biblioteca"
              << std::setw(value_width) << "k_formula"
              << std::setw(value_width) << "x_centro"
              << std::setw(value_width) << "y_centro"
              << std::setw(value_width) << "confere"
              << '\n';

    std::cout << std::string(4 * id_width + 5 * value_width, '-') << '\n';

    for (fvgrid::Size j = 0; j < grid.num_cells_y(); ++j) {
        for (fvgrid::Size i = 0; i < grid.num_cells_x(); ++i) {
            const fvgrid::Size k_library = grid.linear_cell_index(i, j);
            const fvgrid::Size k_formula = j * grid.num_cells_x() + i;
            const bool matches = (k_library == k_formula);

            all_indices_match = all_indices_match && matches;

            std::cout << std::right
                      << std::setw(id_width) << i
                      << std::setw(id_width) << j
                      << std::setw(id_width) << j
                      << std::setw(id_width) << i
                      << std::setw(value_width) << k_library
                      << std::setw(value_width) << k_formula
                      << std::setw(value_width) << grid.x_center(i)
                      << std::setw(value_width) << grid.y_center(j)
                      << std::setw(value_width) << (matches ? "sim" : "nao")
                      << '\n';
        }
    }

    //
    // Teste simples de consistência da indexação.
    //
    // O teste falha se qualquer célula tiver índice linear diferente de
    // j * Nx + i.
    //

    std::cout << "\nTeste de consistência da indexação\n";
    std::cout << "==================================\n";
    std::cout << "O bloco abaixo verifica se todos os índices lineares retornados\n";
    std::cout << "pela biblioteca coincidem com a fórmula row-major.\n\n";

    std::cout << "fórmula esperada : k = j * Nx + i\n";
    std::cout << "Nx               : " << grid.num_cells_x() << '\n';
    std::cout << "células testadas : " << grid.num_cells() << '\n';

    if (all_indices_match) {
        std::cout << "resultado        : OK\n";
    } else {
        std::cout << "resultado        : FALHOU\n";
        return 1;
    }

    return 0;
}

