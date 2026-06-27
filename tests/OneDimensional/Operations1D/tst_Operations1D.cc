// ----------------------------------------------------------------------------
// File: tst_Operations1D.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Tests interval and coordinate operations for 1D axes.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <limits>
#include <span>
#include <string_view>
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/FVGridException.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>
#include <FVGridMaker/OneDimensional/Operations1D/AxisInterval1D.h>
#include <FVGridMaker/OneDimensional/Operations1D/Operations1D.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

TEST(AxisInterval1D, BuildsEmptyInterval) {
    const AxisInterval1D interval = AxisInterval1D::empty();

    EXPECT_TRUE(interval.is_empty());
    EXPECT_FALSE(interval.is_point());
    EXPECT_FALSE(interval.is_interval());
    EXPECT_DOUBLE_EQ(interval.length(), 0.0);
}

TEST(AxisInterval1D, BuildsPointInterval) {
    const AxisInterval1D interval = AxisInterval1D::point(1.25);

    EXPECT_FALSE(interval.is_empty());
    EXPECT_TRUE(interval.is_point());
    EXPECT_FALSE(interval.is_interval());
    EXPECT_DOUBLE_EQ(interval.lower(), 1.25);
    EXPECT_DOUBLE_EQ(interval.upper(), 1.25);
    EXPECT_DOUBLE_EQ(interval.length(), 0.0);
}

TEST(AxisInterval1D, BuildsBoundedInterval) {
    const AxisInterval1D interval = AxisInterval1D::from_bounds(0.25, 1.0);

    EXPECT_FALSE(interval.is_empty());
    EXPECT_FALSE(interval.is_point());
    EXPECT_TRUE(interval.is_interval());
    EXPECT_DOUBLE_EQ(interval.lower(), 0.25);
    EXPECT_DOUBLE_EQ(interval.upper(), 1.0);
    EXPECT_DOUBLE_EQ(interval.length(), 0.75);
}

TEST(AxisInterval1D, EqualBoundsBuildPointInterval) {
    const AxisInterval1D interval = AxisInterval1D::from_bounds(0.5, 0.5);

    EXPECT_FALSE(interval.is_empty());
    EXPECT_TRUE(interval.is_point());
    EXPECT_FALSE(interval.is_interval());
    EXPECT_DOUBLE_EQ(interval.lower(), 0.5);
    EXPECT_DOUBLE_EQ(interval.upper(), 0.5);
    EXPECT_DOUBLE_EQ(interval.length(), 0.0);
}

TEST(AxisInterval1D, InvalidBoundsBecomeEmpty) {
    const AxisInterval1D interval = AxisInterval1D::from_bounds(2.0, 1.0);

    EXPECT_TRUE(interval.is_empty());
}

TEST(Operations1D, StoresClassIdentity) {
    EXPECT_EQ(
        Operations1D::id().module(),
        std::string_view{"OneDimensional"}
    );
    EXPECT_EQ(
        Operations1D::id().class_name(),
        std::string_view{"Operations1D"}
    );
    EXPECT_EQ(
        Operations1D::id().class_id(),
        std::string_view{"fvgrid.1d.operations.Operations1D"}
    );

    EXPECT_EQ(Operations1D::class_name(), std::string_view{"Operations1D"});
    EXPECT_EQ(
        Operations1D::class_id(),
        std::string_view{"fvgrid.1d.operations.Operations1D"}
    );
}

TEST(Operations1D, BuildsDomainIntervalFromAxis) {
    const Axis1D axis{
        std::vector<Real>{0.0, 0.25, 0.5, 0.75, 1.0}
    };

    const AxisInterval1D interval = Operations1D::domain_interval(axis);

    EXPECT_TRUE(interval.is_interval());
    EXPECT_DOUBLE_EQ(interval.lower(), 0.0);
    EXPECT_DOUBLE_EQ(interval.upper(), 1.0);
    EXPECT_DOUBLE_EQ(interval.length(), 1.0);
}

