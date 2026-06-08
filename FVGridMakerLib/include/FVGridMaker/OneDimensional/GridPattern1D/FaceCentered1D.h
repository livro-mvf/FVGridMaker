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
#include <span>
// #include <string_view>
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/Types.h>

namespace fvgrid {

struct FaceCentered1D final {
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "FaceCentered1D",
            "fvgrid.1d.grid_pattern.FaceCentered1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view name() noexcept {
        return "FaceCentered1D";
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] static constexpr std::string_view primary_coordinates()
        noexcept {
        return "centers";
    }

    [[nodiscard]] static constexpr std::string_view secondary_coordinates()
        noexcept {
        return "faces";
    }

    // Reconstructs face coordinates from centre coordinates.
    //
    // This is the defining rule for the face-centred construction path:
    //
    //   - the primary coordinates are the cell centres;
    //   - the first face is fixed at x_min;
    //   - the last face is fixed at x_max;
    //   - internal faces are midpoints between neighbouring centres.
    //
    // This rule is intentionally kept in FaceCentered1D instead of Axis1D,
    // because Axis1D should store a completed geometry and compute metrics,
    // not decide how a specific grid pattern reconstructs missing coordinates.
    [[nodiscard]] static std::vector<Real> faces_from_centers(
        std::span<const Real> centers,
        Real x_min,
        Real x_max
    );
};

}  // namespace fvgrid