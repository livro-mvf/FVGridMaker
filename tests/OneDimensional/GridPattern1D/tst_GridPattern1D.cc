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

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
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
}

}  // namespace fvgrid