TEST(Operations1D, DetectsIntervalIntersection) {
    const Axis1D left{
        std::vector<Real>{0.0, 0.25, 0.5, 0.75, 1.0}
    };

    const Axis1D right{
        std::vector<Real>{0.35, 0.6, 0.85, 1.1, 1.35}
    };

    const AxisInterval1D interval =
        Operations1D::intersection(left, right);

    EXPECT_FALSE(interval.is_empty());
    EXPECT_FALSE(interval.is_point());
    EXPECT_TRUE(interval.is_interval());
    EXPECT_DOUBLE_EQ(interval.lower(), 0.35);
    EXPECT_DOUBLE_EQ(interval.upper(), 1.0);
}

TEST(Operations1D, DetectsEmptyIntersection) {
    const Axis1D left{
        std::vector<Real>{0.0, 0.25, 0.5, 0.75, 1.0}
    };

    const Axis1D right{
        std::vector<Real>{1.25, 1.5, 1.75, 2.0}
    };

    const AxisInterval1D interval =
        Operations1D::intersection(left, right);

    EXPECT_TRUE(interval.is_empty());
    EXPECT_FALSE(interval.is_point());
    EXPECT_FALSE(interval.is_interval());
}

TEST(Operations1D, DetectsPointIntersection) {
    const Axis1D left{
        std::vector<Real>{0.0, 0.25, 0.5, 0.75, 1.0}
    };

    const Axis1D right{
        std::vector<Real>{1.0, 1.25, 1.5, 1.75, 2.0}
    };

    const AxisInterval1D interval =
        Operations1D::intersection(left, right);

    EXPECT_FALSE(interval.is_empty());
    EXPECT_TRUE(interval.is_point());
    EXPECT_FALSE(interval.is_interval());
    EXPECT_DOUBLE_EQ(interval.lower(), 1.0);
    EXPECT_DOUBLE_EQ(interval.upper(), 1.0);
}

TEST(Operations1D, DetectsToleranceBasedPointIntersection) {
    const Axis1D left{
        std::vector<Real>{0.0, 0.25, 0.5, 0.75, 1.0}
    };

    const Axis1D right{
        std::vector<Real>{1.0 + 1.0e-12, 1.25, 1.5, 1.75, 2.0}
    };

    const AxisInterval1D interval =
        Operations1D::intersection(left, right, 1.0e-10);

    EXPECT_FALSE(interval.is_empty());
    EXPECT_TRUE(interval.is_point());
    EXPECT_NEAR(interval.lower(), 1.0, 1.0e-10);
    EXPECT_NEAR(interval.upper(), 1.0, 1.0e-10);
}

TEST(Operations1D, RequiresIntervalIntersection) {
    const Axis1D left{
        std::vector<Real>{0.0, 0.25, 0.5, 0.75, 1.0}
    };

    const Axis1D right{
        std::vector<Real>{0.5, 0.75, 1.0, 1.25}
    };

    const AxisInterval1D interval =
        Operations1D::require_interval_intersection(left, right);

    EXPECT_TRUE(interval.is_interval());
    EXPECT_DOUBLE_EQ(interval.lower(), 0.5);
    EXPECT_DOUBLE_EQ(interval.upper(), 1.0);
}

TEST(Operations1D, RejectsRequiredPointIntersection) {
    const Axis1D left{
        std::vector<Real>{0.0, 0.25, 0.5, 0.75, 1.0}
    };

    const Axis1D right{
        std::vector<Real>{1.0, 1.25, 1.5, 1.75}
    };

    try {
        [[maybe_unused]] const AxisInterval1D interval =
            Operations1D::require_interval_intersection(left, right);
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.OPERATION.EMPTY_GRID_INTERSECTION"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Operation"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"Operations1D"}
        );
        return;
    }

    FAIL() << "Operations1D accepted a point as an interval intersection.";
}

TEST(Operations1D, RejectsNegativeTolerance) {
    try {
        Operations1D::validate_tolerance(-1.0e-12);
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.OPERATION.INVALID_TOLERANCE"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Operation"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"Operations1D"}
        );
        return;
    }

    FAIL() << "Operations1D accepted a negative tolerance.";
}

TEST(Operations1D, RejectsInfiniteTolerance) {
    try {
        Operations1D::validate_tolerance(
            std::numeric_limits<Real>::infinity()
        );
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.OPERATION.INVALID_TOLERANCE"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Operation"});
        return;
    }

    FAIL() << "Operations1D accepted an infinite tolerance.";
}

