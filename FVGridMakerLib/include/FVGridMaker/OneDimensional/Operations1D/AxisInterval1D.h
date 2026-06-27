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
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <concepts>

namespace fvgrid {

template <std::floating_point T>
class BasicAxisInterval1D final {
public:
    using value_type = T;

    [[nodiscard]] static constexpr BasicAxisInterval1D empty() noexcept {
        return BasicAxisInterval1D{T{}, T{}, true};
    }

    [[nodiscard]] static constexpr BasicAxisInterval1D point(
        T coordinate
    ) noexcept {
        return BasicAxisInterval1D{coordinate, coordinate, false};
    }

    [[nodiscard]] static constexpr BasicAxisInterval1D from_bounds(
        T lower,
        T upper
    ) noexcept {
        if (upper < lower) {
            return BasicAxisInterval1D::empty();
        }

        return BasicAxisInterval1D{lower, upper, false};
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

    [[nodiscard]] constexpr T lower() const noexcept {
        return lower_;
    }

    [[nodiscard]] constexpr T upper() const noexcept {
        return upper_;
    }

    [[nodiscard]] constexpr T length() const noexcept {
        if (is_empty_) {
            return T{};
        }

        return upper_ - lower_;
    }

private:
    constexpr BasicAxisInterval1D(
        T lower,
        T upper,
        bool is_empty
    ) noexcept
        : lower_(lower),
          upper_(upper),
          is_empty_(is_empty) {}

    T lower_{};
    T upper_{};
    bool is_empty_{true};
};

using AxisInterval1D = BasicAxisInterval1D<double>;
using AxisInterval1DFloat = BasicAxisInterval1D<float>;
using AxisInterval1DLongDouble = BasicAxisInterval1D<long double>;

}  // namespace fvgrid
