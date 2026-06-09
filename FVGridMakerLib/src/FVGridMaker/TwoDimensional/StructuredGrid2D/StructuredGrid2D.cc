// ----------------------------------------------------------------------------
// File: StructuredGrid2D.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Implements a two-dimensional rectilinear structured FV grid.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <utility>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/ErrorCatalog.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

namespace fvgrid {

StructuredGrid2D::StructuredGrid2D(
    Axis1D x_axis,
    Axis1D y_axis
)
    : x_axis_(std::move(x_axis)),
      y_axis_(std::move(y_axis)) {}

const Axis1D& StructuredGrid2D::x_axis() const noexcept {
    return x_axis_;
}

const Axis1D& StructuredGrid2D::y_axis() const noexcept {
    return y_axis_;
}

Size StructuredGrid2D::num_cells_x() const noexcept {
    return x_axis_.num_cells();
}

Size StructuredGrid2D::num_cells_y() const noexcept {
    return y_axis_.num_cells();
}

Size StructuredGrid2D::num_cells() const noexcept {
    return num_cells_x() * num_cells_y();
}

Size StructuredGrid2D::num_faces_x() const noexcept {
    return x_axis_.num_faces();
}

Size StructuredGrid2D::num_faces_y() const noexcept {
    return y_axis_.num_faces();
}

Real StructuredGrid2D::xmin() const noexcept {
    return x_axis_.xmin();
}

Real StructuredGrid2D::xmax() const noexcept {
    return x_axis_.xmax();
}

Real StructuredGrid2D::ymin() const noexcept {
    return y_axis_.xmin();
}

Real StructuredGrid2D::ymax() const noexcept {
    return y_axis_.xmax();
}

Real StructuredGrid2D::length_x() const noexcept {
    return x_axis_.length();
}

Real StructuredGrid2D::length_y() const noexcept {
    return y_axis_.length();
}

Size StructuredGrid2D::linear_cell_index(
    Size i,
    Size j
) const {
    validate_cell_index(i, j);

    return j * num_cells_x() + i;
}

Real StructuredGrid2D::x_face(
    Size i
) const {
    validate_x_face_index(i);

    return x_axis_.faces()[i];
}

Real StructuredGrid2D::y_face(
    Size j
) const {
    validate_y_face_index(j);

    return y_axis_.faces()[j];
}

Real StructuredGrid2D::x_center(
    Size i
) const {
    validate_x_cell_index(i);

    return x_axis_.centers()[i];
}

Real StructuredGrid2D::y_center(
    Size j
) const {
    validate_y_cell_index(j);

    return y_axis_.centers()[j];
}

Real StructuredGrid2D::x_cell_length(
    Size i
) const {
    validate_x_cell_index(i);

    return x_axis_.dx_faces()[i];
}

Real StructuredGrid2D::y_cell_length(
    Size j
) const {
    validate_y_cell_index(j);

    return y_axis_.dx_faces()[j];
}

Real StructuredGrid2D::cell_area(
    Size i,
    Size j
) const {
    validate_cell_index(i, j);

    return x_cell_length(i) * y_cell_length(j);
}

Real StructuredGrid2D::vertical_face_length(
    Size j
) const {
    validate_y_cell_index(j);

    return y_cell_length(j);
}

Real StructuredGrid2D::horizontal_face_length(
    Size i
) const {
    validate_x_cell_index(i);

    return x_cell_length(i);
}

void StructuredGrid2D::validate_x_cell_index(
    Size i
) const {
    require(
        i < num_cells_x(),
        error_catalog::kOutOfRange,
        StructuredGrid2D::id()
    );
}

void StructuredGrid2D::validate_y_cell_index(
    Size j
) const {
    require(
        j < num_cells_y(),
        error_catalog::kOutOfRange,
        StructuredGrid2D::id()
    );
}

void StructuredGrid2D::validate_x_face_index(
    Size i
) const {
    require(
        i < num_faces_x(),
        error_catalog::kOutOfRange,
        StructuredGrid2D::id()
    );
}

void StructuredGrid2D::validate_y_face_index(
    Size j
) const {
    require(
        j < num_faces_y(),
        error_catalog::kOutOfRange,
        StructuredGrid2D::id()
    );
}

void StructuredGrid2D::validate_cell_index(
    Size i,
    Size j
) const {
    validate_x_cell_index(i);
    validate_y_cell_index(j);
}

}  // namespace fvgrid