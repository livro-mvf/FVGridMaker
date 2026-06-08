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
#include <span>
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/Types.h>

namespace fvgrid {

class Axis1D final {
public:
    Axis1D() = default;

    explicit Axis1D(std::vector<Real> faces);

    [[nodiscard]] Size num_cells() const noexcept;
    [[nodiscard]] Size num_faces() const noexcept;

    [[nodiscard]] std::span<const Real> faces() const noexcept;
    [[nodiscard]] std::span<const Real> centers() const noexcept;
    [[nodiscard]] std::span<const Real> cell_lengths() const noexcept;

    [[nodiscard]] Real xmin() const noexcept;
    [[nodiscard]] Real xmax() const noexcept;
    [[nodiscard]] Real length() const noexcept;

private:
    std::vector<Real> faces_;
    std::vector<Real> centers_;
    std::vector<Real> cell_lengths_;

    void validate_faces() const;
    void rebuild_derived_data();
};

}  // namespace fvgrid