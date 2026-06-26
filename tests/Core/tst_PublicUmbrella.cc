// ----------------------------------------------------------------------------
// File: tst_PublicUmbrella.cc
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Tests the public umbrella header exposed by FVGridMaker.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <type_traits>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/FVGridMaker.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

TEST(PublicUmbrella, ExposesMainOneAndTwoDimensionalAPI) {
    static_assert(std::is_class_v<Axis1D>);
    static_assert(std::is_class_v<Uniform1D>);
    static_assert(std::is_class_v<StructuredGrid2D>);
    static_assert(CoordinateMapping2D<CartesianCoordinates2D>);

    EXPECT_NE(version(), nullptr);
    EXPECT_STREQ(version(), version_string().data());
    EXPECT_EQ(version_string(), std::string_view{"0.2.0"});
}

}  // namespace fvgrid