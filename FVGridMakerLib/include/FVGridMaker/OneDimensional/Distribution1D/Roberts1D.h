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
#include <concepts>
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
#include <FVGridMaker/OneDimensional/GridPattern1D/CoordinateTags1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Domain1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/GridPatternConcept1D.h>

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
        requires GridPattern1D<std::remove_cvref_t<Pattern>>
    [[nodiscard]] static Axis1D make(
        NVol nvol,
        Length length,
        XInit xinit,
        Beta beta,
        Pattern&& pattern
    ) {
        using PatternType = std::remove_cvref_t<Pattern>;

        validate_input(nvol, length, xinit, beta);

        const Domain1D domain = Domain1D::from_length(xinit, length);

        auto map = [xinit, length, beta](Real eta) {
            return xinit.value() +
                   length.value() * normalized_coordinate(eta, beta.value());
        };

        std::vector<Real> primary_coordinates =
            build_primary_coordinates<PatternType>(
                nvol.value(),
                map
            );

        const auto& pattern_ref = pattern;

        AxisGeometry1D geometry = pattern_ref.complete_geometry(
            std::move(primary_coordinates),
            domain
        );

        return Axis1D::from_geometry(std::move(geometry));
    }

private:
    [[nodiscard]] static constexpr Real beta_margin() noexcept {
        return static_cast<Real>(1.0e-12);
    }

    static void validate_input(
        NVol nvol,
        Length length,
        XInit xinit,
        Beta beta
    );

    [[nodiscard]] static Real normalized_coordinate(
        Real eta,
        Real beta
    );

    template <class PatternType, class CoordinateMap>
    [[nodiscard]] static std::vector<Real> build_primary_coordinates(
        Size cell_count,
        CoordinateMap&& map
    ) {
        using CoordinateTag = typename PatternType::primary_coordinates;

        if constexpr (std::same_as<CoordinateTag, FaceCoordinates1D>) {
            std::vector<Real> faces(cell_count + static_cast<Size>(1));
            const Real deta =
                static_cast<Real>(1.0) / static_cast<Real>(cell_count);

            for (Size i = 0; i <= cell_count; ++i) {
                faces[i] = map(static_cast<Real>(i) * deta);
            }

            return faces;
        } else {
            std::vector<Real> centers(cell_count);
            const Real deta =
                static_cast<Real>(1.0) / static_cast<Real>(cell_count);

            for (Size i = 0; i < cell_count; ++i) {
                centers[i] =
                    map((static_cast<Real>(i) + static_cast<Real>(0.5)) * deta);
            }

            return centers;
        }
    }
};

[[nodiscard]] Axis1D roberts_axis_1d(
    NVol nvol,
    Length length,
    XInit xinit,
    Beta beta
);

template <class Pattern>
    requires GridPattern1D<std::remove_cvref_t<Pattern>>
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