TEST(Operations1D, ChecksPatternCompatibility) {
    const Axis1D left{
        std::vector<Real>{0.0, 0.5, 1.0},
        std::string_view{"PatternA"}
    };

    const Axis1D right{
        std::vector<Real>{0.0, 0.5, 1.0},
        std::string_view{"PatternB"}
    };

    EXPECT_FALSE(Operations1D::same_pattern(left, right));

    try {
        Operations1D::require_same_pattern(left, right);
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.OPERATION.INCOMPATIBLE_GRID_PATTERNS"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Operation"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"Operations1D"}
        );
        return;
    }

    FAIL() << "Operations1D accepted incompatible grid patterns.";
}

TEST(Operations1D, RemovesNearDuplicateCoordinates) {
    const std::vector<Real> coordinates{
        1.0,
        0.0,
        0.5,
        0.5 + 1.0e-12,
        0.0 + 1.0e-12
    };

    const std::vector<Real> unique_coordinates =
        Operations1D::unique_sorted_coordinates(
            std::span<const Real>{coordinates.data(), coordinates.size()},
            1.0e-10
        );

    ASSERT_EQ(unique_coordinates.size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(unique_coordinates[0], 0.0);
    EXPECT_DOUBLE_EQ(unique_coordinates[1], 0.5);
    EXPECT_DOUBLE_EQ(unique_coordinates[2], 1.0);
}

TEST(Operations1D, ClipsFacesToInterval) {
    const Axis1D axis{
        std::vector<Real>{0.0, 0.25, 0.5, 0.75, 1.0}
    };

    const AxisInterval1D interval = AxisInterval1D::from_bounds(0.2, 0.8);

    const Axis1D clipped =
        Operations1D::clip_faces_to_interval(axis, interval, 1.0e-12);

    EXPECT_EQ(clipped.pattern_name(), VolumeCentered1D::name());

    ASSERT_EQ(clipped.faces().size(), static_cast<Size>(5));
    EXPECT_DOUBLE_EQ(clipped.faces()[0], 0.2);
    EXPECT_DOUBLE_EQ(clipped.faces()[1], 0.25);
    EXPECT_DOUBLE_EQ(clipped.faces()[2], 0.5);
    EXPECT_DOUBLE_EQ(clipped.faces()[3], 0.75);
    EXPECT_DOUBLE_EQ(clipped.faces()[4], 0.8);

    ASSERT_EQ(clipped.centers().size(), static_cast<Size>(4));
    EXPECT_DOUBLE_EQ(clipped.centers()[0], 0.225);
    EXPECT_DOUBLE_EQ(clipped.centers()[1], 0.375);
    EXPECT_DOUBLE_EQ(clipped.centers()[2], 0.625);
    EXPECT_DOUBLE_EQ(clipped.centers()[3], 0.775);
}

TEST(Operations1D, ClipsFacesToIntervalWithoutInteriorFaces) {
    const Axis1D axis{
        std::vector<Real>{0.0, 0.25, 0.5, 0.75, 1.0}
    };

    const AxisInterval1D interval = AxisInterval1D::from_bounds(0.1, 0.2);

    const Axis1D clipped =
        Operations1D::clip_faces_to_interval(axis, interval, 1.0e-12);

    EXPECT_EQ(clipped.pattern_name(), VolumeCentered1D::name());

    ASSERT_EQ(clipped.faces().size(), static_cast<Size>(2));
    EXPECT_DOUBLE_EQ(clipped.faces()[0], 0.1);
    EXPECT_DOUBLE_EQ(clipped.faces()[1], 0.2);

    ASSERT_EQ(clipped.centers().size(), static_cast<Size>(1));
    EXPECT_DOUBLE_EQ(clipped.centers()[0], 0.15);
}

TEST(Operations1D, RejectsClipToEmptyOrPointInterval) {
    const Axis1D axis{
        std::vector<Real>{0.0, 0.25, 0.5, 0.75, 1.0}
    };

    try {
        [[maybe_unused]] const Axis1D clipped =
            Operations1D::clip_faces_to_interval(
                axis,
                AxisInterval1D::point(0.5)
            );
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.OPERATION.EMPTY_GRID_INTERSECTION"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Operation"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"Operations1D"}
        );
        return;
    }

    FAIL() << "Operations1D clipped an axis to a point interval.";
}

}  // namespace fvgrid