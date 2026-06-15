// ----------------------------------------------------------------------------
// File: Roberts1D.h
// Project: FVGridMaker
// Version: 0.1.0
// Description: Declares Roberts one-dimensional axis generation utilities.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <cmath>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/AxisGeometry1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Domain1D.h>

namespace fvgrid {

class Roberts1D final {
public:
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "Roberts1D",
            "fvgrid.1d.distribution.Roberts1D"
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
        XInit xinit,
        Beta beta
    );

    template <class Pattern>
    [[nodiscard]] static Axis1D make(
        NVol nvol,
        Length length,
        XInit xinit,
        Beta beta,
        Pattern&&
    ) {
        using PatternType = std::remove_cvref_t<Pattern>;

        validate_input(nvol, length, beta);

        const Domain1D domain = Domain1D::from_length(xinit, length);

        auto map = [xinit, length, beta](Real eta) {
            return xinit.value() +
                   length.value() * normalized_coordinate(eta, beta.value());
        };

        std::vector<Real> primary_coordinates =
            PatternType::primary_coordinates_from_map(
                nvol.value(),
                map
            );

        AxisGeometry1D geometry = PatternType::complete_geometry(
            std::move(primary_coordinates),
            domain
        );

        return Axis1D{
            std::move(geometry.faces),
            std::move(geometry.centers),
            geometry.pattern_name
        };
    }

private:
    static void validate_input(
        NVol nvol,
        Length length,
        Beta beta
    );

    [[nodiscard]] static Real normalized_coordinate(
        Real eta,
        Real beta
    );
};

[[nodiscard]] Axis1D roberts_axis_1d(
    NVol nvol,
    Length length,
    XInit xinit,
    Beta beta
);

template <class Pattern>
[[nodiscard]] Axis1D roberts_axis_1d(
    NVol nvol,
    Length length,
    XInit xinit,
    Beta beta,
    Pattern&& pattern
) {
    return Roberts1D::make(
        nvol,
        length,
        xinit,
        beta,
        std::forward<Pattern>(pattern)
    );
}

}  // namespace fvgrid
