// ----------------------------------------------------------------------------
// File: ConstantWeight1D.h
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Defines a constant interpolation weight for 1D grid patterns.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <cmath>
#include <string_view>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/ErrorHandling/BuiltInErrors.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>

namespace fvgrid {

class ConstantWeight1D final {
public:
    explicit ConstantWeight1D(Real value)
        : value_{value} {
        validate(value_);
    }

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "ConstantWeight1D",
            "fvgrid.1d.grid_pattern.ConstantWeight1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] Real value() const noexcept {
        return value_;
    }

    [[nodiscard]] Real operator()(Size) const noexcept {
        return value_;
    }

private:
    static void validate(Real value) {
        require<errors::core::InvalidArgument>(
            std::isfinite(value) &&
            value > static_cast<Real>(0.0) &&
            value < static_cast<Real>(1.0),
            ConstantWeight1D::id()
        );
    }

    Real value_;
};

}  // namespace fvgrid