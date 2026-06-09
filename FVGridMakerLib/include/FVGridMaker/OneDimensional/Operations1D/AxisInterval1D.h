// ----------------------------------------------------------------------------
// File: AxisInterval1D.h
// Project: FVGridMaker
// Version: 0.1.0
// Description: Defines a lightweight one-dimensional interval result.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/Types.h>

namespace fvgrid {

class AxisInterval1D final {
public:
    [[nodiscard]] static constexpr AxisInterval1D empty() noexcept {
        return AxisInterval1D{
            static_cast<Real>(0.0),
            static_cast<Real>(0.0),
            true
        };
    }

    [[nodiscard]] static constexpr AxisInterval1D point(
        Real coordinate
    ) noexcept {
        return AxisInterval1D{coordinate, coordinate, false};
    }

    [[nodiscard]] static constexpr AxisInterval1D from_bounds(
        Real lower,
        Real upper
    ) noexcept {
        if (upper < lower) {
            return AxisInterval1D::empty();
        }

        return AxisInterval1D{lower, upper, false};
    }

    [[nodiscard]] constexpr bool is_empty() const noexcept {
        return is_empty_;
    }

    [[nodiscard]] constexpr bool is_point() const noexcept {
        return !is_empty_ && lower_ == upper_;
    }

    [[nodiscard]] constexpr bool is_interval() const noexcept {
        return !is_empty_ && upper_ > lower_;
    }

    [[nodiscard]] constexpr Real lower() const noexcept {
        return lower_;
    }

    [[nodiscard]] constexpr Real upper() const noexcept {
        return upper_;
    }

    [[nodiscard]] constexpr Real length() const noexcept {
        if (is_empty_) {
            return static_cast<Real>(0.0);
        }

        return upper_ - lower_;
    }

private:
    constexpr AxisInterval1D(
        Real lower,
        Real upper,
        bool is_empty
    ) noexcept
        : lower_(lower),
          upper_(upper),
          is_empty_(is_empty) {}

    Real lower_ = static_cast<Real>(0.0);
    Real upper_ = static_cast<Real>(0.0);
    bool is_empty_ = true;
};

}  // namespace fvgrid