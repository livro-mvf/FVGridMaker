// ----------------------------------------------------------------------------
// File: tst_ErrorTraits.cc
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Tests compile-time contracts and helpers for FVGridMaker error
//              descriptor providers.
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
#include <FVGridMaker/ErrorHandling/BuiltInErrors.h>
#include <FVGridMaker/ErrorHandling/ErrorTraits.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>


namespace fvgrid {

namespace {

struct ValidUserError final {
    static constexpr std::string_view code =
        "USER.TEST.ERROR";

    static constexpr std::string_view message =
        "User test error.";

    static constexpr std::string_view category =
        "User";
};

struct MissingCategoryError final {
    static constexpr std::string_view code =
        "USER.TEST.MISSING_CATEGORY";

    static constexpr std::string_view message =
        "Missing category.";
};

}  // namespace

TEST(ErrorTraits, RecognizesValidDescriptorProviders) {
    static_assert(ErrorDescriptorProvider<ValidUserError>);
    static_assert(ErrorDescriptorProvider<errors::grid::InvalidNVol>);
    static_assert(ErrorDescriptorProvider<errors::output::FileOpenFailed>);

    SUCCEED();
}

TEST(ErrorTraits, RejectsInvalidDescriptorProviders) {
    static_assert(!ErrorDescriptorProvider<MissingCategoryError>);
    static_assert(!ErrorDescriptorProvider<int>);

    SUCCEED();
}

TEST(ErrorTraits, DescriptorOfBuildsDescriptorFromProvider) {
    constexpr ErrorDescriptor descriptor = descriptor_of<ValidUserError>();

    EXPECT_EQ(descriptor.code, std::string_view{"USER.TEST.ERROR"});
    EXPECT_EQ(descriptor.message, std::string_view{"User test error."});
    EXPECT_EQ(descriptor.category, std::string_view{"User"});
}

TEST(ErrorTraits, DescriptorOfWorksForBuiltInErrors) {
    constexpr ErrorDescriptor descriptor =
        descriptor_of<errors::grid::InvalidNVol>();

    EXPECT_EQ(descriptor.code, std::string_view{"FVGRID.GRID.INVALID_NVOL"});
    EXPECT_EQ(
        descriptor.message,
        std::string_view{"The number of control volumes must be positive."}
    );
    EXPECT_EQ(descriptor.category, std::string_view{"Grid"});
}

}  // namespace fvgrid
