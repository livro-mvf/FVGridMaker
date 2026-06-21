// ----------------------------------------------------------------------------
// File: tst_Version.cc
// Project: FVGridMaker
// Version: 0.1.0
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
    EXPECT_EQ(version_major(), 1);
    EXPECT_EQ(version_minor(), 0);
    EXPECT_EQ(version_patch(), 0);
}

TEST(Version, VersionStringIsAvailable) {
    EXPECT_EQ(version_string(), std::string_view{"1.0.0"});
}

TEST(Version, RootVersionFunctionMatchesVersionString) {
    EXPECT_STREQ(version(), version_string().data());
}

TEST(Version, VersionConstantsMatchAccessors) {
    EXPECT_EQ(kVersionMajor, version_major());
    EXPECT_EQ(kVersionMinor, version_minor());
    EXPECT_EQ(kVersionPatch, version_patch());
    EXPECT_EQ(kVersionString, version_string());
}

}  // namespace fvgrid