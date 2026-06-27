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

template <std::floating_point T>
class BasicCoordinates1D final {
public:
    using value_type = T;

    [[nodiscard]] static BasicCoordinates1D faces(std::vector<T> values) {
        return BasicCoordinates1D{std::move(values), true};
    }

    [[nodiscard]] static BasicCoordinates1D centers(std::vector<T> values) {
        return BasicCoordinates1D{std::move(values), false};
    }

    template <PrimaryCoordinateTag1D Tag>
    [[nodiscard]] bool has_tag() const noexcept {
        if constexpr (std::same_as<Tag, FaceCoordinates1D>) {
            return is_face_coordinates_;
        } else {
            return !is_face_coordinates_;
        }
    }

    [[nodiscard]] std::span<const T> values() const noexcept {
        return values_;
    }

    [[nodiscard]] std::vector<T> release_values() && noexcept {
        return std::move(values_);
    }

private:
    BasicCoordinates1D(
        std::vector<T> values,
        bool is_face_coordinates
    ) noexcept
        : values_(std::move(values)),
          is_face_coordinates_(is_face_coordinates) {}

    std::vector<T> values_;
    bool is_face_coordinates_{};
};

using Coordinates1D = BasicCoordinates1D<double>;
using Coordinates1DFloat = BasicCoordinates1D<float>;
using Coordinates1DLongDouble = BasicCoordinates1D<long double>;

}  // namespace fvgrid
