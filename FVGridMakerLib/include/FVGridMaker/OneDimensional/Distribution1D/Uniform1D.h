// ----------------------------------------------------------------------------
// File: Uniform1D.h
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Declares canonical uniform one-dimensional axis generation.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

#include <concepts>
#include <string_view>

#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>

namespace fvgrid {

template <std::floating_point T>
class BasicUniform1D final {
public:
    using value_type = T;

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "Uniform1D",
            "fvgrid.1d.distribution.Uniform1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] static BasicAxis1D<T> make(
        NVol nvol,
        BasicLength<T> length,
        BasicXInit<T> xinit
    );

private:
    static void validate_input(
        NVol nvol,
        BasicLength<T> length,
        BasicXInit<T> xinit
    );
};

using Uniform1D = BasicUniform1D<double>;
using Uniform1DFloat = BasicUniform1D<float>;
using Uniform1DLongDouble = BasicUniform1D<long double>;

template <std::floating_point T>
[[nodiscard]] BasicAxis1D<T> uniform_axis_1d(
    NVol nvol,
    BasicLength<T> length,
    BasicXInit<T> xinit
);

[[nodiscard]] Axis1D uniform_axis_1d(
    NVol nvol,
    Length length,
    XInit xinit
);

template <std::floating_point T>
[[nodiscard]] BasicAxis1D<T> uniform_axis_1d(
    Size nvol,
    T xinit,
    T xfinal
);

[[nodiscard]] Axis1D uniform_axis_1d(
    Size nvol,
    double xinit,
    double xfinal
);

}  // namespace fvgrid

#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.tpp>
