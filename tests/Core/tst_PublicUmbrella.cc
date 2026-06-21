#include <type_traits>

#include <gtest/gtest.h>

#include <FVGridMaker/FVGridMaker.h>

namespace fvgrid {

TEST(PublicUmbrella, ExposesMainOneAndTwoDimensionalAPI) {
    static_assert(std::is_class_v<Axis1D>);
    static_assert(std::is_class_v<Uniform1D>);
    static_assert(std::is_class_v<StructuredGrid2D>);
    static_assert(CoordinateMapping2D<CartesianCoordinates2D>);
    EXPECT_NE(version(),nullptr);
    EXPECT_STREQ(version(),"1.0.0");
}

}  // namespace fvgrid
