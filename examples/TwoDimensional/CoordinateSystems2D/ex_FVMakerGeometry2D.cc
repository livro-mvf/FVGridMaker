// ============================================================================
// Arquivo: ex_FVMakerGeometry2D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Demonstra o uso de FVMakerGeometry2D com a API pública do FVGridMaker.
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
#include <numbers>
#include <string_view>
#include <vector>

#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/TwoDimensional/CoordinateSystem2D/CoordinateMetrics2D.h>
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

namespace {

void print_geometry(std::string_view title, const fvgrid::StructuredGrid2D& grid) {
    std::cout << title << '\n'
              << "  distance between first-direction faces: "
              << grid.first_axis().dx_faces().front() << '\n'
              << "  distance between first-direction centres/boundary: "
              << grid.first_axis().dx_centers().front() << '\n'
              << "  first-direction face measure: "
              << grid.first_face_measure(1,0) << '\n'
              << "  second-direction face measure: "
              << grid.second_face_measure(0,1) << '\n'
              << "  cell measure: " << grid.cell_measure(0,0) << "\n\n";
}

}  // namespace

int main() {
    using namespace fvgrid;
    const Axis1D first{std::vector<Real>{0.0,1.0,2.0}};
    const Axis1D second{std::vector<Real>{0.0,1.0}};

    print_geometry("Cartesian",StructuredGrid2D{first,second});
    print_geometry("Polar",StructuredGrid2D{first,second,PolarCoordinates2D{}});
    print_geometry("Cylindrical axisymmetric",
        StructuredGrid2D{first,second,CylindricalAxisymmetricCoordinates2D{}});

    const Axis1D theta{std::vector<Real>{0.0,std::numbers::pi/2.0}};
    print_geometry("Spherical axisymmetric",
        StructuredGrid2D{first,theta,SphericalAxisymmetricCoordinates2D{}});
}
