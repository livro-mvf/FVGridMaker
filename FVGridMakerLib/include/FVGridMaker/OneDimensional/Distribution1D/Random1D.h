// ----------------------------------------------------------------------------
// File: Random1D.h
// Project: FVGridMaker
// Version: 0.1.0
// Description: Declares random one-dimensional axis generation utilities.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <random>
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
#include <FVGridMaker/OneDimensional/GridPattern1D/CoordinateKind1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Domain1D.h>

namespace fvgrid {

class Random1D final {
public:
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "Random1D",
            "fvgrid.1d.distribution.Random1D"
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
        Seed seed
    );

    [[nodiscard]] static Axis1D make(
        NVol nvol,
        Length length,
        XInit xinit,
        Seed seed,
        MinSpacing min_spacing
    );

    template <class Pattern>
    [[nodiscard]] static Axis1D make(
        NVol nvol,
        Length length,
        XInit xinit,
        Seed seed,
        Pattern&&
    ) {
        return make(
            nvol,
            length,
            xinit,
            seed,
            MinSpacing{static_cast<Real>(0.0)},
            std::remove_cvref_t<Pattern>{}
        );
    }

    template <class Pattern>
    [[nodiscard]] static Axis1D make(
        NVol nvol,
        Length length,
        XInit xinit,
        Seed seed,
        MinSpacing min_spacing,
        Pattern&&
    ) {
        using PatternType = std::remove_cvref_t<Pattern>;

        validate_input(
            nvol,
            length,
            min_spacing,
            PatternType::input_kind()
        );

        const Domain1D domain = Domain1D::from_length(
            xinit,
            length
        );

        std::mt19937_64 random_engine{seed.value()};

        std::vector<Real> primary_coordinates =
            build_primary_coordinates<PatternType>(
                nvol,
                length,
                xinit,
                min_spacing,
                random_engine
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
        MinSpacing min_spacing,
        CoordinateKind1D input_kind
    );

    [[nodiscard]] static std::vector<Real> build_random_partition(
        Size interval_count,
        Real total_length,
        Real min_spacing,
        std::mt19937_64& random_engine
    );

    template <class PatternType>
    [[nodiscard]] static std::vector<Real> build_primary_coordinates(
        NVol nvol,
        Length length,
        XInit xinit,
        MinSpacing min_spacing,
        std::mt19937_64& random_engine
    ) {
        const Size cell_count = nvol.value();
        const Real x0 = xinit.value();

        if constexpr (PatternType::input_kind() == CoordinateKind1D::Faces) {
            const std::vector<Real> widths = build_random_partition(
                cell_count,
                length.value(),
                min_spacing.value(),
                random_engine
            );

            std::vector<Real> faces(cell_count + 1);
            faces[0] = x0;

            for (Size i = 0; i < cell_count; ++i) {
                faces[i + 1] = faces[i] + widths[i];
            }

            faces[cell_count] = x0 + length.value();

            return faces;
        } else {
            const Size gap_count = cell_count + 1;

            const std::vector<Real> gaps = build_random_partition(
                gap_count,
                length.value(),
                min_spacing.value(),
                random_engine
            );

            std::vector<Real> centers(cell_count);
            Real coordinate = x0 + gaps[0];
            centers[0] = coordinate;

            for (Size i = 1; i < cell_count; ++i) {
                coordinate += gaps[i];
                centers[i] = coordinate;
            }

            return centers;
        }
    }
};

[[nodiscard]] Axis1D random_axis_1d(
    NVol nvol,
    Length length,
    XInit xinit,
    Seed seed
);

[[nodiscard]] Axis1D random_axis_1d(
    NVol nvol,
    Length length,
    XInit xinit,
    Seed seed,
    MinSpacing min_spacing
);

template <class Pattern>
[[nodiscard]] Axis1D random_axis_1d(
    NVol nvol,
    Length length,
    XInit xinit,
    Seed seed,
    Pattern&& pattern
) {
    return Random1D::make(
        nvol,
        length,
        xinit,
        seed,
        std::forward<Pattern>(pattern)
    );
}

template <class Pattern>
[[nodiscard]] Axis1D random_axis_1d(
    NVol nvol,
    Length length,
    XInit xinit,
    Seed seed,
    MinSpacing min_spacing,
    Pattern&& pattern
) {
    return Random1D::make(
        nvol,
        length,
        xinit,
        seed,
        min_spacing,
        std::forward<Pattern>(pattern)
    );
}

}  // namespace fvgrid