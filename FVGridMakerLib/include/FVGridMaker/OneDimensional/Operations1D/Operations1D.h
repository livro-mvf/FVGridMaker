// ----------------------------------------------------------------------------
// File: Operations1D.h
// Project: FVGridMaker
// Version: 0.1.0
// Description: Declares interval and coordinate operations for 1D axes.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

#include <concepts>
#include <span>
#include <string_view>
#include <vector>

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

    template <std::floating_point T>
    static void validate_tolerance(T tolerance);

    template <std::floating_point T>
    [[nodiscard]] static bool same_pattern(
        const BasicAxis1D<T>& left,
        const BasicAxis1D<T>& right
    ) noexcept;

    template <std::floating_point T>
    static void require_same_pattern(
        const BasicAxis1D<T>& left,
        const BasicAxis1D<T>& right
    );

    template <std::floating_point T>
    [[nodiscard]] static BasicAxisInterval1D<T> domain_interval(
        const BasicAxis1D<T>& axis
    ) noexcept;

    template <std::floating_point T>
    [[nodiscard]] static BasicAxisInterval1D<T> intersection(
        const BasicAxis1D<T>& left,
        const BasicAxis1D<T>& right,
        T tolerance = T{}
    );

    template <std::floating_point T>
    [[nodiscard]] static BasicAxisInterval1D<T> require_interval_intersection(
        const BasicAxis1D<T>& left,
        const BasicAxis1D<T>& right,
        T tolerance = T{}
    );

    template <std::floating_point T>
    [[nodiscard]] static std::vector<T> unique_sorted_coordinates(
        std::span<const T> coordinates,
        T tolerance = T{}
    );

    template <std::floating_point T>
    [[nodiscard]] static BasicAxis1D<T> clip_faces_to_interval(
        const BasicAxis1D<T>& axis,
        BasicAxisInterval1D<T> interval,
        T tolerance = T{}
    );
};

}  // namespace fvgrid

#include <FVGridMaker/OneDimensional/Operations1D/Operations1D.tpp>
