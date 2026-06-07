// ----------------------------------------------------------------------------
// File: tst_ErrorRecord.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Tests the structured error record used by FVGridMaker.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <source_location>
#include <string_view>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/ErrorRecord.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

TEST(ErrorRecord, StoresCodeMessageAndModule) {
    const ErrorRecord record{
        .code = "FVGRID.TEST.ERROR",
        .message = "test error message",
        .module = "ErrorHandling",
    };

    EXPECT_EQ(record.code, std::string_view{"FVGRID.TEST.ERROR"});
    EXPECT_EQ(record.message, "test error message");
    EXPECT_EQ(record.module, std::string_view{"ErrorHandling"});
}

TEST(ErrorRecord, StoresExplicitSourceLocation) {
    constexpr auto location = std::source_location::current();

    const ErrorRecord record{
        .code = "FVGRID.TEST.LOCATION",
        .message = "location test",
        .module = "ErrorHandling",
        .location = location,
    };

    EXPECT_EQ(record.location.line(), location.line());
    EXPECT_STREQ(record.location.file_name(), location.file_name());
}

}  // namespace fvgrid