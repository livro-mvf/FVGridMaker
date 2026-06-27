// ----------------------------------------------------------------------------
// File: LogicalBox2D.h
// Project: FVGridMaker
// Description: Lightweight logical box result for two-dimensional operations.
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

#include <concepts>

#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/OneDimensional/Operations1D/AxisInterval1D.h>

namespace fvgrid {

template <std::floating_point T>
class BasicLogicalBox2D final {
public:
    using value_type = T;

    [[nodiscard]] static constexpr BasicLogicalBox2D empty() noexcept {
        return BasicLogicalBox2D{
            BasicAxisInterval1D<T>::empty(),
            BasicAxisInterval1D<T>::empty()
        };
    }

    [[nodiscard]] static constexpr BasicLogicalBox2D from_intervals(
        BasicAxisInterval1D<T> first,
        BasicAxisInterval1D<T> second
    ) noexcept {
        return BasicLogicalBox2D{first, second};
    }

    [[nodiscard]] constexpr BasicAxisInterval1D<T> first_interval()
        const noexcept {
        return first_;
    }

    [[nodiscard]] constexpr BasicAxisInterval1D<T> second_interval()
        const noexcept {
        return second_;
    }

    [[nodiscard]] constexpr bool is_empty() const noexcept {
        return first_.is_empty() || second_.is_empty();
    }

    [[nodiscard]] constexpr bool is_area() const noexcept {
        return first_.is_interval() && second_.is_interval();
    }

    [[nodiscard]] constexpr T computational_area() const noexcept {
        if (!is_area()) {
            return T{};
        }

        return first_.length() * second_.length();
    }

private:
    constexpr BasicLogicalBox2D(
        BasicAxisInterval1D<T> first,
        BasicAxisInterval1D<T> second
    ) noexcept
        : first_(first),
          second_(second) {}

    BasicAxisInterval1D<T> first_ = BasicAxisInterval1D<T>::empty();
    BasicAxisInterval1D<T> second_ = BasicAxisInterval1D<T>::empty();
};

using LogicalBox2D = BasicLogicalBox2D<double>;
using LogicalBox2DFloat = BasicLogicalBox2D<float>;
using LogicalBox2DLongDouble = BasicLogicalBox2D<long double>;

}  // namespace fvgrid
