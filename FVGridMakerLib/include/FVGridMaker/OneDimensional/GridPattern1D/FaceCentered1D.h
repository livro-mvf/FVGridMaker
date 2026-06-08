// ----------------------------------------------------------------------------
// File: FaceCentered1D.h
// Project: FVGridMaker
// Version: 0.1.0
// Description: Defines the face-centred one-dimensional grid pattern.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <string_view>

namespace fvgrid {

struct FaceCentered1D final {
    [[nodiscard]] static constexpr std::string_view name() noexcept {
        return "FaceCentered1D";
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return "fvgrid.1d.grid_pattern.FaceCentered1D";
    }

    [[nodiscard]] static constexpr std::string_view primary_coordinates()
        noexcept {
        return "centers";
    }

    [[nodiscard]] static constexpr std::string_view secondary_coordinates()
        noexcept {
        return "faces";
    }
};

}  // namespace fvgrid