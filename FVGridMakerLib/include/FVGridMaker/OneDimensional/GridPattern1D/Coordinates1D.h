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
#include <span>
#include <utility>
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/CoordinateKind1D.h>

namespace fvgrid {

class Coordinates1D final {
public:
    [[nodiscard]] static Coordinates1D faces(std::vector<Real> values) {
        return Coordinates1D{std::move(values), CoordinateKind1D::Faces};
    }

    [[nodiscard]] static Coordinates1D centers(std::vector<Real> values) {
        return Coordinates1D{std::move(values), CoordinateKind1D::Centers};
    }

    [[nodiscard]] CoordinateKind1D kind() const noexcept {
        return kind_;
    }

    [[nodiscard]] std::span<const Real> values() const noexcept {
        return values_;
    }

    [[nodiscard]] std::vector<Real> release_values() noexcept {
        return std::move(values_);
    }

private:
    Coordinates1D(
        std::vector<Real> values,
        CoordinateKind1D kind
    ) noexcept
        : values_(std::move(values)),
          kind_(kind) {}

    std::vector<Real> values_;
    CoordinateKind1D kind_;
};

}  // namespace fvgrid