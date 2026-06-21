// ----------------------------------------------------------------------------
// File: Version.h
// Project: FVGridMaker
// Version: 0.1.0
// Description: Declares version information for the FVGridMaker core.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <string_view>

namespace fvgrid {

inline constexpr int kVersionMajor = 1;
inline constexpr int kVersionMinor = 0;
inline constexpr int kVersionPatch = 0;

inline constexpr std::string_view kVersionString = "1.0.0";

[[nodiscard]] constexpr int version_major() noexcept {
    return kVersionMajor;
}

[[nodiscard]] constexpr int version_minor() noexcept {
    return kVersionMinor;
}

[[nodiscard]] constexpr int version_patch() noexcept {
    return kVersionPatch;
}

[[nodiscard]] constexpr std::string_view version_string() noexcept {
    return kVersionString;
}

}  // namespace fvgrid