
// ----------------------------------------------------------------------------
// File: Version.h
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Declares version information for the FVGridMaker core.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <string_view>

// ----------------------------------------------------------------------------
// FVGridMaker generated includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/version.hpp>

namespace fvgrid {

inline constexpr int kVersionMajor =
    static_cast<int>(build_info::VersionInfo::kMajor);

inline constexpr int kVersionMinor =
    static_cast<int>(build_info::VersionInfo::kMinor);

inline constexpr int kVersionPatch =
    static_cast<int>(build_info::VersionInfo::kPatch);

inline constexpr std::string_view kVersionString =
    build_info::VersionInfo::kSemanticVersion;

inline constexpr std::string_view kFullVersionString =
    build_info::VersionInfo::kFullVersion;

inline constexpr std::string_view kGitHash =
    build_info::VersionInfo::kGitHash;

inline constexpr std::string_view kGitBranch =
    build_info::VersionInfo::kGitBranch;

inline constexpr std::string_view kBuildTimestamp =
    build_info::VersionInfo::kBuildTimestamp;

inline constexpr std::string_view kBuildDate =
    build_info::VersionInfo::kBuildDate;

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

[[nodiscard]] constexpr std::string_view full_version_string() noexcept {
    return kFullVersionString;
}

[[nodiscard]] constexpr std::string_view git_hash() noexcept {
    return kGitHash;
}

[[nodiscard]] constexpr std::string_view git_branch() noexcept {
    return kGitBranch;
}

[[nodiscard]] constexpr std::string_view build_timestamp() noexcept {
    return kBuildTimestamp;
}

[[nodiscard]] constexpr std::string_view build_date() noexcept {
    return kBuildDate;
}

[[nodiscard]] constexpr bool is_development_build() noexcept {
    return build_info::VersionInfo::is_development_build();
}

[[nodiscard]] constexpr bool is_tagged_release() noexcept {
    return build_info::VersionInfo::is_tagged_release();
}

}  // namespace fvgrid

