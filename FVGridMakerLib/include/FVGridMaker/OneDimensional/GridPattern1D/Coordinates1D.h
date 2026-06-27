// ----------------------------------------------------------------------------
// File: Coordinates1D.h
// Project: FVGridMaker
// Version: 0.1.0
// Description: Defines typed one-dimensional coordinate input data.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <concepts>
#include <span>
#include <utility>
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/CoordinateTags1D.h>

namespace fvgrid {

class Coordinates1D final {
public:
    [[nodiscard]] static Coordinates1D faces(std::vector<Real> values) {
        return Coordinates1D{
            std::move(values),
            true
        };
    }

    [[nodiscard]] static Coordinates1D centers(std::vector<Real> values) {
        return Coordinates1D{
            std::move(values),
            false
        };
    }

    template <PrimaryCoordinateTag1D Tag>
    [[nodiscard]] bool has_tag() const noexcept {
        if constexpr (std::same_as<Tag, FaceCoordinates1D>) {
            return is_face_coordinates_;
        } else {
            return !is_face_coordinates_;
        }
    }

    [[nodiscard]] std::span<const Real> values() const noexcept {
        return values_;
    }

    [[nodiscard]] std::vector<Real> release_values() && noexcept {
        return std::move(values_);
    }

private:
    Coordinates1D(
        std::vector<Real> values,
        bool is_face_coordinates
    ) noexcept
        : values_(std::move(values)),
          is_face_coordinates_(is_face_coordinates) {}

    std::vector<Real> values_;
    bool is_face_coordinates_;
};

}  // namespace fvgrid