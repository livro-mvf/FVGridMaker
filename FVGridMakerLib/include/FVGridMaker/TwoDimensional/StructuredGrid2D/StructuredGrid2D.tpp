// ----------------------------------------------------------------------------
// File: StructuredGrid2D.tpp
// Project: FVGridMaker
// Description: Template implementation for BasicStructuredGrid2D.
// ----------------------------------------------------------------------------

#pragma once

#include <cmath>
#include <ostream>
#include <utility>

#include <FVGridMaker/ErrorHandling/BuiltInErrors.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>

namespace fvgrid {

template <std::floating_point T>
BasicStructuredGrid2D<T>::BasicStructuredGrid2D(
    BasicAxis1D<T> first_axis,
    BasicAxis1D<T> second_axis
)
    : BasicStructuredGrid2D(
          std::move(first_axis),
          std::move(second_axis),
          BasicCartesianCoordinates2D<T>{}
      ) {}

template <std::floating_point T>
std::string_view BasicStructuredGrid2D<T>::coordinate_system_name()
    const noexcept {
    return coordinate_system_name_;
}

template <std::floating_point T>
std::string_view BasicStructuredGrid2D<T>::first_coordinate_name()
    const noexcept {
    return first_coordinate_name_;
}

template <std::floating_point T>
std::string_view BasicStructuredGrid2D<T>::second_coordinate_name()
    const noexcept {
    return second_coordinate_name_;
}

template <std::floating_point T>
bool BasicStructuredGrid2D<T>::vtk_rectilinear() const noexcept {
    return vtk_rectilinear_;
}

template <std::floating_point T>
const BasicAxis1D<T>& BasicStructuredGrid2D<T>::first_axis() const noexcept {
    return first_axis_;
}

template <std::floating_point T>
const BasicAxis1D<T>& BasicStructuredGrid2D<T>::second_axis() const noexcept {
    return second_axis_;
}

template <std::floating_point T>
const BasicAxis1D<T>& BasicStructuredGrid2D<T>::x_axis() const noexcept {
    return first_axis_;
}

template <std::floating_point T>
const BasicAxis1D<T>& BasicStructuredGrid2D<T>::y_axis() const noexcept {
    return second_axis_;
}

template <std::floating_point T>
Size BasicStructuredGrid2D<T>::num_cells_x() const noexcept {
    return first_axis_.num_cells();
}

template <std::floating_point T>
Size BasicStructuredGrid2D<T>::num_cells_y() const noexcept {
    return second_axis_.num_cells();
}

template <std::floating_point T>
Size BasicStructuredGrid2D<T>::num_cells() const noexcept {
    return num_cells_x() * num_cells_y();
}

template <std::floating_point T>
Size BasicStructuredGrid2D<T>::num_faces_x() const noexcept {
    return first_axis_.num_faces();
}

template <std::floating_point T>
Size BasicStructuredGrid2D<T>::num_faces_y() const noexcept {
    return second_axis_.num_faces();
}

template <std::floating_point T>
T BasicStructuredGrid2D<T>::xmin() const noexcept {
    return first_axis_.xmin();
}

template <std::floating_point T>
T BasicStructuredGrid2D<T>::xmax() const noexcept {
    return first_axis_.xmax();
}

template <std::floating_point T>
T BasicStructuredGrid2D<T>::ymin() const noexcept {
    return second_axis_.xmin();
}

template <std::floating_point T>
T BasicStructuredGrid2D<T>::ymax() const noexcept {
    return second_axis_.xmax();
}

template <std::floating_point T>
T BasicStructuredGrid2D<T>::length_x() const noexcept {
    return first_axis_.length();
}

template <std::floating_point T>
T BasicStructuredGrid2D<T>::length_y() const noexcept {
    return second_axis_.length();
}

template <std::floating_point T>
Size BasicStructuredGrid2D<T>::linear_cell_index(Size i, Size j) const {
    validate_cell_index(i, j);
    return j * num_cells_x() + i;
}

template <std::floating_point T>
T BasicStructuredGrid2D<T>::first_face(Size i) const {
    validate_x_face_index(i);
    return first_axis_.faces()[i];
}

template <std::floating_point T>
T BasicStructuredGrid2D<T>::second_face(Size j) const {
    validate_y_face_index(j);
    return second_axis_.faces()[j];
}

template <std::floating_point T>
T BasicStructuredGrid2D<T>::first_center(Size i) const {
    validate_x_cell_index(i);
    return first_axis_.centers()[i];
}

template <std::floating_point T>
T BasicStructuredGrid2D<T>::second_center(Size j) const {
    validate_y_cell_index(j);
    return second_axis_.centers()[j];
}

template <std::floating_point T>
T BasicStructuredGrid2D<T>::first_cell_length(Size i) const {
    validate_x_cell_index(i);
    return first_axis_.dx_faces()[i];
}

template <std::floating_point T>
T BasicStructuredGrid2D<T>::second_cell_length(Size j) const {
    validate_y_cell_index(j);
    return second_axis_.dx_faces()[j];
}

template <std::floating_point T>
T BasicStructuredGrid2D<T>::x_face(Size i) const {
    return first_face(i);
}

template <std::floating_point T>
T BasicStructuredGrid2D<T>::y_face(Size j) const {
    return second_face(j);
}

template <std::floating_point T>
T BasicStructuredGrid2D<T>::x_center(Size i) const {
    return first_center(i);
}

template <std::floating_point T>
T BasicStructuredGrid2D<T>::y_center(Size j) const {
    return second_center(j);
}

template <std::floating_point T>
T BasicStructuredGrid2D<T>::x_cell_length(Size i) const {
    return first_cell_length(i);
}

template <std::floating_point T>
T BasicStructuredGrid2D<T>::y_cell_length(Size j) const {
    return second_cell_length(j);
}

template <std::floating_point T>
T BasicStructuredGrid2D<T>::cell_logical_area(Size i, Size j) const {
    validate_cell_index(i, j);
    return first_cell_length(i) * second_cell_length(j);
}

template <std::floating_point T>
T BasicStructuredGrid2D<T>::cell_area(Size i, Size j) const {
    return cell_logical_area(i, j);
}

template <std::floating_point T>
T BasicStructuredGrid2D<T>::cell_measure(Size i, Size j) const {
    validate_cell_index(i, j);
    return cell_measures_[linear_cell_index(i, j)];
}

template <std::floating_point T>
T BasicStructuredGrid2D<T>::first_face_measure(Size i, Size j) const {
    validate_x_face_index(i);
    validate_y_cell_index(j);
    return first_face_measures_[j * num_faces_x() + i];
}

template <std::floating_point T>
T BasicStructuredGrid2D<T>::second_face_measure(Size i, Size j) const {
    validate_x_cell_index(i);
    validate_y_face_index(j);
    return second_face_measures_[j * num_cells_x() + i];
}

template <std::floating_point T>
std::span<const T> BasicStructuredGrid2D<T>::cell_measures() const noexcept {
    return cell_measures_;
}

template <std::floating_point T>
std::span<const T> BasicStructuredGrid2D<T>::first_face_measures()
    const noexcept {
    return first_face_measures_;
}

template <std::floating_point T>
std::span<const T> BasicStructuredGrid2D<T>::second_face_measures()
    const noexcept {
    return second_face_measures_;
}

template <std::floating_point T>
T BasicStructuredGrid2D<T>::vertical_face_length(Size j) const {
    return second_cell_length(j);
}

template <std::floating_point T>
T BasicStructuredGrid2D<T>::horizontal_face_length(Size i) const {
    return first_cell_length(i);
}

template <std::floating_point T>
BasicPhysicalPoint2D<T> BasicStructuredGrid2D<T>::physical_vertex(
    Size i,
    Size j
) const {
    validate_x_face_index(i);
    validate_y_face_index(j);
    return physical_face_points_[j * num_faces_x() + i];
}

template <std::floating_point T>
BasicPhysicalPoint2D<T> BasicStructuredGrid2D<T>::physical_face_point(
    Size i,
    Size j
) const {
    return physical_vertex(i, j);
}

template <std::floating_point T>
BasicPhysicalPoint2D<T> BasicStructuredGrid2D<T>::physical_cell_center(
    Size i,
    Size j
) const {
    validate_cell_index(i, j);
    return physical_cell_centers_[linear_cell_index(i, j)];
}

template <std::floating_point T>
BasicPhysicalPoint2D<T> BasicStructuredGrid2D<T>::physical_first_face_center(
    Size i,
    Size j
) const {
    validate_x_face_index(i);
    validate_y_cell_index(j);
    return physical_first_face_centers_[j * num_faces_x() + i];
}

template <std::floating_point T>
BasicPhysicalPoint2D<T> BasicStructuredGrid2D<T>::physical_second_face_center(
    Size i,
    Size j
) const {
    validate_x_cell_index(i);
    validate_y_face_index(j);
    return physical_second_face_centers_[j * num_cells_x() + i];
}

template <std::floating_point T>
bool BasicStructuredGrid2D<T>::physical_point_is_finite(
    BasicPhysicalPoint2D<T> point
) noexcept {
    return std::isfinite(point.x) &&
        std::isfinite(point.y) &&
        std::isfinite(point.z);
}

template <std::floating_point T>
void BasicStructuredGrid2D<T>::validate_axis_patterns() const {
    require<errors::operation::IncompatibleGridPatterns>(
        first_axis_.pattern_name() == second_axis_.pattern_name(),
        id()
    );
}

template <std::floating_point T>
void BasicStructuredGrid2D<T>::validate_x_cell_index(Size i) const {
    require<errors::core::OutOfRange>(i < num_cells_x(), id());
}

template <std::floating_point T>
void BasicStructuredGrid2D<T>::validate_y_cell_index(Size j) const {
    require<errors::core::OutOfRange>(j < num_cells_y(), id());
}

template <std::floating_point T>
void BasicStructuredGrid2D<T>::validate_x_face_index(Size i) const {
    require<errors::core::OutOfRange>(i < num_faces_x(), id());
}

template <std::floating_point T>
void BasicStructuredGrid2D<T>::validate_y_face_index(Size j) const {
    require<errors::core::OutOfRange>(j < num_faces_y(), id());
}

template <std::floating_point T>
void BasicStructuredGrid2D<T>::validate_cell_index(Size i, Size j) const {
    validate_x_cell_index(i);
    validate_y_cell_index(j);
}

template <std::floating_point T>
template <class Mapping>
    requires CoordinateMapping2DFor<Mapping, T>
void BasicStructuredGrid2D<T>::build_derived_geometry(const Mapping& mapping) {
    physical_face_points_.resize(num_faces_x() * num_faces_y());

    for (Size j = 0; j < num_faces_y(); ++j) {
        for (Size i = 0; i < num_faces_x(); ++i) {
            const BasicPhysicalPoint2D<T> point =
                mapping.map(first_face(i), second_face(j));

            require<errors::core::InvalidArgument>(
                physical_point_is_finite(point),
                id()
            );

            physical_face_points_[j * num_faces_x() + i] = point;
        }
    }

    physical_cell_centers_.resize(num_cells());

    for (Size j = 0; j < num_cells_y(); ++j) {
        for (Size i = 0; i < num_cells_x(); ++i) {
            const BasicPhysicalPoint2D<T> point =
                mapping.map(first_center(i), second_center(j));

            require<errors::core::InvalidArgument>(
                physical_point_is_finite(point),
                id()
            );

            physical_cell_centers_[linear_cell_index(i, j)] = point;
        }
    }

    physical_first_face_centers_.resize(num_faces_x() * num_cells_y());

    for (Size j = 0; j < num_cells_y(); ++j) {
        for (Size i = 0; i < num_faces_x(); ++i) {
            const BasicPhysicalPoint2D<T> point =
                mapping.map(first_face(i), second_center(j));

            require<errors::core::InvalidArgument>(
                physical_point_is_finite(point),
                id()
            );

            physical_first_face_centers_[j * num_faces_x() + i] = point;
        }
    }

    physical_second_face_centers_.resize(num_cells_x() * num_faces_y());

    for (Size j = 0; j < num_faces_y(); ++j) {
        for (Size i = 0; i < num_cells_x(); ++i) {
            const BasicPhysicalPoint2D<T> point =
                mapping.map(first_center(i), second_face(j));

            require<errors::core::InvalidArgument>(
                physical_point_is_finite(point),
                id()
            );

            physical_second_face_centers_[j * num_cells_x() + i] = point;
        }
    }

    cell_measures_.resize(num_cells());

    for (Size j = 0; j < num_cells_y(); ++j) {
        for (Size i = 0; i < num_cells_x(); ++i) {
            const T measure = static_cast<T>(
                mapping.cell_measure(
                    BasicCoordinateCell2D<T>{
                        first_face(i),
                        first_face(i + static_cast<Size>(1)),
                        second_face(j),
                        second_face(j + static_cast<Size>(1))
                    }
                )
            );

            require<errors::core::InvalidArgument>(
                std::isfinite(measure) && measure > T{},
                id()
            );

            cell_measures_[linear_cell_index(i, j)] = measure;
        }
    }

    build_face_measures(mapping);
}

template <std::floating_point T>
template <class Mapping>
    requires CoordinateMapping2DFor<Mapping, T>
void BasicStructuredGrid2D<T>::build_face_measures(const Mapping& mapping) {
    first_face_measures_.resize(num_faces_x() * num_cells_y());

    for (Size j = 0; j < num_cells_y(); ++j) {
        for (Size i = 0; i < num_faces_x(); ++i) {
            const T value = coordinate_metrics::first_face_measure(
                mapping,
                first_face(i),
                second_face(j),
                second_face(j + static_cast<Size>(1))
            );

            require<errors::core::InvalidArgument>(
                std::isfinite(value) && value >= T{},
                id()
            );

            first_face_measures_[j * num_faces_x() + i] = value;
        }
    }

    second_face_measures_.resize(num_cells_x() * num_faces_y());

    for (Size j = 0; j < num_faces_y(); ++j) {
        for (Size i = 0; i < num_cells_x(); ++i) {
            const T value = coordinate_metrics::second_face_measure(
                mapping,
                second_face(j),
                first_face(i),
                first_face(i + static_cast<Size>(1))
            );

            require<errors::core::InvalidArgument>(
                std::isfinite(value) && value >= T{},
                id()
            );

            second_face_measures_[j * num_cells_x() + i] = value;
        }
    }
}

template <std::floating_point T>
std::ostream& operator<<(
    std::ostream& output,
    const BasicStructuredGrid2D<T>& grid
) {
    output << "StructuredGrid2D\n"
           << "  coordinate system: " << grid.coordinate_system_name() << "\n"
           << "  logical coordinates: " << grid.first_coordinate_name()
           << ", " << grid.second_coordinate_name() << "\n"
           << "  dimensions: " << grid.num_cells_x() << " x "
           << grid.num_cells_y() << " = " << grid.num_cells() << " cells\n"
           << "  axis pattern: " << grid.first_axis().pattern_name() << "\n"
           << "  first independent Axis1D:\n"
           << grid.first_axis() << "\n"
           << "  second independent Axis1D:\n"
           << grid.second_axis();

    return output;
}

}  // namespace fvgrid
