// ----------------------------------------------------------------------------
// File: tst_GridPattern1D.cc
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Tests the built-in one-dimensional grid pattern descriptors.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <concepts>
#include <string_view>
#include <utility>
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/OneDimensional/GridPattern1D/AxisGeometry1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/CentersFromFaces1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/ConstantWeight1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/CoordinateTags1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Domain1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/FaceCentered1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/FacesFromCenters1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/GridPatternConcept1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {
namespace {

struct PatternWithoutPrimaryCoordinateTag final {
    [[nodiscard]] static constexpr std::string_view name() noexcept {
        return "PatternWithoutPrimaryCoordinateTag";
    }

    [[nodiscard]] static AxisGeometry1D complete_geometry(
        std::vector<Real> coordinates,
        Domain1D
    ) {
        return AxisGeometry1D{
            std::move(coordinates),
            std::vector<Real>{},
            "PatternWithoutPrimaryCoordinateTag"
        };
    }
};

struct PatternWithInvalidPrimaryCoordinateTag final {
    using primary_coordinates = int;

    [[nodiscard]] static constexpr std::string_view name() noexcept {
        return "PatternWithInvalidPrimaryCoordinateTag";
    }

    [[nodiscard]] static AxisGeometry1D complete_geometry(
        std::vector<Real> coordinates,
        Domain1D
    ) {
        return AxisGeometry1D{
            std::move(coordinates),
            std::vector<Real>{},
            "PatternWithInvalidPrimaryCoordinateTag"
        };
    }
};

struct PatternWithoutCompleteGeometry final {
    using primary_coordinates = FaceCoordinates1D;

    [[nodiscard]] static constexpr std::string_view name() noexcept {
        return "PatternWithoutCompleteGeometry";
    }
};

struct PatternWithInvalidCompleteGeometryReturn final {
    using primary_coordinates = FaceCoordinates1D;

    [[nodiscard]] static constexpr std::string_view name() noexcept {
        return "PatternWithInvalidCompleteGeometryReturn";
    }

    [[nodiscard]] static int complete_geometry(
        std::vector<Real>,
        Domain1D
    ) {
        return 0;
    }
};

}  // namespace

TEST(GridPattern1D, VolumeCenteredMetadataIsStable) {
    EXPECT_EQ(VolumeCentered1D::name(), std::string_view{"VolumeCentered1D"});
    EXPECT_EQ(
        VolumeCentered1D::class_id(),
        std::string_view{"fvgrid.1d.grid_pattern.VolumeCentered1D"}
    );
}

TEST(GridPattern1D, VolumeCenteredCoordinateNamesAreStable) {
    EXPECT_EQ(
        VolumeCentered1D::primary_coordinates_name(),
        std::string_view{"faces"}
    );
    EXPECT_EQ(
        VolumeCentered1D::secondary_coordinates(),
        std::string_view{"centers"}
    );
}

TEST(GridPattern1D, VolumeCenteredDeclaresPrimaryCoordinateTag) {
    EXPECT_TRUE(
        (std::same_as<
            typename VolumeCentered1D::primary_coordinates,
            FaceCoordinates1D
        >)
    );
}

TEST(GridPattern1D, VolumeCenteredSatisfiesGridPatternConcept) {
    EXPECT_TRUE((GridPattern1D<VolumeCentered1D>));
}

TEST(GridPattern1D, VolumeCenteredCompletesGeometryFromFaces) {
    AxisGeometry1D geometry = VolumeCentered1D::complete_geometry(
        std::vector<Real>{0.0, 0.2, 0.7, 1.0},
        Domain1D::none()
    );

    EXPECT_EQ(geometry.pattern_name, VolumeCentered1D::name());

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

TEST(GridPattern1D, VolumeCenteredMatchesCentersFromFacesWithHalfWeight) {
    AxisGeometry1D volume_centered_geometry =
        VolumeCentered1D::complete_geometry(
            std::vector<Real>{0.0, 0.2, 0.7, 1.0},
            Domain1D::none()
        );

    const CentersFromFaces1D generic_pattern{ConstantWeight1D{0.5}};

    AxisGeometry1D generic_geometry = generic_pattern.complete_geometry(
        std::vector<Real>{0.0, 0.2, 0.7, 1.0},
        Domain1D::none()
    );

    EXPECT_EQ(
        volume_centered_geometry.pattern_name,
        std::string_view{"VolumeCentered1D"}
    );
    EXPECT_EQ(
        generic_geometry.pattern_name,
        std::string_view{"CentersFromFaces1D"}
    );

    ASSERT_EQ(
        volume_centered_geometry.faces.size(),
        generic_geometry.faces.size()
    );
    ASSERT_EQ(
        volume_centered_geometry.centers.size(),
        generic_geometry.centers.size()
    );

    for (Size i = 0; i < volume_centered_geometry.faces.size(); ++i) {
        EXPECT_DOUBLE_EQ(
            volume_centered_geometry.faces[i],
            generic_geometry.faces[i]
        );
    }

    for (Size i = 0; i < volume_centered_geometry.centers.size(); ++i) {
        EXPECT_DOUBLE_EQ(
            volume_centered_geometry.centers[i],
            generic_geometry.centers[i]
        );
    }
}

TEST(GridPattern1D, FaceCenteredMetadataIsStable) {
    EXPECT_EQ(FaceCentered1D::name(), std::string_view{"FaceCentered1D"});
    EXPECT_EQ(
        FaceCentered1D::class_id(),
        std::string_view{"fvgrid.1d.grid_pattern.FaceCentered1D"}
    );
}

TEST(GridPattern1D, FaceCenteredCoordinateNamesAreStable) {
    EXPECT_EQ(
        FaceCentered1D::primary_coordinates_name(),
        std::string_view{"centers"}
    );
    EXPECT_EQ(
        FaceCentered1D::secondary_coordinates(),
        std::string_view{"faces"}
    );
}

TEST(GridPattern1D, FaceCenteredDeclaresPrimaryCoordinateTag) {
    EXPECT_TRUE(
        (std::same_as<
            typename FaceCentered1D::primary_coordinates,
            CenterCoordinates1D
        >)
    );
}

TEST(GridPattern1D, FaceCenteredSatisfiesGridPatternConcept) {
    EXPECT_TRUE((GridPattern1D<FaceCentered1D>));
}

TEST(GridPattern1D, FaceCenteredCompletesGeometryFromCenters) {
    AxisGeometry1D geometry = FaceCentered1D::complete_geometry(
        std::vector<Real>{0.1, 0.45, 0.85},
        Domain1D::from_length(XInit{0.0}, Length{1.0})
    );

    EXPECT_EQ(geometry.pattern_name, FaceCentered1D::name());

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

TEST(GridPattern1D, FaceCenteredMatchesFacesFromCentersWithHalfWeight) {
    AxisGeometry1D face_centered_geometry =
        FaceCentered1D::complete_geometry(
            std::vector<Real>{0.1, 0.45, 0.85},
            Domain1D::from_length(XInit{0.0}, Length{1.0})
        );

    const FacesFromCenters1D generic_pattern{ConstantWeight1D{0.5}};

    AxisGeometry1D generic_geometry = generic_pattern.complete_geometry(
        std::vector<Real>{0.1, 0.45, 0.85},
        Domain1D::from_length(XInit{0.0}, Length{1.0})
    );

    EXPECT_EQ(
        face_centered_geometry.pattern_name,
        std::string_view{"FaceCentered1D"}
    );
    EXPECT_EQ(
        generic_geometry.pattern_name,
        std::string_view{"FacesFromCenters1D"}
    );

    ASSERT_EQ(
        face_centered_geometry.faces.size(),
        generic_geometry.faces.size()
    );
    ASSERT_EQ(
        face_centered_geometry.centers.size(),
        generic_geometry.centers.size()
    );

    for (Size i = 0; i < face_centered_geometry.faces.size(); ++i) {
        EXPECT_DOUBLE_EQ(
            face_centered_geometry.faces[i],
            generic_geometry.faces[i]
        );
    }

    for (Size i = 0; i < face_centered_geometry.centers.size(); ++i) {
        EXPECT_DOUBLE_EQ(
            face_centered_geometry.centers[i],
            generic_geometry.centers[i]
        );
    }
}

TEST(GridPattern1D, BuiltInPatternsDeclarePrimaryCoordinateTags) {
    EXPECT_TRUE(
        (std::same_as<
            typename VolumeCentered1D::primary_coordinates,
            FaceCoordinates1D
        >)
    );

    EXPECT_TRUE(
        (std::same_as<
            typename FaceCentered1D::primary_coordinates,
            CenterCoordinates1D
        >)
    );
}

TEST(GridPattern1D, RejectsPatternWithoutPrimaryCoordinateTag) {
    EXPECT_FALSE((GridPattern1D<PatternWithoutPrimaryCoordinateTag>));
}

TEST(GridPattern1D, RejectsPatternWithInvalidPrimaryCoordinateTag) {
    EXPECT_FALSE((GridPattern1D<PatternWithInvalidPrimaryCoordinateTag>));
}

TEST(GridPattern1D, RejectsPatternWithoutCompleteGeometry) {
    EXPECT_FALSE((GridPattern1D<PatternWithoutCompleteGeometry>));
}

TEST(GridPattern1D, RejectsPatternWithInvalidCompleteGeometryReturn) {
    EXPECT_FALSE((GridPattern1D<PatternWithInvalidCompleteGeometryReturn>));
}

}  // namespace fvgrid