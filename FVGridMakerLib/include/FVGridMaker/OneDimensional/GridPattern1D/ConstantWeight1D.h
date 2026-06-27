// ----------------------------------------------------------------------------
// File: ConstantWeight1D.h
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Defines a constant interpolation weight for 1D grid patterns.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

#include <cmath>
#include <concepts>
#include <string_view>

#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/ErrorHandling/BuiltInErrors.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>

namespace fvgrid {

template <std::floating_point T>
class BasicConstantWeight1D final {
public:
    using value_type = T;

    explicit BasicConstantWeight1D(T value)
        : value_{value} {
        validate(value_);
    }

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "ConstantWeight1D",
            "fvgrid.1d.grid_pattern.ConstantWeight1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] T value() const noexcept {
        return value_;
    }

    [[nodiscard]] T operator()(Size) const noexcept {
        return value_;
    }

private:
    static void validate(T value) {
        require<errors::core::InvalidArgument>(
            std::isfinite(value) && value > T{0} && value < T{1},
            BasicConstantWeight1D::id()
        );
    }

    T value_;
};

template <std::floating_point T>
BasicConstantWeight1D(T) -> BasicConstantWeight1D<T>;

using ConstantWeight1D = BasicConstantWeight1D<double>;
using ConstantWeight1DFloat = BasicConstantWeight1D<float>;
using ConstantWeight1DLongDouble = BasicConstantWeight1D<long double>;

}  // namespace fvgrid
