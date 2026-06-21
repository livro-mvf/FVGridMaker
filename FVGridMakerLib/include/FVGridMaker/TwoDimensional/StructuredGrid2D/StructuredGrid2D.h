// ----------------------------------------------------------------------------
// File: StructuredGrid2D.h
// Project: FVGridMaker
// Description: Two-dimensional grid composed from two independent Axis1D objects.
// License: MIT
// ----------------------------------------------------------------------------
#pragma once

#include <cmath>
#include <iosfwd>
#include <string>
#include <string_view>
#include <span>
#include <utility>
#include <vector>

#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/ErrorHandling/ErrorCatalog.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/TwoDimensional/CoordinateSystem2D/CoordinateSystem2D.h>
#include <FVGridMaker/TwoDimensional/CoordinateSystem2D/CoordinateMetrics2D.h>

namespace fvgrid {

class StructuredGrid2D final {
public:
    StructuredGrid2D(Axis1D first_axis, Axis1D second_axis);

    template <CoordinateMapping2D Mapping>
    StructuredGrid2D(
        Axis1D first_axis,
        Axis1D second_axis,
        const Mapping& mapping
    )
        : first_axis_(std::move(first_axis)),
          second_axis_(std::move(second_axis)),
          coordinate_system_name_(mapping.name()),
          first_coordinate_name_(mapping.first_coordinate_name()),
          second_coordinate_name_(mapping.second_coordinate_name()),
          vtk_rectilinear_(mapping.vtk_rectilinear()) {
        build_derived_geometry(mapping);
    }

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{"TwoDimensional", "StructuredGrid2D",
                  "fvgrid.2d.grid.StructuredGrid2D"};
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }
    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] std::string_view coordinate_system_name() const noexcept;
    [[nodiscard]] std::string_view first_coordinate_name() const noexcept;
    [[nodiscard]] std::string_view second_coordinate_name() const noexcept;
    [[nodiscard]] bool vtk_rectilinear() const noexcept;

    [[nodiscard]] const Axis1D& first_axis() const noexcept;
    [[nodiscard]] const Axis1D& second_axis() const noexcept;
    [[nodiscard]] const Axis1D& x_axis() const noexcept;
    [[nodiscard]] const Axis1D& y_axis() const noexcept;

    [[nodiscard]] Size num_cells_x() const noexcept;
    [[nodiscard]] Size num_cells_y() const noexcept;
    [[nodiscard]] Size num_cells() const noexcept;
    [[nodiscard]] Size num_faces_x() const noexcept;
    [[nodiscard]] Size num_faces_y() const noexcept;

    [[nodiscard]] Real xmin() const noexcept;
    [[nodiscard]] Real xmax() const noexcept;
    [[nodiscard]] Real ymin() const noexcept;
    [[nodiscard]] Real ymax() const noexcept;
    [[nodiscard]] Real length_x() const noexcept;
    [[nodiscard]] Real length_y() const noexcept;

    [[nodiscard]] Size linear_cell_index(Size i, Size j) const;
    [[nodiscard]] Real x_face(Size i) const;
    [[nodiscard]] Real y_face(Size j) const;
    [[nodiscard]] Real x_center(Size i) const;
    [[nodiscard]] Real y_center(Size j) const;
    [[nodiscard]] Real x_cell_length(Size i) const;
    [[nodiscard]] Real y_cell_length(Size j) const;

    // Computational area in the two independent coordinates.
    [[nodiscard]] Real cell_area(Size i, Size j) const;
    // Physical area/volume supplied by the coordinate trait.
    [[nodiscard]] Real cell_measure(Size i, Size j) const;
    [[nodiscard]] Real first_face_measure(Size i, Size j) const;
    [[nodiscard]] Real second_face_measure(Size i, Size j) const;
    [[nodiscard]] std::span<const Real> cell_measures() const noexcept;
    [[nodiscard]] std::span<const Real> first_face_measures() const noexcept;
    [[nodiscard]] std::span<const Real> second_face_measures() const noexcept;
    [[nodiscard]] Real vertical_face_length(Size j) const;
    [[nodiscard]] Real horizontal_face_length(Size i) const;
    [[nodiscard]] PhysicalPoint2D physical_face_point(Size i, Size j) const;

private:
    Axis1D first_axis_;
    Axis1D second_axis_;
    std::string coordinate_system_name_;
    std::string first_coordinate_name_;
    std::string second_coordinate_name_;
    bool vtk_rectilinear_{};
    std::vector<PhysicalPoint2D> physical_face_points_;
    std::vector<Real> cell_measures_;
    std::vector<Real> first_face_measures_;
    std::vector<Real> second_face_measures_;

    template <CoordinateMapping2D Mapping>
    void build_derived_geometry(const Mapping& mapping) {
        physical_face_points_.resize(num_faces_x() * num_faces_y());
        for (Size j = 0; j < num_faces_y(); ++j) {
            for (Size i = 0; i < num_faces_x(); ++i) {
                physical_face_points_[j * num_faces_x() + i] =
                    mapping.map(x_face(i), y_face(j));
            }
        }

        cell_measures_.resize(num_cells());
        for (Size j = 0; j < num_cells_y(); ++j) {
            for (Size i = 0; i < num_cells_x(); ++i) {
                const Real measure = static_cast<Real>(mapping.cell_measure(
                    CoordinateCell2D{x_face(i), x_face(i + 1),
                                     y_face(j), y_face(j + 1)}
                ));
                require(
                    std::isfinite(measure) && measure > Real{},
                    error_catalog::kInvalidArgument,
                    id()
                );
                cell_measures_[j * num_cells_x() + i] = measure;
            }
        }
        build_face_measures(mapping);
    }

    template <CoordinateMapping2D Mapping>
    void build_face_measures(const Mapping& mapping) {
        first_face_measures_.resize(num_faces_x()*num_cells_y());
        for (Size j=0; j<num_cells_y(); ++j)
            for (Size i=0; i<num_faces_x(); ++i) {
                const Real value=coordinate_metrics::first_face_measure(
                    mapping,x_face(i),y_face(j),y_face(j+1));
                require(std::isfinite(value) && value>=Real{},
                        error_catalog::kInvalidArgument,id());
                first_face_measures_[j*num_faces_x()+i]=value;
            }
        second_face_measures_.resize(num_cells_x()*num_faces_y());
        for (Size j=0; j<num_faces_y(); ++j)
            for (Size i=0; i<num_cells_x(); ++i) {
                const Real value=coordinate_metrics::second_face_measure(
                    mapping,y_face(j),x_face(i),x_face(i+1));
                require(std::isfinite(value) && value>=Real{},
                        error_catalog::kInvalidArgument,id());
                second_face_measures_[j*num_cells_x()+i]=value;
            }
    }

    void validate_x_cell_index(Size i) const;
    void validate_y_cell_index(Size j) const;
    void validate_x_face_index(Size i) const;
    void validate_y_face_index(Size j) const;
    void validate_cell_index(Size i, Size j) const;
};

std::ostream& operator<<(std::ostream& output, const StructuredGrid2D& grid);

}  // namespace fvgrid
