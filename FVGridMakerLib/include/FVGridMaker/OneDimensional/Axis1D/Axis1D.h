// ----------------------------------------------------------------------------
// File: Axis1D.h
// Project: FVGridMaker
// Version: 0.1.0
// Description: Defines a one-dimensional structured finite-volume axis.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <iosfwd>
#include <span>
#include <string>
#include <string_view>
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/Types.h>

namespace fvgrid {

class Axis1D final {
public:
    Axis1D() = default;

    explicit Axis1D(std::vector<Real> faces);

    Axis1D(std::vector<Real> faces, std::string_view pattern_name);

    Axis1D(
        std::vector<Real> faces,
        std::vector<Real> centers,
        std::string_view pattern_name
    );

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "Axis1D",
            "fvgrid.1d.axis.Axis1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] Size num_cells() const noexcept;
    [[nodiscard]] Size num_faces() const noexcept;

    [[nodiscard]] std::span<const Real> faces() const noexcept;
    [[nodiscard]] std::span<const Real> centers() const noexcept;

    [[nodiscard]] std::span<const Real> dx_faces() const noexcept;
    [[nodiscard]] std::span<const Real> dx_centers() const noexcept;

    [[nodiscard]] std::span<const Real> cell_lengths() const noexcept;

    [[nodiscard]] Real xmin() const noexcept;
    [[nodiscard]] Real xmax() const noexcept;
    [[nodiscard]] Real length() const noexcept;

    [[nodiscard]] std::string_view pattern_name() const noexcept;

private:
    std::vector<Real> faces_;
    std::vector<Real> centers_;
    std::vector<Real> dx_faces_;
    std::vector<Real> dx_centers_;

    std::string pattern_name_{"VolumeCentered1D"};

    void validate_geometry() const;
    void rebuild_metrics();
};

std::ostream& operator<<(std::ostream& os, const Axis1D& axis);

}  // namespace fvgrid