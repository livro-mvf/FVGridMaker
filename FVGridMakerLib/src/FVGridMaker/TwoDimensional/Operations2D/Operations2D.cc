// ----------------------------------------------------------------------------
// File: Operations2D.cc
// Project: FVGridMaker
// Description: Implements logical operations for StructuredGrid2D.
// License: MIT
// ----------------------------------------------------------------------------

#include <cmath>

#include <FVGridMaker/TwoDimensional/Operations2D/Operations2D.h>

namespace fvgrid {

void Operations2D::validate_tolerance(Real tolerance) {
    require<errors::operation::InvalidTolerance>(
        std::isfinite(tolerance) && tolerance >= Real{},
        id()
    );
}

LogicalBox2D Operations2D::domain_box(
    const StructuredGrid2D& grid
) noexcept {
    return LogicalBox2D::from_intervals(
        Operations1D::domain_interval(grid.first_axis()),
        Operations1D::domain_interval(grid.second_axis())
    );
}

LogicalBox2D Operations2D::intersection(
    const StructuredGrid2D& left,
    const StructuredGrid2D& right,
    Real tolerance
) {
    validate_tolerance(tolerance);
    require_same_patterns(left, right);

    return LogicalBox2D::from_intervals(
        Operations1D::intersection(
            left.first_axis(),
            right.first_axis(),
            tolerance
        ),
        Operations1D::intersection(
            left.second_axis(),
            right.second_axis(),
            tolerance
        )
    );
}

LogicalBox2D Operations2D::require_box_intersection(
    const StructuredGrid2D& left,
    const StructuredGrid2D& right,
    Real tolerance
) {
    const LogicalBox2D box = intersection(left, right, tolerance);

    require<errors::operation::EmptyGridIntersection>(
        box.is_area(),
        id()
    );

    return box;
}

StructuredGrid2D Operations2D::clip_to_logical_box(
    const StructuredGrid2D& grid,
    LogicalBox2D box,
    Real tolerance
) {
    require<errors::core::InvalidArgument>(
        grid.coordinate_system_name() == CartesianCoordinates2D{}.name()
            && grid.first_coordinate_name()
                == CartesianCoordinates2D{}.first_coordinate_name()
            && grid.second_coordinate_name()
                == CartesianCoordinates2D{}.second_coordinate_name(),
        id()
    );

    return clip_to_logical_box(grid, box, CartesianCoordinates2D{}, tolerance);
}

void Operations2D::require_same_patterns(
    const StructuredGrid2D& left,
    const StructuredGrid2D& right
) {
    require<errors::operation::IncompatibleGridPatterns>(
        left.first_axis().pattern_name() == right.first_axis().pattern_name()
            && left.second_axis().pattern_name()
                == right.second_axis().pattern_name(),
        id()
    );
}

void Operations2D::require_area_box(LogicalBox2D box) {
    require<errors::operation::EmptyGridIntersection>(
        box.is_area(),
        id()
    );
}

void Operations2D::require_volume_centered_axes(const StructuredGrid2D& grid) {
    require<errors::operation::IncompatibleGridPatterns>(
        grid.first_axis().has_pattern<VolumeCentered1D>()
            && grid.second_axis().has_pattern<VolumeCentered1D>(),
        id()
    );
}

}  // namespace fvgrid
