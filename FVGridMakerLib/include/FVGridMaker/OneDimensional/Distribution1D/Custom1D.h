// ----------------------------------------------------------------------------
// File: Custom1D.h
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Declares generic custom one-dimensional axis generation.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <concepts>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/ErrorHandling/BuiltInErrors.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/AxisGeometry1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/CoordinateKind1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Coordinates1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Domain1D.h>

namespace fvgrid {

template <class Pattern>
concept AxisCompletionPattern1D =
    requires(std::vector<Real> coordinates, Domain1D domain) {
        {
            std::remove_cvref_t<Pattern>::input_kind()
        } -> std::same_as<CoordinateKind1D>;

        {
            std::remove_cvref_t<Pattern>::complete_geometry(
                std::move(coordinates),
                domain
            )
        } -> std::same_as<AxisGeometry1D>;
    };

class Custom1D final {
public:
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "Custom1D",
            "fvgrid.1d.distribution.Custom1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    template <AxisCompletionPattern1D Pattern>
    [[nodiscard]] static Axis1D make(
        Coordinates1D coordinates,
        Pattern&&,
        Domain1D domain = Domain1D::none()
    ) {
        using PatternType = std::remove_cvref_t<Pattern>;

        require<errors::grid::InvalidCoordinateKind>(
            coordinates.kind() == PatternType::input_kind(),
            Custom1D::id()
        );

        AxisGeometry1D geometry = PatternType::complete_geometry(
            coordinates.release_values(),
            domain
        );

        return Axis1D{
            std::move(geometry.faces),
            std::move(geometry.centers),
            geometry.pattern_name
        };
    }
};

template <AxisCompletionPattern1D Pattern>
[[nodiscard]] Axis1D custom_axis_1d(
    Coordinates1D coordinates,
    Pattern&& pattern,
    Domain1D domain = Domain1D::none()
) {
    return Custom1D::make(
        std::move(coordinates),
        std::forward<Pattern>(pattern),
        domain
    );
}

}  // namespace fvgrid