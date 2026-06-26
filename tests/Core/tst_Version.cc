// ----------------------------------------------------------------------------
// File: tst_Version.cc
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Tests the version constants and accessors used by FVGridMaker.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <string_view>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/Version.h>
#include <FVGridMaker/FVGridMaker.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

TEST(Version, VersionNumbersAreAvailable) {
    EXPECT_EQ(version_major(), 0);
    EXPECT_EQ(version_minor(), 2);
    EXPECT_EQ(version_patch(), 0);
}

TEST(Version, VersionStringIsAvailable) {
    EXPECT_EQ(version_string(), std::string_view{"0.2.0"});
}

TEST(Version, FullVersionStringIsAvailable) {
    EXPECT_FALSE(full_version_string().empty());
    EXPECT_TRUE(full_version_string().starts_with(version_string()));
}

TEST(Version, RootVersionFunctionMatchesVersionString) {
    EXPECT_STREQ(version(), version_string().data());
}

TEST(Version, VersionConstantsMatchAccessors) {
    EXPECT_EQ(kVersionMajor, version_major());
    EXPECT_EQ(kVersionMinor, version_minor());
    EXPECT_EQ(kVersionPatch, version_patch());
    EXPECT_EQ(kVersionString, version_string());
    EXPECT_EQ(kFullVersionString, full_version_string());
}

TEST(Version, GitMetadataAccessorsAreAvailable) {
    EXPECT_FALSE(git_hash().empty());
    EXPECT_FALSE(git_branch().empty());
}

TEST(Version, BuildMetadataAccessorsAreAvailable) {
    EXPECT_FALSE(build_timestamp().empty());
    EXPECT_FALSE(build_date().empty());
}

TEST(Version, BuildClassificationAccessorsAreCallable) {
    [[maybe_unused]] const bool development_build = is_development_build();
    [[maybe_unused]] const bool tagged_release = is_tagged_release();

    SUCCEED();
}

}  // namespace fvgrid

