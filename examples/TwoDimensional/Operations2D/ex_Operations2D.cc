// ============================================================================
// Arquivo: ex_Operations2D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Demonstra o uso de Operations2D com a API pública do FVGridMaker.
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
#include <iostream>
#include <vector>

#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/TwoDimensional/Operations2D/Operations2D.h>
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

int main() {
    using namespace fvgrid;

    const StructuredGrid2D left{
        Axis1D{std::vector<Real>{0.0, 1.0, 2.0, 3.0}},
        Axis1D{std::vector<Real>{0.0, 1.0, 2.0}}
    };
    const StructuredGrid2D right{
        Axis1D{std::vector<Real>{0.5, 1.5, 2.5}},
        Axis1D{std::vector<Real>{0.5, 1.5}}
    };

    const LogicalBox2D overlap = Operations2D::require_box_intersection(
        left,
        right
    );

    std::cout << "Logical overlap\n";
    std::cout << "first : [" << overlap.first_interval().lower()
              << ", " << overlap.first_interval().upper() << "]\n";
    std::cout << "second: [" << overlap.second_interval().lower()
              << ", " << overlap.second_interval().upper() << "]\n";
    std::cout << "computational area: "
              << overlap.computational_area() << "\n\n";

    const StructuredGrid2D clipped = Operations2D::clip_to_logical_box(
        left,
        overlap
    );

    std::cout << "Clipped grid\n" << clipped << '\n';
}
