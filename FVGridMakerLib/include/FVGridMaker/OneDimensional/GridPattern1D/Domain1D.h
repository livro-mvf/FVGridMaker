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
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/Core/Types.h>

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

    [[nodiscard]] constexpr bool has_bounds() const noexcept {
        return has_bounds_;
    }

    [[nodiscard]] constexpr Real xmin() const noexcept {
        return xmin_;
    }

    [[nodiscard]] constexpr Real xmax() const noexcept {
        return xmax_;
    }

    [[nodiscard]] constexpr Real length() const noexcept {
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