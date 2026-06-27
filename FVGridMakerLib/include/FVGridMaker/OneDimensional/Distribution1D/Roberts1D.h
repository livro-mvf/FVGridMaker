// ----------------------------------------------------------------------------
// File: Roberts1D.h
// Project: FVGridMaker
// Version: 0.1.0
// Description: Declares Roberts one-dimensional axis generation utilities.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

#include <cmath>
#include <concepts>
#include <limits>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/CoordinateTags1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Domain1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/GridPatternConcept1D.h>

namespace fvgrid {

template <std::floating_point T>
class BasicRoberts1D final {
public:
    using value_type = T;

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

    [[nodiscard]] static BasicAxis1D<T> make(
        NVol nvol,
        BasicLength<T> length,
        BasicXInit<T> xinit,
        BasicBeta<T> beta
    );

    template <class Pattern>
        requires GridPattern1DFor<std::remove_cvref_t<Pattern>, T>
    [[nodiscard]] static BasicAxis1D<T> make(
        NVol nvol,
        BasicLength<T> length,
        BasicXInit<T> xinit,
        BasicBeta<T> beta,
        Pattern&& pattern
    ) {
        using PatternType = std::remove_cvref_t<Pattern>;

        validate_input(nvol, length, xinit, beta);

        const BasicDomain1D<T> domain = BasicDomain1D<T>::from_length(
            xinit,
            length
        );

        auto map = [xinit, length, beta](T eta) {
            return xinit.value() +
                length.value() * normalized_coordinate(eta, beta.value());
        };

        std::vector<T> primary_coordinates =
            build_primary_coordinates<PatternType>(nvol.value(), map);

        const auto& pattern_ref = pattern;

        BasicAxisGeometry1D<T> geometry = pattern_ref.complete_geometry(
            std::move(primary_coordinates),
            domain
        );

        return BasicAxis1D<T>::from_geometry(std::move(geometry));
    }

private:
    [[nodiscard]] static constexpr T beta_margin() noexcept {
        return static_cast<T>(100) * std::numeric_limits<T>::epsilon();
    }

    static void validate_input(
        NVol nvol,
        BasicLength<T> length,
        BasicXInit<T> xinit,
        BasicBeta<T> beta
    );

    [[nodiscard]] static T normalized_coordinate(T eta, T beta);

    template <class PatternType, class CoordinateMap>
    [[nodiscard]] static std::vector<T> build_primary_coordinates(
        Size cell_count,
        CoordinateMap&& map
    ) {
        using CoordinateTag = typename PatternType::primary_coordinates;

        if constexpr (std::same_as<CoordinateTag, FaceCoordinates1D>) {
            std::vector<T> faces(cell_count + static_cast<Size>(1));
            const T deta = T{1} / static_cast<T>(cell_count);

            for (Size i = 0; i <= cell_count; ++i) {
                faces[i] = map(static_cast<T>(i) * deta);
            }

            return faces;
        } else {
            std::vector<T> centers(cell_count);
            const T deta = T{1} / static_cast<T>(cell_count);

            for (Size i = 0; i < cell_count; ++i) {
                centers[i] = map((static_cast<T>(i) + T{0.5}) * deta);
            }

            return centers;
        }
    }
};

using Roberts1D = BasicRoberts1D<double>;
using Roberts1DFloat = BasicRoberts1D<float>;
using Roberts1DLongDouble = BasicRoberts1D<long double>;

template <std::floating_point T>
[[nodiscard]] BasicAxis1D<T> roberts_axis_1d(
    NVol nvol,
    BasicLength<T> length,
    BasicXInit<T> xinit,
    BasicBeta<T> beta
);

[[nodiscard]] Axis1D roberts_axis_1d(
    NVol nvol,
    Length length,
    XInit xinit,
    Beta beta
);

template <class Pattern>
    requires GridPattern1DFor<std::remove_cvref_t<Pattern>, double>
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

template <std::floating_point T>
[[nodiscard]] BasicAxis1D<T> roberts_axis_1d(
    Size nvol,
    T xinit,
    T xfinal,
    T beta
);

[[nodiscard]] Axis1D roberts_axis_1d(
    Size nvol,
    double xinit,
    double xfinal,
    double beta
);

}  // namespace fvgrid

#include <FVGridMaker/OneDimensional/Distribution1D/Roberts1D.tpp>
