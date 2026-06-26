// ----------------------------------------------------------------------------
// File: tst_ErrorDescriptor.cc
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Tests immutable error descriptors used by FVGridMaker.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <string_view>
#include <type_traits>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/ErrorDescriptor.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

    
TEST(ErrorDescriptor, StoresCodeMessageAndCategory) {
    constexpr ErrorDescriptor descriptor{
        .code = "FVGRID.TEST.ERROR",
        .message = "Test error message.",
        .category = "Test",
    };

    EXPECT_EQ(descriptor.code, std::string_view{"FVGRID.TEST.ERROR"});
    EXPECT_EQ(descriptor.message, std::string_view{"Test error message."});
    EXPECT_EQ(descriptor.category, std::string_view{"Test"});
}

TEST(ErrorDescriptor, IsFinalAndNotPolymorphic) {
    static_assert(std::is_final_v<ErrorDescriptor>);
    static_assert(!std::is_polymorphic_v<ErrorDescriptor>);

    SUCCEED();
}

TEST(ErrorDescriptor, IsLightweightValueType) {
    static_assert(std::is_copy_constructible_v<ErrorDescriptor>);
    static_assert(std::is_copy_assignable_v<ErrorDescriptor>);
    static_assert(std::is_move_constructible_v<ErrorDescriptor>);
    static_assert(std::is_move_assignable_v<ErrorDescriptor>);

    SUCCEED();
}

}  // namespace fvgrid
