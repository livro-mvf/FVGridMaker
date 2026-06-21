// ----------------------------------------------------------------------------
// File: StructuredGrid2D.cc
// Project: FVGridMaker
// License: MIT
// ----------------------------------------------------------------------------
#include <ostream>
#include <utility>

#include <FVGridMaker/ErrorHandling/ErrorCatalog.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

namespace fvgrid {

StructuredGrid2D::StructuredGrid2D(Axis1D first_axis, Axis1D second_axis)
    : StructuredGrid2D(
          std::move(first_axis),
          std::move(second_axis),
          CartesianCoordinates2D{}
      ) {}

std::string_view StructuredGrid2D::coordinate_system_name() const noexcept {
    return coordinate_system_name_;
}
std::string_view StructuredGrid2D::first_coordinate_name() const noexcept {
    return first_coordinate_name_;
}
std::string_view StructuredGrid2D::second_coordinate_name() const noexcept {
    return second_coordinate_name_;
}
bool StructuredGrid2D::vtk_rectilinear() const noexcept {
    return vtk_rectilinear_;
}

const Axis1D& StructuredGrid2D::first_axis() const noexcept {
    return first_axis_;
}
const Axis1D& StructuredGrid2D::second_axis() const noexcept {
    return second_axis_;
}
const Axis1D& StructuredGrid2D::x_axis() const noexcept { return first_axis_; }
const Axis1D& StructuredGrid2D::y_axis() const noexcept { return second_axis_; }

Size StructuredGrid2D::num_cells_x() const noexcept {
    return first_axis_.num_cells();
}
Size StructuredGrid2D::num_cells_y() const noexcept {
    return second_axis_.num_cells();
}
Size StructuredGrid2D::num_cells() const noexcept {
    return num_cells_x() * num_cells_y();
}
Size StructuredGrid2D::num_faces_x() const noexcept {
    return first_axis_.num_faces();
}
Size StructuredGrid2D::num_faces_y() const noexcept {
    return second_axis_.num_faces();
}

Real StructuredGrid2D::xmin() const noexcept { return first_axis_.xmin(); }
Real StructuredGrid2D::xmax() const noexcept { return first_axis_.xmax(); }
Real StructuredGrid2D::ymin() const noexcept { return second_axis_.xmin(); }
Real StructuredGrid2D::ymax() const noexcept { return second_axis_.xmax(); }
Real StructuredGrid2D::length_x() const noexcept { return first_axis_.length(); }
Real StructuredGrid2D::length_y() const noexcept { return second_axis_.length(); }

Size StructuredGrid2D::linear_cell_index(Size i, Size j) const {
    validate_cell_index(i, j);
    return j * num_cells_x() + i;
}
Real StructuredGrid2D::x_face(Size i) const {
    validate_x_face_index(i);
    return first_axis_.faces()[i];
}
Real StructuredGrid2D::y_face(Size j) const {
    validate_y_face_index(j);
    return second_axis_.faces()[j];
}
Real StructuredGrid2D::x_center(Size i) const {
    validate_x_cell_index(i);
    return first_axis_.centers()[i];
}
Real StructuredGrid2D::y_center(Size j) const {
    validate_y_cell_index(j);
    return second_axis_.centers()[j];
}
Real StructuredGrid2D::x_cell_length(Size i) const {
    validate_x_cell_index(i);
    return first_axis_.dx_faces()[i];
}
Real StructuredGrid2D::y_cell_length(Size j) const {
    validate_y_cell_index(j);
    return second_axis_.dx_faces()[j];
}

Real StructuredGrid2D::cell_area(Size i, Size j) const {
    validate_cell_index(i, j);
    return x_cell_length(i) * y_cell_length(j);
}
Real StructuredGrid2D::cell_measure(Size i, Size j) const {
    validate_cell_index(i, j);
    return cell_measures_[linear_cell_index(i, j)];
}
Real StructuredGrid2D::first_face_measure(Size i, Size j) const {
    validate_x_face_index(i);
    validate_y_cell_index(j);
    return first_face_measures_[j*num_faces_x()+i];
}
Real StructuredGrid2D::second_face_measure(Size i, Size j) const {
    validate_x_cell_index(i);
    validate_y_face_index(j);
    return second_face_measures_[j*num_cells_x()+i];
}
std::span<const Real> StructuredGrid2D::cell_measures() const noexcept {
    return cell_measures_;
}
std::span<const Real> StructuredGrid2D::first_face_measures() const noexcept {
    return first_face_measures_;
}
std::span<const Real> StructuredGrid2D::second_face_measures() const noexcept {
    return second_face_measures_;
}
Real StructuredGrid2D::vertical_face_length(Size j) const {
    validate_y_cell_index(j);
    return y_cell_length(j);
}
Real StructuredGrid2D::horizontal_face_length(Size i) const {
    validate_x_cell_index(i);
    return x_cell_length(i);
}
PhysicalPoint2D StructuredGrid2D::physical_face_point(Size i, Size j) const {
    validate_x_face_index(i);
    validate_y_face_index(j);
    return physical_face_points_[j * num_faces_x() + i];
}

void StructuredGrid2D::validate_x_cell_index(Size i) const {
    require(i < num_cells_x(), error_catalog::kOutOfRange, id());
}
void StructuredGrid2D::validate_y_cell_index(Size j) const {
    require(j < num_cells_y(), error_catalog::kOutOfRange, id());
}
void StructuredGrid2D::validate_x_face_index(Size i) const {
    require(i < num_faces_x(), error_catalog::kOutOfRange, id());
}
void StructuredGrid2D::validate_y_face_index(Size j) const {
    require(j < num_faces_y(), error_catalog::kOutOfRange, id());
}
void StructuredGrid2D::validate_cell_index(Size i, Size j) const {
    validate_x_cell_index(i);
    validate_y_cell_index(j);
}

std::ostream& operator<<(std::ostream& output, const StructuredGrid2D& grid) {
    output << "StructuredGrid2D\n"
           << "  coordinate system: " << grid.coordinate_system_name() << '\n'
           << "  logical coordinates: " << grid.first_coordinate_name()
           << ", " << grid.second_coordinate_name() << '\n'
           << "  dimensions: " << grid.num_cells_x() << " x "
           << grid.num_cells_y() << " = " << grid.num_cells() << " cells\n"
           << "  first independent Axis1D:\n" << grid.first_axis() << '\n'
           << "  second independent Axis1D:\n" << grid.second_axis();
    return output;
}

}  // namespace fvgrid
