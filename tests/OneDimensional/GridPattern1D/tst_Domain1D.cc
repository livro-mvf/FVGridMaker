// ----------------------------------------------------------------------------
// File: tst_Domain1D.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Tests lightweight one-dimensional physical domains.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/OneDimensional/GridPattern1D/Domain1D.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

TEST(Domain1D, NoneHasNoBounds) {
    constexpr Domain1D domain = Domain1D::none();

    EXPECT_FALSE(domain.has_bounds());
    EXPECT_DOUBLE_EQ(domain.xmin(), 0.0);
    EXPECT_DOUBLE_EQ(domain.xmax(), 0.0);
    EXPECT_DOUBLE_EQ(domain.length(), 0.0);
}

TEST(Domain1D, BuildsFromInitialCoordinateAndLength) {
    constexpr Domain1D domain = Domain1D::from_length(
        XInit{2.0},
        Length{3.5}
    );

    EXPECT_TRUE(domain.has_bounds());
    EXPECT_DOUBLE_EQ(domain.xmin(), 2.0);
    EXPECT_DOUBLE_EQ(domain.xmax(), 5.5);
    EXPECT_DOUBLE_EQ(domain.length(), 3.5);
}

TEST(Domain1D, BuildsFromBounds) {
    constexpr Domain1D domain = Domain1D::from_bounds(
        XInit{-1.0},
        XFinal{2.0}
    );

    EXPECT_TRUE(domain.has_bounds());
    EXPECT_DOUBLE_EQ(domain.xmin(), -1.0);
    EXPECT_DOUBLE_EQ(domain.xmax(), 2.0);
    EXPECT_DOUBLE_EQ(domain.length(), 3.0);
}

}  // namespace fvgrid