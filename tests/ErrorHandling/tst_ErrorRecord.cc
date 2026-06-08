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

TEST(ErrorRecord, StoresCodeMessageCategoryAndSource) {
    const ErrorRecord record{
        .code = "FVGRID.TEST.ERROR",
        .message = "test error message",
        .category = "Test",
        .source = ID{
            "ErrorHandling",
            "ErrorRecordTest",
            "fvgrid.test.ErrorRecordTest"
        },
    };

    EXPECT_EQ(record.code, std::string_view{"FVGRID.TEST.ERROR"});
    EXPECT_EQ(record.message, "test error message");
    EXPECT_EQ(record.category, std::string_view{"Test"});
    EXPECT_EQ(record.source.module(), std::string_view{"ErrorHandling"});
    EXPECT_EQ(record.source.class_name(), std::string_view{"ErrorRecordTest"});
    EXPECT_EQ(record.source.class_id(), std::string_view{"fvgrid.test.ErrorRecordTest"});
}

TEST(ErrorRecord, StoresExplicitSourceLocation) {
    constexpr auto location = std::source_location::current();

    const ErrorRecord record{
        .code = "FVGRID.TEST.LOCATION",
        .message = "location test",
        .category = "Test",
        .source = ID{
            "ErrorHandling",
            "ErrorRecordTest",
            "fvgrid.test.ErrorRecordTest"
        },
        .location = location,
    };

    EXPECT_EQ(record.location.line(), location.line());
    EXPECT_STREQ(record.location.file_name(), location.file_name());
}

}  // namespace fvgrid