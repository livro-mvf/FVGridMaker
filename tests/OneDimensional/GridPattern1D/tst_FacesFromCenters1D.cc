// ============================================================================
// Arquivo: tst_FacesFromCenters1D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Testa o componente FacesFromCenters1D no contexto de GridPattern1D.
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
#include <FVGridMaker/OneDimensional/GridPattern1D/ConstantWeight1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/CoordinateTags1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Domain1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/FacesFromCenters1D.h>
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

void expect_invalid_domain_without_bounds() {
    try {
        const FacesFromCenters1D pattern{ConstantWeight1D{0.5}};

        [[maybe_unused]] const AxisGeometry1D geometry =
            pattern.complete_geometry(
                std::vector<Real>{0.5},
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
            std::string_view{"FacesFromCenters1D"}
        );
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.grid_pattern.FacesFromCenters1D"}
        );
        return;
    }

    FAIL() << "FacesFromCenters1D accepted a domain without bounds.";
}

void expect_invalid_length() {
    try {
        const FacesFromCenters1D pattern{ConstantWeight1D{0.5}};

        [[maybe_unused]] const AxisGeometry1D geometry =
            pattern.complete_geometry(
                std::vector<Real>{0.5},
                Domain1D::from_bounds(XInit{1.0}, XFinal{0.0})
            );
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.GRID.INVALID_LENGTH"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Grid"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"FacesFromCenters1D"}
        );
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.grid_pattern.FacesFromCenters1D"}
        );
        return;
    }

    FAIL() << "FacesFromCenters1D accepted an invalid domain length.";
}

void expect_invalid_center_count() {
    try {
        const FacesFromCenters1D pattern{ConstantWeight1D{0.5}};

        [[maybe_unused]] const AxisGeometry1D geometry =
            pattern.complete_geometry(
                std::vector<Real>{},
                Domain1D::from_length(XInit{0.0}, Length{1.0})
            );
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.GRID.INVALID_CENTER_COUNT"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Grid"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"FacesFromCenters1D"}
        );
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.grid_pattern.FacesFromCenters1D"}
        );
        return;
    }

    FAIL() << "FacesFromCenters1D accepted an empty centre list.";
}

void expect_non_increasing_centers() {
    try {
        const FacesFromCenters1D pattern{ConstantWeight1D{0.5}};

        [[maybe_unused]] const AxisGeometry1D geometry =
            pattern.complete_geometry(
                std::vector<Real>{0.2, 0.2, 0.8},
                Domain1D::from_length(XInit{0.0}, Length{1.0})
            );
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.GRID.NON_INCREASING_CENTERS"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Grid"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"FacesFromCenters1D"}
        );
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.grid_pattern.FacesFromCenters1D"}
        );
        return;
    }

    FAIL() << "FacesFromCenters1D accepted non-increasing centres.";
}

void expect_center_out_of_range(std::vector<Real> centers) {
    try {
        const FacesFromCenters1D pattern{ConstantWeight1D{0.5}};

        [[maybe_unused]] const AxisGeometry1D geometry =
            pattern.complete_geometry(
                std::move(centers),
                Domain1D::from_length(XInit{0.0}, Length{1.0})
            );
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.CORE.OUT_OF_RANGE"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Core"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"FacesFromCenters1D"}
        );
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.grid_pattern.FacesFromCenters1D"}
        );
        return;
    }

    FAIL() << "FacesFromCenters1D accepted centres outside the domain.";
}

void expect_invalid_reconstruction_weight(Real value) {
    try {
        const FacesFromCenters1D pattern{InvalidWeightRule{value}};

        [[maybe_unused]] const AxisGeometry1D geometry =
            pattern.complete_geometry(
                std::vector<Real>{0.25, 0.75},
                Domain1D::from_length(XInit{0.0}, Length{1.0})
            );
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.CORE.INVALID_ARGUMENT"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Core"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"FacesFromCenters1D"}
        );
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.grid_pattern.FacesFromCenters1D"}
        );
        return;
    }

    FAIL() << "FacesFromCenters1D accepted an invalid weight.";
}

}  // namespace

TEST(FacesFromCenters1D, MetadataIsStable) {
    using Pattern = FacesFromCenters1D<ConstantWeight1D>;

    EXPECT_EQ(Pattern::name(), std::string_view{"FacesFromCenters1D"});
    EXPECT_EQ(Pattern::class_name(), std::string_view{"FacesFromCenters1D"});
    EXPECT_EQ(
        Pattern::class_id(),
        std::string_view{"fvgrid.1d.grid_pattern.FacesFromCenters1D"}
    );
}

TEST(FacesFromCenters1D, DeclaresPrimaryCoordinateTag) {
    using Pattern = FacesFromCenters1D<ConstantWeight1D>;

    EXPECT_TRUE(
        (std::same_as<
            typename Pattern::primary_coordinates,
            CenterCoordinates1D
        >)
    );
}

