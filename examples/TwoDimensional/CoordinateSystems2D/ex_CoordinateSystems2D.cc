// ============================================================================
// Arquivo: ex_CoordinateSystems2D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Demonstra o uso de CoordinateSystems2D com a API pública do FVGridMaker.
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
#include <filesystem>
#include <iostream>
#include <numbers>

#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>
#include <FVGridMaker/Output/VTK/LegacyVTKRectilinearGrid2DWriter.h>
#include <FVGridMaker/TwoDimensional/CoordinateSystem2D/CoordinateSystem2D.h>
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

int main() {
    using namespace fvgrid;

    // Uma coordenada 1D para cada direção lógica; nunca x[i][j].
    const Axis1D radius = Uniform1D::make(
        NVol{12}, Length{1.0}, XInit{0.5}
    );
    const Axis1D angle = Uniform1D::make(
        NVol{24}, Length{2.0 * std::numbers::pi}, XInit{0.0}
    );

    const StructuredGrid2D polar{radius, angle, PolarCoordinates2D{}};
    std::cout << polar << "\n\n";
    write_vtk(polar, "malha_polar.vtk");

    // Sistema inventado pelo usuário: factory sem enum e sem herança virtual.
    const Axis1D q = Uniform1D::make(NVol{10}, Length{2.0}, XInit{-1.0});
    const Axis1D s = Uniform1D::make(NVol{8}, Length{1.0}, XInit{0.0});
    const auto ondulado = make_coordinate_mapping_2d(
        "Ondulado", "q", "s",
        [](Real first, Real second) {
            return PhysicalPoint2D{
                first,
                second + 0.15 * std::sin(std::numbers::pi * first),
                0.0
            };
        },
        [](CoordinateCell2D cell) {
            // O cisalhamento acima tem jacobiano unitário.
            return (cell.first_max - cell.first_min)
                 * (cell.second_max - cell.second_min);
        }
    );

    const StructuredGrid2D personalizada{q, s, ondulado};
    std::cout << personalizada << '\n';
    write_vtk(personalizada, "malha_ondulada.vtk");
}
