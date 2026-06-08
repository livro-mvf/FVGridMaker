// ----------------------------------------------------------------------------
// File: Uniform1D.h
// Project: FVGridMaker
// Version: 0.1.0
// Description: Declares uniform one-dimensional axis generation utilities.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
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

class Uniform1D final {
public:
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "Uniform1D",
            "fvgrid.1d.distribution.Uniform1D"
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
        XInit xinit
    );

    template <class Pattern>
    [[nodiscard]] static Axis1D make(
        NVol nvol,
        Length length,
        XInit xinit,
        Pattern&&
    ) {
        using PatternType = std::remove_cvref_t<Pattern>;

        validate_input(nvol, length);

        const Domain1D domain = Domain1D::from_length(xinit, length);

        std::vector<Real> primary_coordinates =
            build_primary_coordinates<PatternType>(
                nvol,
                length,
                xinit
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
        Length length
    );

    template <class PatternType>
    [[nodiscard]] static std::vector<Real> build_primary_coordinates(
        NVol nvol,
        Length length,
        XInit xinit
    ) {
        const Size cell_count = nvol.value();
        const Real dx = length.value() / static_cast<Real>(cell_count);
        const Real x0 = xinit.value();

        if constexpr (PatternType::input_kind() == CoordinateKind1D::Faces) {
            std::vector<Real> faces(cell_count + 1);

            for (Size i = 0; i <= cell_count; ++i) {
                faces[i] = x0 + static_cast<Real>(i) * dx;
            }

            return faces;
        } else {
            std::vector<Real> centers(cell_count);

            for (Size i = 0; i < cell_count; ++i) {
                centers[i] =
                    x0 + (static_cast<Real>(i) + static_cast<Real>(0.5)) * dx;
            }

            return centers;
        }
    }
};

[[nodiscard]] Axis1D uniform_axis_1d(
    NVol nvol,
    Length length,
    XInit xinit
);

template <class Pattern>
[[nodiscard]] Axis1D uniform_axis_1d(
    NVol nvol,
    Length length,
    XInit xinit,
    Pattern&& pattern
) {
    return Uniform1D::make(
        nvol,
        length,
        xinit,
        std::forward<Pattern>(pattern)
    );
}

}  // namespace fvgrid