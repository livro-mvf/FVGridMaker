// ----------------------------------------------------------------------------
// File: LogicalBox2D.h
// Project: FVGridMaker
// Description: Lightweight logical box result for two-dimensional operations.
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/OneDimensional/Operations1D/AxisInterval1D.h>

namespace fvgrid {

class LogicalBox2D final {
public:
    [[nodiscard]] static constexpr LogicalBox2D empty() noexcept {
        return LogicalBox2D{AxisInterval1D::empty(), AxisInterval1D::empty()};
    }

    [[nodiscard]] static constexpr LogicalBox2D from_intervals(
        AxisInterval1D first,
        AxisInterval1D second
    ) noexcept {
        return LogicalBox2D{first, second};
    }

    [[nodiscard]] constexpr AxisInterval1D first_interval() const noexcept {
        return first_;
    }

    [[nodiscard]] constexpr AxisInterval1D second_interval() const noexcept {
        return second_;
    }

    [[nodiscard]] constexpr bool is_empty() const noexcept {
        return first_.is_empty() || second_.is_empty();
    }

    [[nodiscard]] constexpr bool is_area() const noexcept {
        return first_.is_interval() && second_.is_interval();
    }

    [[nodiscard]] constexpr Real computational_area() const noexcept {
        if (!is_area()) {
            return Real{};
        }

        return first_.length() * second_.length();
    }

private:
    constexpr LogicalBox2D(
        AxisInterval1D first,
        AxisInterval1D second
    ) noexcept
        : first_(first),
          second_(second) {}

    AxisInterval1D first_ = AxisInterval1D::empty();
    AxisInterval1D second_ = AxisInterval1D::empty();
};

}  // namespace fvgrid
