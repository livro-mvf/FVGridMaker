// ----------------------------------------------------------------------------
// File: Operations1D.tpp
// Project: FVGridMaker
// Description: Template implementation for Operations1D.
// ----------------------------------------------------------------------------

#pragma once

#include <algorithm>
#include <cmath>
#include <span>
#include <utility>
#include <vector>

#include <FVGridMaker/ErrorHandling/BuiltInErrors.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>

namespace fvgrid {

template <std::floating_point T>
void Operations1D::validate_tolerance(T tolerance) {
    require<errors::operation::InvalidTolerance>(
        std::isfinite(tolerance) && tolerance >= T{},
        Operations1D::id()
    );
}

template <std::floating_point T>
bool Operations1D::same_pattern(
    const BasicAxis1D<T>& left,
    const BasicAxis1D<T>& right
) noexcept {
    return left.pattern_name() == right.pattern_name();
}

template <std::floating_point T>
void Operations1D::require_same_pattern(
    const BasicAxis1D<T>& left,
    const BasicAxis1D<T>& right
) {
    require<errors::operation::IncompatibleGridPatterns>(
        same_pattern(left, right),
        Operations1D::id()
    );
}

template <std::floating_point T>
BasicAxisInterval1D<T> Operations1D::domain_interval(
    const BasicAxis1D<T>& axis
) noexcept {
    return BasicAxisInterval1D<T>::from_bounds(axis.xmin(), axis.xmax());
}

template <std::floating_point T>
BasicAxisInterval1D<T> Operations1D::intersection(
    const BasicAxis1D<T>& left,
    const BasicAxis1D<T>& right,
    T tolerance
) {
    validate_tolerance(tolerance);

    const T lower = std::max(left.xmin(), right.xmin());
    const T upper = std::min(left.xmax(), right.xmax());

    if (upper + tolerance < lower) {
        return BasicAxisInterval1D<T>::empty();
    }

    if (std::abs(upper - lower) <= tolerance) {
        return BasicAxisInterval1D<T>::point(T{0.5} * (lower + upper));
    }

    return BasicAxisInterval1D<T>::from_bounds(lower, upper);
}

template <std::floating_point T>
BasicAxisInterval1D<T> Operations1D::require_interval_intersection(
    const BasicAxis1D<T>& left,
    const BasicAxis1D<T>& right,
    T tolerance
) {
    const BasicAxisInterval1D<T> result = intersection(left, right, tolerance);

    require<errors::operation::EmptyGridIntersection>(
        result.is_interval(),
        Operations1D::id()
    );

    return result;
}

template <std::floating_point T>
std::vector<T> Operations1D::unique_sorted_coordinates(
    std::span<const T> coordinates,
    T tolerance
) {
    validate_tolerance(tolerance);

    std::vector<T> sorted_coordinates{coordinates.begin(), coordinates.end()};

    if (sorted_coordinates.empty()) {
        return sorted_coordinates;
    }

    std::sort(sorted_coordinates.begin(), sorted_coordinates.end());

    std::vector<T> unique_coordinates;
    unique_coordinates.reserve(sorted_coordinates.size());
    unique_coordinates.push_back(sorted_coordinates.front());

    for (Size i = 1; i < sorted_coordinates.size(); ++i) {
        if (sorted_coordinates[i] > unique_coordinates.back() + tolerance) {
            unique_coordinates.push_back(sorted_coordinates[i]);
        }
    }

    return unique_coordinates;
}

template <std::floating_point T>
BasicAxis1D<T> Operations1D::clip_faces_to_interval(
    const BasicAxis1D<T>& axis,
    BasicAxisInterval1D<T> interval,
    T tolerance
) {
    validate_tolerance(tolerance);

    require<errors::operation::EmptyGridIntersection>(
        interval.is_interval(),
        Operations1D::id()
    );

    require<errors::operation::IncompatibleGridPatterns>(
        axis.template has_pattern<VolumeCentered1D>(),
        Operations1D::id()
    );

    std::vector<T> clipped_faces;
    clipped_faces.reserve(axis.faces().size() + static_cast<Size>(2));

    clipped_faces.push_back(interval.lower());

    for (const T face : axis.faces()) {
        const bool strictly_inside =
            face > interval.lower() + tolerance &&
            face < interval.upper() - tolerance;

        if (strictly_inside) {
            clipped_faces.push_back(face);
        }
    }

    clipped_faces.push_back(interval.upper());

    clipped_faces = unique_sorted_coordinates(
        std::span<const T>{clipped_faces.data(), clipped_faces.size()},
        tolerance
    );

    require<errors::operation::EmptyGridIntersection>(
        clipped_faces.size() >= static_cast<Size>(2),
        Operations1D::id()
    );

    return BasicAxis1D<T>{std::move(clipped_faces)};
}

}  // namespace fvgrid
