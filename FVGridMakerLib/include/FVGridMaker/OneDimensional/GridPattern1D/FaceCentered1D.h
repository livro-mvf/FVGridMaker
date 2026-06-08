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
#include <string_view>
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/AxisGeometry1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/CoordinateKind1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Domain1D.h>

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

    [[nodiscard]] static constexpr CoordinateKind1D input_kind() noexcept {
        return CoordinateKind1D::Centers;
    }

    [[nodiscard]] static std::vector<Real> faces_from_centers(
        std::span<const Real> centers,
        Real x_min,
        Real x_max
    );

    [[nodiscard]] static AxisGeometry1D complete_geometry(
        std::vector<Real> centers,
        Domain1D domain
    );
};

}  // namespace fvgrid