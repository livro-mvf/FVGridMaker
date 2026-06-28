// ============================================================================
// Arquivo: tst_CoordinateMeasureInvariants2D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Testa o componente CoordinateMeasureInvariants2D no contexto de StructuredGrid2D.
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
#include <limits>
#include <vector>

#include <gtest/gtest.h>

#include <FVGridMaker/ErrorHandling/FVGridException.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/TwoDimensional/CoordinateSystem2D/CoordinateMappingFactory2D.h>
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

namespace fvgrid {

TEST(CoordinateMeasureInvariants2D, RejectsNegativeCellMeasure) {
    const auto mapping=make_coordinate_mapping_2d(
        "Invalid","u","v",
        [](Real u,Real v){ return PhysicalPoint2D{u,v,0.0}; },
        [](CoordinateCell2D){ return -1.0; },
        [](Real,Real,Real){ return 1.0; },
        [](Real,Real,Real){ return 1.0; });
    EXPECT_THROW(
        StructuredGrid2D(Axis1D{{0.0,1.0}},Axis1D{{0.0,1.0}},mapping),
        FVGridException);
}

TEST(CoordinateMeasureInvariants2D, RejectsNonFiniteFaceMeasure) {
    const auto mapping=make_coordinate_mapping_2d(
        "Invalid","u","v",
        [](Real u,Real v){ return PhysicalPoint2D{u,v,0.0}; },
        [](CoordinateCell2D){ return 1.0; },
        [](Real,Real,Real){ return std::numeric_limits<Real>::infinity(); },
        [](Real,Real,Real){ return 1.0; });
    EXPECT_THROW(
        StructuredGrid2D(Axis1D{{0.0,1.0}},Axis1D{{0.0,1.0}},mapping),
        FVGridException);
}

TEST(CoordinateMeasureInvariants2D, RejectsNonFiniteMappedPoint) {
    const auto mapping=make_coordinate_mapping_2d(
        "InvalidMap","u","v",
        [](Real,Real){
            return PhysicalPoint2D{
                std::numeric_limits<Real>::quiet_NaN(),0.0,0.0
            };
        },
        [](CoordinateCell2D){ return 1.0; },
        [](Real,Real,Real){ return 1.0; },
        [](Real,Real,Real){ return 1.0; });

    EXPECT_THROW(
        StructuredGrid2D(Axis1D{{0.0,1.0}},Axis1D{{0.0,1.0}},mapping),
        FVGridException);
}
TEST(CoordinateMeasureInvariants2D, RejectsFaceMeasureIndexOutsideShape) {
    const StructuredGrid2D grid{Axis1D{{0.0,1.0}},Axis1D{{0.0,1.0}}};
    EXPECT_THROW((void)grid.first_face_measure(2,0),FVGridException);
    EXPECT_THROW((void)grid.second_face_measure(0,2),FVGridException);
}

}  // namespace fvgrid