TEST(FacesFromCenters1D, SatisfiesGridPatternConcept) {
    EXPECT_TRUE((GridPattern1D<FacesFromCenters1D<ConstantWeight1D>>));
}

TEST(FacesFromCenters1D, ReconstructsFacesWithHalfWeight) {
    const FacesFromCenters1D pattern{ConstantWeight1D{0.5}};

    const AxisGeometry1D geometry = pattern.complete_geometry(
        std::vector<Real>{0.1, 0.45, 0.85},
        Domain1D::from_length(XInit{0.0}, Length{1.0})
    );

    EXPECT_EQ(geometry.pattern_name, std::string_view{"FacesFromCenters1D"});

    ASSERT_EQ(geometry.centers.size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(geometry.centers[0], 0.1);
    EXPECT_DOUBLE_EQ(geometry.centers[1], 0.45);
    EXPECT_DOUBLE_EQ(geometry.centers[2], 0.85);

    ASSERT_EQ(geometry.faces.size(), static_cast<Size>(4));
    EXPECT_DOUBLE_EQ(geometry.faces[0], 0.0);
    EXPECT_DOUBLE_EQ(geometry.faces[1], 0.275);
    EXPECT_DOUBLE_EQ(geometry.faces[2], 0.65);
    EXPECT_DOUBLE_EQ(geometry.faces[3], 1.0);
}

TEST(FacesFromCenters1D, ReconstructsFacesWithQuarterWeight) {
    const FacesFromCenters1D pattern{ConstantWeight1D{0.25}};

    const AxisGeometry1D geometry = pattern.complete_geometry(
        std::vector<Real>{0.1, 0.45, 0.85},
        Domain1D::from_length(XInit{0.0}, Length{1.0})
    );

    ASSERT_EQ(geometry.faces.size(), static_cast<Size>(4));
    EXPECT_DOUBLE_EQ(geometry.faces[0], 0.0);
    EXPECT_DOUBLE_EQ(geometry.faces[1], 0.1875);
    EXPECT_DOUBLE_EQ(geometry.faces[2], 0.55);
    EXPECT_DOUBLE_EQ(geometry.faces[3], 1.0);
}

TEST(FacesFromCenters1D, ReconstructsBoundaryFacesForSingleCenter) {
    const FacesFromCenters1D pattern{ConstantWeight1D{0.5}};

    const AxisGeometry1D geometry = pattern.complete_geometry(
        std::vector<Real>{0.5},
        Domain1D::from_length(XInit{0.0}, Length{1.0})
    );

    ASSERT_EQ(geometry.centers.size(), static_cast<Size>(1));
    EXPECT_DOUBLE_EQ(geometry.centers[0], 0.5);

    ASSERT_EQ(geometry.faces.size(), static_cast<Size>(2));
    EXPECT_DOUBLE_EQ(geometry.faces[0], 0.0);
    EXPECT_DOUBLE_EQ(geometry.faces[1], 1.0);
}

TEST(FacesFromCenters1D, RejectsDomainWithoutBounds) {
    expect_invalid_domain_without_bounds();
}

TEST(FacesFromCenters1D, RejectsInvalidDomainLength) {
    expect_invalid_length();
}

TEST(FacesFromCenters1D, RejectsEmptyCenters) {
    expect_invalid_center_count();
}

TEST(FacesFromCenters1D, RejectsNonIncreasingCenters) {
    expect_non_increasing_centers();
}

TEST(FacesFromCenters1D, RejectsLeftCenterOutsideDomain) {
    expect_center_out_of_range(std::vector<Real>{0.0, 0.5});
}

TEST(FacesFromCenters1D, RejectsRightCenterOutsideDomain) {
    expect_center_out_of_range(std::vector<Real>{0.5, 1.0});
}

TEST(FacesFromCenters1D, RejectsZeroWeightReturnedByRule) {
    expect_invalid_reconstruction_weight(0.0);
}

TEST(FacesFromCenters1D, RejectsOneWeightReturnedByRule) {
    expect_invalid_reconstruction_weight(1.0);
}

TEST(FacesFromCenters1D, RejectsNegativeWeightReturnedByRule) {
    expect_invalid_reconstruction_weight(-0.25);
}

TEST(FacesFromCenters1D, RejectsWeightGreaterThanOneReturnedByRule) {
    expect_invalid_reconstruction_weight(1.25);
}

TEST(FacesFromCenters1D, RejectsInfinityReturnedByRule) {
    expect_invalid_reconstruction_weight(
        std::numeric_limits<Real>::infinity()
    );
}

TEST(FacesFromCenters1D, RejectsNaNReturnedByRule) {
    expect_invalid_reconstruction_weight(
        std::numeric_limits<Real>::quiet_NaN()
    );
}

}  // namespace fvgrid