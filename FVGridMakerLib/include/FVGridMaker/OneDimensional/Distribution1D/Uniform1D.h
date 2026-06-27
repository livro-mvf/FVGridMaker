// ----------------------------------------------------------------------------
// File: Uniform1D.h
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Declares canonical uniform one-dimensional axis generation.
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

private:
    static void validate_input(
        NVol nvol,
        Length length,
        XInit xinit
    );
};

[[nodiscard]] Axis1D uniform_axis_1d(
    NVol nvol,
    Length length,
    XInit xinit
);

}  // namespace fvgrid