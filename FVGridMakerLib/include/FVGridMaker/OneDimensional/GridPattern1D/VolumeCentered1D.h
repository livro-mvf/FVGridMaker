// ----------------------------------------------------------------------------
// File: VolumeCentered1D.h
// Project: FVGridMaker
// Version: 0.1.0
// Description: Defines the volume-centred one-dimensional grid pattern.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <span>
// #include <string_view>
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/Types.h>

namespace fvgrid {

struct VolumeCentered1D final {
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "VolumeCentered1D",
            "fvgrid.1d.grid_pattern.VolumeCentered1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view name() noexcept {
        return "VolumeCentered1D";
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] static constexpr std::string_view primary_coordinates()
        noexcept {
        return "faces";
    }

    [[nodiscard]] static constexpr std::string_view secondary_coordinates()
        noexcept {
        return "centers";
    }

    [[nodiscard]] static std::vector<Real> centers_from_faces(
        std::span<const Real> faces
    );
};

}  // namespace fvgrid