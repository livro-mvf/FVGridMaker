#include <limits>
#include <type_traits>
#include <vector>

#include <gtest/gtest.h>

#include <FVGridMaker/ErrorHandling/FVGridException.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>

namespace fvgrid {

TEST(Axis1DInvariants, CannotBeDefaultConstructedInvalid) {
    static_assert(!std::is_default_constructible_v<Axis1D>);
    SUCCEED();
}

TEST(Axis1DInvariants, RejectsNaNCoordinate) {
    EXPECT_THROW(
        Axis1D(std::vector<Real>{0.0,std::numeric_limits<Real>::quiet_NaN(),1.0}),
        FVGridException);
}

TEST(Axis1DInvariants, RejectsInfiniteCoordinate) {
    EXPECT_THROW(
        Axis1D(std::vector<Real>{0.0,std::numeric_limits<Real>::infinity()}),
        FVGridException);
}

TEST(Axis1DInvariants, RejectsNonFiniteExplicitCenter) {
    EXPECT_THROW(
        Axis1D(std::vector<Real>{0.0,1.0},
               std::vector<Real>{std::numeric_limits<Real>::infinity()},
               "Custom"),
        FVGridException);
}

}  // namespace fvgrid

