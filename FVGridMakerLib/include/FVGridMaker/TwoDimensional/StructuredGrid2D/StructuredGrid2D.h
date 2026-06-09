// ----------------------------------------------------------------------------
// File: StructuredGrid2D.h
// Project: FVGridMaker
// Version: 0.1.0
// Description: Defines a two-dimensional rectilinear structured FV grid.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <string_view>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>

namespace fvgrid {

class StructuredGrid2D final {
public:
    StructuredGrid2D(Axis1D x_axis, Axis1D y_axis);

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "TwoDimensional",
            "StructuredGrid2D",
            "fvgrid.2d.grid.StructuredGrid2D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

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

    [[nodiscard]] Real cell_area(Size i, Size j) const;

    [[nodiscard]] Real vertical_face_length(Size j) const;
    [[nodiscard]] Real horizontal_face_length(Size i) const;

private:
    Axis1D x_axis_;
    Axis1D y_axis_;

    void validate_x_cell_index(Size i) const;
    void validate_y_cell_index(Size j) const;

    void validate_x_face_index(Size i) const;
    void validate_y_face_index(Size j) const;

    void validate_cell_index(Size i, Size j) const;
};

}  // namespace fvgrid