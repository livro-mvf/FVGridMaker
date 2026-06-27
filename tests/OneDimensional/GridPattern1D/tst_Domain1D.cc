// ----------------------------------------------------------------------------
// File: tst_Domain1D.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Tests lightweight one-dimensional physical domains.
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
#include <FVGridMaker/ErrorHandling/FVGridException.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Domain1D.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

namespace {

void expect_invalid_argument_from_unbounded_domain(
    const FVGridException& exception
) {
    EXPECT_EQ(
        exception.record().code,
        std::string_view{"FVGRID.CORE.INVALID_ARGUMENT"}
    );
    EXPECT_EQ(exception.record().category, std::string_view{"Core"});
    EXPECT_EQ(
        exception.record().source.class_name(),
        std::string_view{"Domain1D"}
    );
    EXPECT_EQ(
        exception.record().source.class_id(),
        std::string_view{"fvgrid.1d.pattern.Domain1D"}
    );
}

}  // namespace

TEST(Domain1D, NoneHasNoBounds) {
    const Domain1D domain = Domain1D::none();

    EXPECT_FALSE(domain.has_bounds());

    try {
        [[maybe_unused]] const Real xmin = domain.xmin();
    } catch (const FVGridException& exception) {
        expect_invalid_argument_from_unbounded_domain(exception);
        return;
    }

    FAIL() << "Domain1D::none() did not reject xmin().";
}

TEST(Domain1D, NoneRejectsUpperBoundAccess) {
    const Domain1D domain = Domain1D::none();

    EXPECT_FALSE(domain.has_bounds());

    try {
        [[maybe_unused]] const Real xmax = domain.xmax();
    } catch (const FVGridException& exception) {
        expect_invalid_argument_from_unbounded_domain(exception);
        return;
    }

    FAIL() << "Domain1D::none() did not reject xmax().";
}

TEST(Domain1D, NoneRejectsLengthAccess) {
    const Domain1D domain = Domain1D::none();

    EXPECT_FALSE(domain.has_bounds());

    try {
        [[maybe_unused]] const Real length = domain.length();
    } catch (const FVGridException& exception) {
        expect_invalid_argument_from_unbounded_domain(exception);
        return;
    }

    FAIL() << "Domain1D::none() did not reject length().";
}

TEST(Domain1D, BuildsFromInitialCoordinateAndLength) {
    const Domain1D domain = Domain1D::from_length(
        XInit{2.0},
        Length{3.5}
    );

    EXPECT_TRUE(domain.has_bounds());
    EXPECT_DOUBLE_EQ(domain.xmin(), 2.0);
    EXPECT_DOUBLE_EQ(domain.xmax(), 5.5);
    EXPECT_DOUBLE_EQ(domain.length(), 3.5);
}

TEST(Domain1D, BuildsFromBounds) {
    const Domain1D domain = Domain1D::from_bounds(
        XInit{-1.0},
        XFinal{2.0}
    );

    EXPECT_TRUE(domain.has_bounds());
    EXPECT_DOUBLE_EQ(domain.xmin(), -1.0);
    EXPECT_DOUBLE_EQ(domain.xmax(), 2.0);
    EXPECT_DOUBLE_EQ(domain.length(), 3.0);
}

}  // namespace fvgrid