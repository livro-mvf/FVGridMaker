// ============================================================================
// Arquivo: ex_Quality2D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Demonstra o uso de Quality2D com a API pública do FVGridMaker.
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
#include <FVGridMaker/TwoDimensional/Quality2D/Quality2D.h>
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

int main() {
    using namespace fvgrid;

    const StructuredGrid2D grid{
        Axis1D{std::vector<Real>{0.0, 1.0, 3.0}},
        Axis1D{std::vector<Real>{0.0, 1.0, 4.0}}
    };

    const QualityReport2D quality = Quality2D::evaluate(grid);

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Quality2D report\n";
    std::cout << "cell measure min/max/ratio: "
              << quality.min_cell_measure << " / "
              << quality.max_cell_measure << " / "
              << quality.cell_measure_ratio << '\n';
    std::cout << "computational area min/max/ratio: "
              << quality.min_computational_area << " / "
              << quality.max_computational_area << " / "
              << quality.computational_area_ratio << '\n';
    std::cout << "max adjacent ratio first direction: "
              << quality.max_adjacent_cell_measure_ratio_first_direction
              << '\n';
    std::cout << "max adjacent ratio second direction: "
              << quality.max_adjacent_cell_measure_ratio_second_direction
              << '\n';
}
