// ============================================================================
// Arquivo: tst_CoordinateMeasures2D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Testa o componente CoordinateMeasures2D no contexto de StructuredGrid2D.
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
#include <numbers>
#include <vector>

#include <gtest/gtest.h>

#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/TwoDimensional/CoordinateSystem2D/CoordinateMappingFactory2D.h>
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

namespace fvgrid {

TEST(CoordinateMeasures2D, CartesianAreasAreContiguousAndRowMajor) {
    const StructuredGrid2D grid{
        Axis1D{std::vector<Real>{0.0,1.0,3.0}},
        Axis1D{std::vector<Real>{0.0,2.0}}
    };
    ASSERT_EQ(grid.cell_measures().size(),2U);
    EXPECT_DOUBLE_EQ(grid.cell_measures()[0],2.0);
    EXPECT_DOUBLE_EQ(grid.cell_measures()[1],4.0);
    EXPECT_DOUBLE_EQ(grid.first_face_measure(0,0),2.0);
    EXPECT_DOUBLE_EQ(grid.first_face_measure(2,0),2.0);
    EXPECT_DOUBLE_EQ(grid.second_face_measure(0,0),1.0);
    EXPECT_DOUBLE_EQ(grid.second_face_measure(1,1),2.0);
    EXPECT_EQ(grid.first_face_measures().size(),3U);
    EXPECT_EQ(grid.second_face_measures().size(),4U);
}

TEST(CoordinateMeasures2D, PolarAreasIncludeZeroAreaAxisFace) {
    const StructuredGrid2D grid{
        Axis1D{std::vector<Real>{0.0,1.0,2.0}},
        Axis1D{std::vector<Real>{0.0,std::numbers::pi/2.0}},
        PolarCoordinates2D{}
    };
    EXPECT_DOUBLE_EQ(grid.first_face_measure(0,0),0.0);
    EXPECT_NEAR(grid.first_face_measure(1,0),std::numbers::pi/2.0,1e-14);
    EXPECT_DOUBLE_EQ(grid.second_face_measure(1,0),1.0);
    EXPECT_NEAR(grid.cell_measure(0,0),std::numbers::pi/4.0,1e-14);
}

TEST(CoordinateMeasures2D, CylindricalAxisymmetricMeasuresArePhysical) {
    const StructuredGrid2D grid{
        Axis1D{std::vector<Real>{0.0,1.0}},
        Axis1D{std::vector<Real>{0.0,2.0}},
        CylindricalAxisymmetricCoordinates2D{}
    };
    EXPECT_DOUBLE_EQ(grid.first_face_measure(0,0),0.0);
    EXPECT_NEAR(grid.first_face_measure(1,0),4.0*std::numbers::pi,1e-14);
    EXPECT_NEAR(grid.second_face_measure(0,0),std::numbers::pi,1e-14);
    EXPECT_NEAR(grid.cell_measure(0,0),2.0*std::numbers::pi,1e-14);
}

TEST(CoordinateMeasures2D, SphericalAxisymmetricMeasuresArePhysical) {
    const StructuredGrid2D grid{
        Axis1D{std::vector<Real>{0.0,1.0}},
        Axis1D{std::vector<Real>{0.0,std::numbers::pi/2.0}},
        SphericalAxisymmetricCoordinates2D{}
    };
    EXPECT_DOUBLE_EQ(grid.first_face_measure(0,0),0.0);
    EXPECT_NEAR(grid.first_face_measure(1,0),2.0*std::numbers::pi,1e-14);
    EXPECT_DOUBLE_EQ(grid.second_face_measure(0,0),0.0);
    EXPECT_NEAR(grid.second_face_measure(0,1),std::numbers::pi,1e-14);
    EXPECT_NEAR(grid.cell_measure(0,0),2.0*std::numbers::pi/3.0,1e-14);
}

TEST(CoordinateMeasures2D, CustomFactoryAcceptsExactFaceMeasures) {
    const auto mapping=make_coordinate_mapping_2d(
        "Scaled","u","v",
        [](Real u,Real v){ return PhysicalPoint2D{2.0*u,3.0*v,0.0}; },
        [](CoordinateCell2D c){
            return 6.0*(c.first_max-c.first_min)*(c.second_max-c.second_min);
        },
        [](Real,Real a,Real b){ return 3.0*(b-a); },
        [](Real,Real a,Real b){ return 2.0*(b-a); }
    );
    const StructuredGrid2D grid{
        Axis1D{std::vector<Real>{0.0,1.0}},
        Axis1D{std::vector<Real>{0.0,1.0}},mapping};
    EXPECT_DOUBLE_EQ(grid.cell_measure(0,0),6.0);
    EXPECT_DOUBLE_EQ(grid.first_face_measure(0,0),3.0);
    EXPECT_DOUBLE_EQ(grid.second_face_measure(0,0),2.0);
}

}  // namespace fvgrid
