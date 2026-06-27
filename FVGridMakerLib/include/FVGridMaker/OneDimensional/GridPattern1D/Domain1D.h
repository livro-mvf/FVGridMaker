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
#include <concepts>
#include <string_view>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/ErrorHandling/BuiltInErrors.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>

namespace fvgrid {

template <std::floating_point T>
class BasicDomain1D final {
public:
    using value_type = T;

    [[nodiscard]] static constexpr BasicDomain1D none() noexcept {
        return BasicDomain1D{};
    }

    [[nodiscard]] static constexpr BasicDomain1D from_length(
        BasicXInit<T> xinit,
        BasicLength<T> length
    ) noexcept {
        return BasicDomain1D{
            xinit.value(),
            xinit.value() + length.value(),
            true
        };
    }

    [[nodiscard]] static constexpr BasicDomain1D from_bounds(
        BasicXInit<T> xinit,
        BasicXFinal<T> xfinal
    ) noexcept {
        return BasicDomain1D{xinit.value(), xfinal.value(), true};
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

    [[nodiscard]] T xmin() const {
        require<errors::core::InvalidArgument>(
            has_bounds_,
            BasicDomain1D::id()
        );

        return xmin_;
    }

    [[nodiscard]] T xmax() const {
        require<errors::core::InvalidArgument>(
            has_bounds_,
            BasicDomain1D::id()
        );

        return xmax_;
    }

    [[nodiscard]] T length() const {
        require<errors::core::InvalidArgument>(
            has_bounds_,
            BasicDomain1D::id()
        );

        return xmax_ - xmin_;
    }

private:
    constexpr BasicDomain1D() noexcept = default;

    constexpr BasicDomain1D(
        T xmin,
        T xmax,
        bool has_bounds
    ) noexcept
        : xmin_(xmin),
          xmax_(xmax),
          has_bounds_(has_bounds) {}

    T xmin_{};
    T xmax_{};
    bool has_bounds_{false};
};

using Domain1D = BasicDomain1D<double>;
using Domain1DFloat = BasicDomain1D<float>;
using Domain1DLongDouble = BasicDomain1D<long double>;

}  // namespace fvgrid
