// ----------------------------------------------------------------------------
// File: tst_GridPattern1D.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Tests the built-in one-dimensional grid pattern descriptors.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <string_view>
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/OneDimensional/GridPattern1D/CoordinateKind1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Domain1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/FaceCentered1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

TEST(GridPattern1D, VolumeCenteredMetadataIsStable) {
    EXPECT_EQ(VolumeCentered1D::name(), std::string_view{"VolumeCentered1D"});
    EXPECT_EQ(
        VolumeCentered1D::class_id(),
        std::string_view{"fvgrid.1d.grid_pattern.VolumeCentered1D"}
    );
}

TEST(GridPattern1D, VolumeCenteredCoordinatesAreCorrect) {
    EXPECT_EQ(VolumeCentered1D::primary_coordinates(), std::string_view{"faces"});
    EXPECT_EQ(VolumeCentered1D::secondary_coordinates(), std::string_view{"centers"});
    EXPECT_EQ(VolumeCentered1D::input_kind(), CoordinateKind1D::Faces);
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

TEST(GridPattern1D, FaceCenteredMetadataIsStable) {
    EXPECT_EQ(FaceCentered1D::name(), std::string_view{"FaceCentered1D"});
    EXPECT_EQ(
        FaceCentered1D::class_id(),
        std::string_view{"fvgrid.1d.grid_pattern.FaceCentered1D"}
    );
}

TEST(GridPattern1D, FaceCenteredCoordinatesAreCorrect) {
    EXPECT_EQ(FaceCentered1D::primary_coordinates(), std::string_view{"centers"});
    EXPECT_EQ(FaceCentered1D::secondary_coordinates(), std::string_view{"faces"});
    EXPECT_EQ(FaceCentered1D::input_kind(), CoordinateKind1D::Centers);
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

}  // namespace fvgrid