// ============================================================================
// Arquivo: ex_NonUniformStructuredGrid2D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Demonstra o uso de NonUniformStructuredGrid2D com a API pública do FVGridMaker.
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
#include <vector>

#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

int main() {
    using namespace fvgrid;

    const StructuredGrid2D grid{
        Axis1D{std::vector<Real>{0.0, 0.25, 1.0, 2.0}},
        Axis1D{std::vector<Real>{-1.0, 0.0, 0.5}}
    };

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Non-uniform Cartesian StructuredGrid2D\n";
    std::cout << "logical coordinates: " << grid.first_coordinate_name()
              << ", " << grid.second_coordinate_name() << "\n\n";

    for (Size j = 0; j < grid.num_cells_y(); ++j) {
        for (Size i = 0; i < grid.num_cells_x(); ++i) {
            std::cout << "cell(" << i << ", " << j << ") "
                      << "center=(" << grid.first_center(i) << ", "
                      << grid.second_center(j) << ") "
                      << "d_first=" << grid.first_cell_length(i) << ' '
                      << "d_second=" << grid.second_cell_length(j) << ' '
                      << "area=" << grid.cell_area(i, j) << '\n';
        }
    }
}
