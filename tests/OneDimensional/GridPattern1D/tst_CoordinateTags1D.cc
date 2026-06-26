// ----------------------------------------------------------------------------
// File: tst_CoordinateTags1D.cc
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Tests one-dimensional coordinate type tags.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <concepts>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/OneDimensional/GridPattern1D/CoordinateTags1D.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

TEST(CoordinateTags1D, DefinesFaceCoordinateTag) {
    EXPECT_TRUE((std::same_as<FaceCoordinates1D, FaceCoordinates1D>));
    EXPECT_FALSE((std::same_as<FaceCoordinates1D, CenterCoordinates1D>));
}

TEST(CoordinateTags1D, DefinesCenterCoordinateTag) {
    EXPECT_TRUE((std::same_as<CenterCoordinates1D, CenterCoordinates1D>));
    EXPECT_FALSE((std::same_as<CenterCoordinates1D, FaceCoordinates1D>));
}

TEST(CoordinateTags1D, AcceptsOnlyKnownPrimaryCoordinateTags) {
    EXPECT_TRUE((PrimaryCoordinateTag1D<FaceCoordinates1D>));
    EXPECT_TRUE((PrimaryCoordinateTag1D<CenterCoordinates1D>));
    EXPECT_FALSE((PrimaryCoordinateTag1D<int>));
}

}  // namespace fvgrid