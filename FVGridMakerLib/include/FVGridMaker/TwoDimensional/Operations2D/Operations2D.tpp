// ----------------------------------------------------------------------------
// File: Operations2D.tpp
// Project: FVGridMaker
// Description: Template implementation for Operations2D.
// ----------------------------------------------------------------------------

#pragma once

#include <cmath>

#include <FVGridMaker/ErrorHandling/BuiltInErrors.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>

namespace fvgrid {

template <std::floating_point T>
void Operations2D::validate_tolerance(T tolerance) {
    require<errors::operation::InvalidTolerance>(
        std::isfinite(tolerance) && tolerance >= T{},
        id()
    );
}

template <std::floating_point T>
BasicLogicalBox2D<T> Operations2D::domain_box(
    const BasicStructuredGrid2D<T>& grid
) noexcept {
    return BasicLogicalBox2D<T>::from_intervals(
        Operations1D::domain_interval(grid.first_axis()),
        Operations1D::domain_interval(grid.second_axis())
    );
}

template <std::floating_point T>
BasicLogicalBox2D<T> Operations2D::intersection(
    const BasicStructuredGrid2D<T>& left,
    const BasicStructuredGrid2D<T>& right,
    T tolerance
) {
    validate_tolerance(tolerance);
    require_same_patterns(left, right);

    return BasicLogicalBox2D<T>::from_intervals(
        Operations1D::intersection(left.first_axis(), right.first_axis(), tolerance),
        Operations1D::intersection(left.second_axis(), right.second_axis(), tolerance)
    );
}

template <std::floating_point T>
BasicLogicalBox2D<T> Operations2D::require_box_intersection(
    const BasicStructuredGrid2D<T>& left,
    const BasicStructuredGrid2D<T>& right,
    T tolerance
) {
    const BasicLogicalBox2D<T> box = intersection(left, right, tolerance);

    require<errors::operation::EmptyGridIntersection>(
        box.is_area(),
        id()
    );

    return box;
}

template <std::floating_point T>
BasicStructuredGrid2D<T> Operations2D::clip_to_logical_box(
    const BasicStructuredGrid2D<T>& grid,
    BasicLogicalBox2D<T> box,
    T tolerance
) {
    const BasicCartesianCoordinates2D<T> cartesian{};

    require<errors::core::InvalidArgument>(
        grid.coordinate_system_name() == cartesian.name() &&
            grid.first_coordinate_name() == cartesian.first_coordinate_name() &&
            grid.second_coordinate_name() == cartesian.second_coordinate_name(),
        id()
    );

    return clip_to_logical_box(grid, box, cartesian, tolerance);
}

template <std::floating_point T, class Mapping>
    requires CoordinateMapping2DFor<Mapping, T>
BasicStructuredGrid2D<T> Operations2D::clip_to_logical_box(
    const BasicStructuredGrid2D<T>& grid,
    BasicLogicalBox2D<T> box,
    const Mapping& mapping,
    T tolerance
) {
    validate_tolerance(tolerance);
    require_area_box(box);
    require_volume_centered_axes(grid);

    BasicAxis1D<T> first_axis = Operations1D::clip_faces_to_interval(
        grid.first_axis(),
        box.first_interval(),
        tolerance
    );
    BasicAxis1D<T> second_axis = Operations1D::clip_faces_to_interval(
        grid.second_axis(),
        box.second_interval(),
        tolerance
    );

    return BasicStructuredGrid2D<T>{
        std::move(first_axis),
        std::move(second_axis),
        mapping
    };
}

template <std::floating_point T>
void Operations2D::require_same_patterns(
    const BasicStructuredGrid2D<T>& left,
    const BasicStructuredGrid2D<T>& right
) {
    require<errors::operation::IncompatibleGridPatterns>(
        left.first_axis().pattern_name() == right.first_axis().pattern_name() &&
            left.second_axis().pattern_name() == right.second_axis().pattern_name(),
        id()
    );
}

template <std::floating_point T>
void Operations2D::require_area_box(BasicLogicalBox2D<T> box) {
    require<errors::operation::EmptyGridIntersection>(box.is_area(), id());
}

template <std::floating_point T>
void Operations2D::require_volume_centered_axes(
    const BasicStructuredGrid2D<T>& grid
) {
    require<errors::operation::IncompatibleGridPatterns>(
        grid.first_axis().template has_pattern<VolumeCentered1D>() &&
            grid.second_axis().template has_pattern<VolumeCentered1D>(),
        id()
    );
}

}  // namespace fvgrid
