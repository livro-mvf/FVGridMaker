// ----------------------------------------------------------------------------
// File: Uniform1D.h
// Project: FVGridMaker
// Version: 0.1.0
// Description: Declares uniform one-dimensional axis generation utilities.
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
#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>

namespace fvgrid {

class Uniform1D final {
public:
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

    [[nodiscard]] static Axis1D make(
        NVol nvol,
        Length length,
        XInit xinit
    );

    [[nodiscard]] static Axis1D make(
        NVol nvol,
        Length length,
        XInit xinit,
        std::string_view pattern_name
    );

    template <class Pattern>
    [[nodiscard]] static Axis1D make(
        NVol nvol,
        Length length,
        XInit xinit,
        Pattern
    ) {
        return make(nvol, length, xinit, Pattern::name());
    }
};

[[nodiscard]] Axis1D uniform_axis_1d(
    NVol nvol,
    Length length,
    XInit xinit
);

[[nodiscard]] Axis1D uniform_axis_1d(
    NVol nvol,
    Length length,
    XInit xinit,
    std::string_view pattern_name
);

template <class Pattern>
[[nodiscard]] Axis1D uniform_axis_1d(
    NVol nvol,
    Length length,
    XInit xinit,
    Pattern
) {
    return Uniform1D::make(nvol, length, xinit, Pattern{});
}

}  // namespace fvgrid