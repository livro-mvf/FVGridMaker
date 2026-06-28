// ============================================================================
// Arquivo: tst_CentersFromFaces1D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Testa o componente CentersFromFaces1D no contexto de GridPattern1D.
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
// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <concepts>
#include <limits>
#include <string_view>
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/FVGridException.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/CentersFromFaces1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/ConstantWeight1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/CoordinateTags1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Domain1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/GridPatternConcept1D.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {
namespace {

class InvalidWeightRule final {
public:
    explicit InvalidWeightRule(Real value)
        : value_{value} {}

    [[nodiscard]] Real operator()(Size) const noexcept {
        return value_;
    }

private:
    Real value_;
};

void expect_invalid_face_count() {
    try {
        const CentersFromFaces1D pattern{ConstantWeight1D{0.5}};

        [[maybe_unused]] const AxisGeometry1D geometry =
            pattern.complete_geometry(
                std::vector<Real>{0.0},
                Domain1D::none()
            );
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.GRID.INVALID_FACE_COUNT"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Grid"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"CentersFromFaces1D"}
        );
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.grid_pattern.CentersFromFaces1D"}
        );
        return;
    }

    FAIL() << "CentersFromFaces1D accepted too few faces.";
}

void expect_invalid_reconstruction_weight(Real value) {
    try {
        const CentersFromFaces1D pattern{InvalidWeightRule{value}};

        [[maybe_unused]] const AxisGeometry1D geometry =
            pattern.complete_geometry(
                std::vector<Real>{0.0, 1.0},
                Domain1D::none()
            );
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.CORE.INVALID_ARGUMENT"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Core"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"CentersFromFaces1D"}
        );
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.grid_pattern.CentersFromFaces1D"}
        );
        return;
    }

    FAIL() << "CentersFromFaces1D accepted an invalid weight.";
}

}  // namespace

TEST(CentersFromFaces1D, MetadataIsStable) {
    using Pattern = CentersFromFaces1D<ConstantWeight1D>;

    EXPECT_EQ(Pattern::name(), std::string_view{"CentersFromFaces1D"});
    EXPECT_EQ(Pattern::class_name(), std::string_view{"CentersFromFaces1D"});
    EXPECT_EQ(
        Pattern::class_id(),
        std::string_view{"fvgrid.1d.grid_pattern.CentersFromFaces1D"}
    );
}

TEST(CentersFromFaces1D, DeclaresPrimaryCoordinateTag) {
    using Pattern = CentersFromFaces1D<ConstantWeight1D>;

    EXPECT_TRUE(
        (std::same_as<
            typename Pattern::primary_coordinates,
            FaceCoordinates1D
        >)
    );
}

TEST(CentersFromFaces1D, SatisfiesGridPatternConcept) {
    EXPECT_TRUE((GridPattern1D<CentersFromFaces1D<ConstantWeight1D>>));
}

TEST(CentersFromFaces1D, ReconstructsCentersWithHalfWeight) {
    const CentersFromFaces1D pattern{ConstantWeight1D{0.5}};

    const AxisGeometry1D geometry = pattern.complete_geometry(
        std::vector<Real>{0.0, 0.2, 0.7, 1.0},
        Domain1D::none()
    );

    EXPECT_EQ(geometry.pattern_name, std::string_view{"CentersFromFaces1D"});

    ASSERT_EQ(geometry.faces.size(), static_cast<Size>(4));
    EXPECT_DOUBLE_EQ(geometry.faces[0], 0.0);
    EXPECT_DOUBLE_EQ(geometry.faces[1], 0.2);
    EXPECT_DOUBLE_EQ(geometry.faces[2], 0.7);
    EXPECT_DOUBLE_EQ(geometry.faces[3], 1.0);

    ASSERT_EQ(geometry.centers.size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(geometry.centers[0], 0.1);
    EXPECT_DOUBLE_EQ(geometry.centers[1], 0.45);
    EXPECT_DOUBLE_EQ(geometry.centers[2], 0.85);
}

TEST(CentersFromFaces1D, ReconstructsCentersWithQuarterWeight) {
    const CentersFromFaces1D pattern{ConstantWeight1D{0.25}};

    const AxisGeometry1D geometry = pattern.complete_geometry(
        std::vector<Real>{0.0, 0.2, 0.7, 1.0},
        Domain1D::none()
    );

    ASSERT_EQ(geometry.centers.size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(geometry.centers[0], 0.05);
    EXPECT_DOUBLE_EQ(geometry.centers[1], 0.325);
    EXPECT_DOUBLE_EQ(geometry.centers[2], 0.775);
}

TEST(CentersFromFaces1D, RejectsTooFewFaces) {
    expect_invalid_face_count();
}

TEST(CentersFromFaces1D, RejectsZeroWeightReturnedByRule) {
    expect_invalid_reconstruction_weight(0.0);
}

TEST(CentersFromFaces1D, RejectsOneWeightReturnedByRule) {
    expect_invalid_reconstruction_weight(1.0);
}

TEST(CentersFromFaces1D, RejectsNegativeWeightReturnedByRule) {
    expect_invalid_reconstruction_weight(-0.25);
}

TEST(CentersFromFaces1D, RejectsWeightGreaterThanOneReturnedByRule) {
    expect_invalid_reconstruction_weight(1.25);
}

TEST(CentersFromFaces1D, RejectsInfinityReturnedByRule) {
    expect_invalid_reconstruction_weight(
        std::numeric_limits<Real>::infinity()
    );
}

TEST(CentersFromFaces1D, RejectsNaNReturnedByRule) {
    expect_invalid_reconstruction_weight(
        std::numeric_limits<Real>::quiet_NaN()
    );
}

}  // namespace fvgrid