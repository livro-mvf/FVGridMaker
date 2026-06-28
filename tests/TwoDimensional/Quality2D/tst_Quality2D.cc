// ============================================================================
// Arquivo: tst_Quality2D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Testa o componente Quality2D no contexto de Quality2D.
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
#include <string_view>
#include <vector>

#include <gtest/gtest.h>

#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/TwoDimensional/Quality2D/Quality2D.h>
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

namespace fvgrid {

TEST(Quality2D, StoresClassIdentity) {
    EXPECT_EQ(Quality2D::id().module(), std::string_view{"TwoDimensional"});
    EXPECT_EQ(Quality2D::id().class_name(), std::string_view{"Quality2D"});
    EXPECT_EQ(
        Quality2D::id().class_id(),
        std::string_view{"fvgrid.2d.quality.Quality2D"}
    );
}

TEST(Quality2D, UniformCartesianGridHasUnitRatios) {
    const StructuredGrid2D grid{
        Axis1D{std::vector<Real>{0.0, 1.0, 2.0}},
        Axis1D{std::vector<Real>{0.0, 1.0, 2.0}}
    };

    const QualityReport2D report = Quality2D::evaluate(grid);

    EXPECT_DOUBLE_EQ(report.min_cell_measure, 1.0);
    EXPECT_DOUBLE_EQ(report.max_cell_measure, 1.0);
    EXPECT_DOUBLE_EQ(report.cell_measure_ratio, 1.0);
    EXPECT_DOUBLE_EQ(report.min_computational_area, 1.0);
    EXPECT_DOUBLE_EQ(report.max_computational_area, 1.0);
    EXPECT_DOUBLE_EQ(report.computational_area_ratio, 1.0);
    EXPECT_DOUBLE_EQ(
        report.max_adjacent_cell_measure_ratio_first_direction,
        1.0
    );
    EXPECT_DOUBLE_EQ(
        report.max_adjacent_cell_measure_ratio_second_direction,
        1.0
    );
    EXPECT_DOUBLE_EQ(report.min_first_face_measure, 1.0);
    EXPECT_DOUBLE_EQ(report.max_first_face_measure, 1.0);
    EXPECT_DOUBLE_EQ(report.min_second_face_measure, 1.0);
    EXPECT_DOUBLE_EQ(report.max_second_face_measure, 1.0);
}

TEST(Quality2D, NonUniformCartesianGridReportsHandComputedValues) {
    const StructuredGrid2D grid{
        Axis1D{std::vector<Real>{0.0, 1.0, 3.0}},
        Axis1D{std::vector<Real>{0.0, 1.0, 4.0}}
    };

    const QualityReport2D report = Quality2D::evaluate(grid);

    EXPECT_DOUBLE_EQ(report.min_cell_measure, 1.0);
    EXPECT_DOUBLE_EQ(report.max_cell_measure, 6.0);
    EXPECT_DOUBLE_EQ(report.cell_measure_ratio, 6.0);
    EXPECT_DOUBLE_EQ(report.min_computational_area, 1.0);
    EXPECT_DOUBLE_EQ(report.max_computational_area, 6.0);
    EXPECT_DOUBLE_EQ(report.computational_area_ratio, 6.0);
    EXPECT_DOUBLE_EQ(
        report.max_adjacent_cell_measure_ratio_first_direction,
        2.0
    );
    EXPECT_DOUBLE_EQ(
        report.max_adjacent_cell_measure_ratio_second_direction,
        3.0
    );
    EXPECT_DOUBLE_EQ(report.min_first_face_measure, 1.0);
    EXPECT_DOUBLE_EQ(report.max_first_face_measure, 3.0);
    EXPECT_DOUBLE_EQ(report.min_second_face_measure, 1.0);
    EXPECT_DOUBLE_EQ(report.max_second_face_measure, 2.0);
}

}  // namespace fvgrid
