// ----------------------------------------------------------------------------
// File: tst_ConstantWeight1D.cc
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Tests constant interpolation weights for 1D grid patterns.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <limits>
#include <string_view>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/FVGridException.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/ConstantWeight1D.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {
namespace {

void expect_invalid_weight(Real value) {
    try {
        [[maybe_unused]] const ConstantWeight1D weight{value};
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.CORE.INVALID_ARGUMENT"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Core"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"ConstantWeight1D"}
        );
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.grid_pattern.ConstantWeight1D"}
        );
        return;
    }

    FAIL() << "ConstantWeight1D accepted an invalid weight.";
}

}  // namespace

TEST(ConstantWeight1D, MetadataIsStable) {
    EXPECT_EQ(
        ConstantWeight1D::class_name(),
        std::string_view{"ConstantWeight1D"}
    );
    EXPECT_EQ(
        ConstantWeight1D::class_id(),
        std::string_view{"fvgrid.1d.grid_pattern.ConstantWeight1D"}
    );
}

TEST(ConstantWeight1D, StoresWeightValue) {
    const ConstantWeight1D weight{0.25};

    EXPECT_DOUBLE_EQ(weight.value(), 0.25);
}

TEST(ConstantWeight1D, ReturnsSameWeightForAnyIndex) {
    const ConstantWeight1D weight{0.375};

    EXPECT_DOUBLE_EQ(weight(static_cast<Size>(0)), 0.375);
    EXPECT_DOUBLE_EQ(weight(static_cast<Size>(1)), 0.375);
    EXPECT_DOUBLE_EQ(weight(static_cast<Size>(100)), 0.375);
}

TEST(ConstantWeight1D, AcceptsOpenUnitInterval) {
    EXPECT_NO_THROW([[maybe_unused]] const ConstantWeight1D lower{1.0e-12});
    EXPECT_NO_THROW([[maybe_unused]] const ConstantWeight1D middle{0.5});
    EXPECT_NO_THROW([[maybe_unused]] const ConstantWeight1D upper{1.0 - 1.0e-12});
}

TEST(ConstantWeight1D, RejectsZero) {
    expect_invalid_weight(0.0);
}

TEST(ConstantWeight1D, RejectsOne) {
    expect_invalid_weight(1.0);
}

TEST(ConstantWeight1D, RejectsNegativeValue) {
    expect_invalid_weight(-0.25);
}

TEST(ConstantWeight1D, RejectsValueGreaterThanOne) {
    expect_invalid_weight(1.25);
}

TEST(ConstantWeight1D, RejectsInfinity) {
    expect_invalid_weight(std::numeric_limits<Real>::infinity());
}

TEST(ConstantWeight1D, RejectsNaN) {
    expect_invalid_weight(std::numeric_limits<Real>::quiet_NaN());
}

}  // namespace fvgrid