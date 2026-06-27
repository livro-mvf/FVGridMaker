// ----------------------------------------------------------------------------
// File: StrongTypes.h
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Defines strongly typed wrappers for scalar and integer
//              configuration values used by the FVGridMaker core.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <compare>
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

    friend constexpr bool operator==(
        const StrongReal&,
        const StrongReal&
    ) noexcept = default;

    friend constexpr auto operator<=> (
        const StrongReal&,
        const StrongReal&
    ) noexcept = default;

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

    friend constexpr bool operator==(
        const StrongInteger&,
        const StrongInteger&
    ) noexcept = default;

    friend constexpr auto operator<=> (
        const StrongInteger&,
        const StrongInteger&
    ) noexcept = default;

private:
    T value_;
};

struct NVolTag final {};
struct LengthTag final {};
struct XInitTag final {};
struct XFinalTag final {};
struct MinSpacingTag final {};
struct BetaTag final {};
struct SeedTag final {};

using NVol = StrongInteger<Size, NVolTag>;
using Seed = StrongInteger<UInt64, SeedTag>;

template <std::floating_point T>
using BasicLength = StrongReal<T, LengthTag>;

template <std::floating_point T>
using BasicXInit = StrongReal<T, XInitTag>;

template <std::floating_point T>
using BasicXFinal = StrongReal<T, XFinalTag>;

template <std::floating_point T>
using BasicMinSpacing = StrongReal<T, MinSpacingTag>;

template <std::floating_point T>
using BasicBeta = StrongReal<T, BetaTag>;

using Length = BasicLength<double>;
using XInit = BasicXInit<double>;
using XFinal = BasicXFinal<double>;
using MinSpacing = BasicMinSpacing<double>;
using Beta = BasicBeta<double>;

using LengthFloat = BasicLength<float>;
using XInitFloat = BasicXInit<float>;
using XFinalFloat = BasicXFinal<float>;
using MinSpacingFloat = BasicMinSpacing<float>;
using BetaFloat = BasicBeta<float>;

using LengthLongDouble = BasicLength<long double>;
using XInitLongDouble = BasicXInit<long double>;
using XFinalLongDouble = BasicXFinal<long double>;
using MinSpacingLongDouble = BasicMinSpacing<long double>;
using BetaLongDouble = BasicBeta<long double>;

}  // namespace fvgrid
