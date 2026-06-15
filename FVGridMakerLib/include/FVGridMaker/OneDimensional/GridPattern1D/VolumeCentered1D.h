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

    [[nodiscard]] static constexpr CoordinateKind1D input_kind() noexcept {
        return CoordinateKind1D::Faces;
    }

    [[nodiscard]] static std::vector<Real> centers_from_faces(
        std::span<const Real> faces
    );

    template <class CoordinateMap>
    [[nodiscard]] static std::vector<Real> primary_coordinates_from_map(
        Size cell_count,
        CoordinateMap&& map
    ) {
        std::vector<Real> faces(cell_count + 1);
        const Real deta = static_cast<Real>(1.0) / static_cast<Real>(cell_count);

        for (Size i = 0; i <= cell_count; ++i) {
            faces[i] = map(static_cast<Real>(i) * deta);
        }

        return faces;
    }

    [[nodiscard]] static AxisGeometry1D complete_geometry(
        std::vector<Real> faces,
        Domain1D domain
    );
};

}  // namespace fvgrid
