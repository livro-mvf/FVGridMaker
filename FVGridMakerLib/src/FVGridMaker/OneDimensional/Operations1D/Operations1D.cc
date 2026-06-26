// ----------------------------------------------------------------------------
// File: Operations1D.cc
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Implements interval and coordinate operations for 1D axes.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <algorithm>
#include <cmath>
#include <span>
#include <utility>
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/BuiltInErrors.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>
#include <FVGridMaker/OneDimensional/Operations1D/Operations1D.h>

namespace fvgrid {

void Operations1D::validate_tolerance(
    Real tolerance
) {
    require<errors::operation::InvalidTolerance>(
        std::isfinite(tolerance) && tolerance >= static_cast<Real>(0.0),
        Operations1D::id()
    );
}

bool Operations1D::same_pattern(
    const Axis1D& left,
    const Axis1D& right
) noexcept {
    return left.pattern_name() == right.pattern_name();
}

void Operations1D::require_same_pattern(
    const Axis1D& left,
    const Axis1D& right
) {
    require<errors::operation::IncompatibleGridPatterns>(
        same_pattern(left, right),
        Operations1D::id()
    );
}

AxisInterval1D Operations1D::domain_interval(
    const Axis1D& axis
) noexcept {
    return AxisInterval1D::from_bounds(axis.xmin(), axis.xmax());
}

AxisInterval1D Operations1D::intersection(
    const Axis1D& left,
    const Axis1D& right,
    Real tolerance
) {
    validate_tolerance(tolerance);

    const Real lower = std::max(left.xmin(), right.xmin());
    const Real upper = std::min(left.xmax(), right.xmax());

    if (upper + tolerance < lower) {
        return AxisInterval1D::empty();
    }

    if (std::abs(upper - lower) <= tolerance) {
        return AxisInterval1D::point(static_cast<Real>(0.5) * (lower + upper));
    }

    return AxisInterval1D::from_bounds(lower, upper);
}

AxisInterval1D Operations1D::require_interval_intersection(
    const Axis1D& left,
    const Axis1D& right,
    Real tolerance
) {
    const AxisInterval1D result = intersection(left, right, tolerance);

    require<errors::operation::EmptyGridIntersection>(
        result.is_interval(),
        Operations1D::id()
    );

    return result;
}

std::vector<Real> Operations1D::unique_sorted_coordinates(
    std::span<const Real> coordinates,
    Real tolerance
) {
    validate_tolerance(tolerance);

    std::vector<Real> sorted_coordinates{
        coordinates.begin(),
        coordinates.end()
    };

    if (sorted_coordinates.empty()) {
        return sorted_coordinates;
    }

    std::sort(sorted_coordinates.begin(), sorted_coordinates.end());

    std::vector<Real> unique_coordinates;
    unique_coordinates.reserve(sorted_coordinates.size());
    unique_coordinates.push_back(sorted_coordinates.front());

    for (Size i = 1; i < sorted_coordinates.size(); ++i) {
        if (sorted_coordinates[i] > unique_coordinates.back() + tolerance) {
            unique_coordinates.push_back(sorted_coordinates[i]);
        }
    }

    return unique_coordinates;
}

Axis1D Operations1D::clip_faces_to_interval(
    const Axis1D& axis,
    AxisInterval1D interval,
    Real tolerance
) {
    validate_tolerance(tolerance);

    require<errors::operation::EmptyGridIntersection>(
        interval.is_interval(),
        Operations1D::id()
    );

    std::vector<Real> clipped_faces;
    clipped_faces.reserve(axis.faces().size() + static_cast<Size>(2));

    clipped_faces.push_back(interval.lower());

    for (const Real face : axis.faces()) {
        const bool strictly_inside =
            face > interval.lower() + tolerance &&
            face < interval.upper() - tolerance;

        if (strictly_inside) {
            clipped_faces.push_back(face);
        }
    }

    clipped_faces.push_back(interval.upper());

    clipped_faces = unique_sorted_coordinates(
        std::span<const Real>{clipped_faces.data(), clipped_faces.size()},
        tolerance
    );

    require<errors::operation::EmptyGridIntersection>(
        clipped_faces.size() >= static_cast<Size>(2),
        Operations1D::id()
    );

    return Axis1D{
        std::move(clipped_faces),
        Operations1D::clipped_pattern_name()
    };
}

}  // namespace fvgrid