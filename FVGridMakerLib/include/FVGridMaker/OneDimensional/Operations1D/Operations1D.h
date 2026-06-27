// ----------------------------------------------------------------------------
// File: Operations1D.h
// Project: FVGridMaker
// Version: 0.1.0
// Description: Declares interval and coordinate operations for 1D axes.
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
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/OneDimensional/Operations1D/AxisInterval1D.h>

namespace fvgrid {

class Operations1D final {
public:
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "Operations1D",
            "fvgrid.1d.operations.Operations1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    static void validate_tolerance(Real tolerance);

    [[nodiscard]] static bool same_pattern(
        const Axis1D& left,
        const Axis1D& right
    ) noexcept;

    static void require_same_pattern(
        const Axis1D& left,
        const Axis1D& right
    );

    [[nodiscard]] static AxisInterval1D domain_interval(
        const Axis1D& axis
    ) noexcept;

    [[nodiscard]] static AxisInterval1D intersection(
        const Axis1D& left,
        const Axis1D& right,
        Real tolerance = static_cast<Real>(0.0)
    );

    [[nodiscard]] static AxisInterval1D require_interval_intersection(
        const Axis1D& left,
        const Axis1D& right,
        Real tolerance = static_cast<Real>(0.0)
    );

    [[nodiscard]] static std::vector<Real> unique_sorted_coordinates(
        std::span<const Real> coordinates,
        Real tolerance = static_cast<Real>(0.0)
    );

    [[nodiscard]] static Axis1D clip_faces_to_interval(
        const Axis1D& axis,
        AxisInterval1D interval,
        Real tolerance = static_cast<Real>(0.0)
    );
};

}  // namespace fvgrid