// ----------------------------------------------------------------------------
// File: tst_ErrorContext.cc
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Tests structured diagnostic context entries for FVGridMaker
//              errors.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <string>
#include <string_view>
#include <type_traits>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/ErrorContext.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

TEST(ErrorContext, StoresKeyAndValue) {
    const ErrorContext context{
        .key = "nvol",
        .value = "0",
    };

    EXPECT_EQ(context.key, std::string{"nvol"});
    EXPECT_EQ(context.value, std::string{"0"});
}

TEST(ErrorContext, MakeErrorContextCopiesStringViews) {
    const std::string key{"length"};
    const std::string value{"-1.0"};

    const ErrorContext context = make_error_context(key, value);

    EXPECT_EQ(context.key, std::string{"length"});
    EXPECT_EQ(context.value, std::string{"-1.0"});
}

TEST(ErrorContext, ErrorContextListIsVectorOfErrorContext) {
    static_assert(std::is_same_v<ErrorContextList, std::vector<ErrorContext>>);

    ErrorContextList context;
    context.push_back(make_error_context("beta", "1.0"));
    context.push_back(make_error_context("expected", "> 1"));

    ASSERT_EQ(context.size(), 2U);
    EXPECT_EQ(context[0].key, std::string{"beta"});
    EXPECT_EQ(context[0].value, std::string{"1.0"});
    EXPECT_EQ(context[1].key, std::string{"expected"});
    EXPECT_EQ(context[1].value, std::string{"> 1"});
}

}  // namespace fvgrid
