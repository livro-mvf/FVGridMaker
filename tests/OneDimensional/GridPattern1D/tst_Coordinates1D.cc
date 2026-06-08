// ----------------------------------------------------------------------------
// File: tst_Coordinates1D.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Tests typed one-dimensional coordinate input data.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/OneDimensional/GridPattern1D/CoordinateKind1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Coordinates1D.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

TEST(Coordinates1D, StoresFaceCoordinates) {
    Coordinates1D coordinates = Coordinates1D::faces(
        std::vector<Real>{0.0, 0.2, 0.7, 1.0}
    );

    EXPECT_EQ(coordinates.kind(), CoordinateKind1D::Faces);

    ASSERT_EQ(coordinates.values().size(), static_cast<Size>(4));
    EXPECT_DOUBLE_EQ(coordinates.values()[0], 0.0);
    EXPECT_DOUBLE_EQ(coordinates.values()[1], 0.2);
    EXPECT_DOUBLE_EQ(coordinates.values()[2], 0.7);
    EXPECT_DOUBLE_EQ(coordinates.values()[3], 1.0);
}

TEST(Coordinates1D, StoresCentreCoordinates) {
    Coordinates1D coordinates = Coordinates1D::centers(
        std::vector<Real>{0.1, 0.45, 0.85}
    );

    EXPECT_EQ(coordinates.kind(), CoordinateKind1D::Centers);

    ASSERT_EQ(coordinates.values().size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(coordinates.values()[0], 0.1);
    EXPECT_DOUBLE_EQ(coordinates.values()[1], 0.45);
    EXPECT_DOUBLE_EQ(coordinates.values()[2], 0.85);
}

TEST(Coordinates1D, ReleasesStoredValues) {
    Coordinates1D coordinates = Coordinates1D::faces(
        std::vector<Real>{0.0, 0.5, 1.0}
    );

    std::vector<Real> values = coordinates.release_values();

    ASSERT_EQ(values.size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(values[0], 0.0);
    EXPECT_DOUBLE_EQ(values[1], 0.5);
    EXPECT_DOUBLE_EQ(values[2], 1.0);
}

}  // namespace fvgrid