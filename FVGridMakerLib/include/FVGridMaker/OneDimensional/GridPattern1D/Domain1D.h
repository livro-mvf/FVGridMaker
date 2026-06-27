// ----------------------------------------------------------------------------
// File: Domain1D.h
// Project: FVGridMaker
// Version: 0.1.0
// Description: Defines a lightweight one-dimensional physical domain.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <string_view>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/ErrorHandling/BuiltInErrors.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>

namespace fvgrid {

class Domain1D final {
public:
    [[nodiscard]] static constexpr Domain1D none() noexcept {
        return Domain1D{};
    }

    [[nodiscard]] static constexpr Domain1D from_length(
        XInit xinit,
        Length length
    ) noexcept {
        return Domain1D{
            xinit.value(),
            xinit.value() + length.value(),
            true
        };
    }

    [[nodiscard]] static constexpr Domain1D from_bounds(
        XInit xinit,
        XFinal xfinal
    ) noexcept {
        return Domain1D{xinit.value(), xfinal.value(), true};
    }

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "Domain1D",
            "fvgrid.1d.pattern.Domain1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] constexpr bool has_bounds() const noexcept {
        return has_bounds_;
    }

    [[nodiscard]] Real xmin() const {
        require<errors::core::InvalidArgument>(
            has_bounds_,
            Domain1D::id()
        );

        return xmin_;
    }

    [[nodiscard]] Real xmax() const {
        require<errors::core::InvalidArgument>(
            has_bounds_,
            Domain1D::id()
        );

        return xmax_;
    }

    [[nodiscard]] Real length() const {
        require<errors::core::InvalidArgument>(
            has_bounds_,
            Domain1D::id()
        );

        return xmax_ - xmin_;
    }

private:
    constexpr Domain1D() noexcept = default;

    constexpr Domain1D(
        Real xmin,
        Real xmax,
        bool has_bounds
    ) noexcept
        : xmin_(xmin),
          xmax_(xmax),
          has_bounds_(has_bounds) {}

    Real xmin_ = static_cast<Real>(0.0);
    Real xmax_ = static_cast<Real>(0.0);
    bool has_bounds_ = false;
};

}  // namespace fvgrid