// ----------------------------------------------------------------------------
// File: ErrorTraits.h
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Defines compile-time contracts and helpers for FVGridMaker
//              error descriptor providers.
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
#include <FVGridMaker/ErrorHandling/ErrorDescriptor.h>

namespace fvgrid {

template <class Error>
concept ErrorDescriptorProvider =
    requires {
        { Error::code } -> std::convertible_to<std::string_view>;
        { Error::message } -> std::convertible_to<std::string_view>;
        { Error::category } -> std::convertible_to<std::string_view>;
    };

template <ErrorDescriptorProvider Error>
[[nodiscard]] constexpr ErrorDescriptor descriptor_of() noexcept {
    return ErrorDescriptor{
        .code = Error::code,
        .message = Error::message,
        .category = Error::category,
    };
}

}  // namespace fvgrid
