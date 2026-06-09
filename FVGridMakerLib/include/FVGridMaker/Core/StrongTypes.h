// ----------------------------------------------------------------------------
// File: StrongTypes.h
// Project: FVGridMaker
// Version: 0.1.0
// Description: Defines strongly typed wrappers for scalar and integer
//              configuration values used by the FVGridMaker core.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <concepts>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/Types.h>

namespace fvgrid {

template <std::floating_point T, class Tag>
class StrongReal final {
public:
    using value_type = T;
    using tag_type = Tag;

    constexpr explicit StrongReal(T value) noexcept : value_(value) {}

    [[nodiscard]] constexpr T value() const noexcept {
        return value_;
    }

private:
    T value_;
};

template <std::integral T, class Tag>
class StrongInteger final {
public:
    using value_type = T;
    using tag_type = Tag;

    constexpr explicit StrongInteger(T value) noexcept : value_(value) {}

    [[nodiscard]] constexpr T value() const noexcept {
        return value_;
    }

private:
    T value_;
};

struct NVolTag final {};
struct LengthTag final {};
struct XInitTag final {};
struct XFinalTag final {};
struct MinSpacingTag final {};
struct SeedTag final {};

using NVol = StrongInteger<Size, NVolTag>;
using Length = StrongReal<Real, LengthTag>;
using XInit = StrongReal<Real, XInitTag>;
using XFinal = StrongReal<Real, XFinalTag>;
using MinSpacing = StrongReal<Real, MinSpacingTag>;
using Seed = StrongInteger<UInt64, SeedTag>;

}  // namespace fvgrid