// ----------------------------------------------------------------------------
// File: tst_FVGridException.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Tests the base exception type used by FVGridMaker.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <source_location>
#include <string>
#include <string_view>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/ErrorHandling/FVGridException.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

TEST(FVGridException, StoresErrorRecord) {
    const ErrorRecord record{
        .code = "FVGRID.TEST.EXCEPTION",
        .message = "exception test message",
        .category = "Test",
        .source = ID{
            "ErrorHandling",
            "FVGridExceptionTest",
            "fvgrid.test.FVGridExceptionTest"
        },
    };

    const FVGridException exception{record};

    EXPECT_EQ(exception.record().code, std::string_view{"FVGRID.TEST.EXCEPTION"});
    EXPECT_EQ(exception.record().message, "exception test message");
    EXPECT_EQ(exception.record().category, std::string_view{"Test"});
    EXPECT_EQ(exception.record().source.module(), std::string_view{"ErrorHandling"});
    EXPECT_EQ(exception.record().source.class_name(), std::string_view{"FVGridExceptionTest"});
    EXPECT_EQ(exception.record().source.class_id(), std::string_view{"fvgrid.test.FVGridExceptionTest"});
}

TEST(FVGridException, WhatContainsCodeMessageCategoryAndSource) {
    const ErrorRecord record{
        .code = "FVGRID.TEST.WHAT",
        .message = "what test message",
        .category = "Test",
        .source = ID{
            "ErrorHandling",
            "FVGridExceptionTest",
            "fvgrid.test.FVGridExceptionTest"
        },
    };

    const FVGridException exception{record};
    const std::string what{exception.what()};

    EXPECT_NE(what.find("FVGRID.TEST.WHAT"), std::string::npos);
    EXPECT_NE(what.find("what test message"), std::string::npos);
    EXPECT_NE(what.find("Test"), std::string::npos);
    EXPECT_NE(what.find("ErrorHandling"), std::string::npos);
    EXPECT_NE(what.find("FVGridExceptionTest"), std::string::npos);
    EXPECT_NE(what.find("fvgrid.test.FVGridExceptionTest"), std::string::npos);
}

TEST(FVGridException, WhatContainsSourceLocation) {
    constexpr auto location = std::source_location::current();

    const ErrorRecord record{
        .code = "FVGRID.TEST.LOCATION",
        .message = "location test message",
        .category = "Test",
        .source = ID{
            "ErrorHandling",
            "FVGridExceptionTest",
            "fvgrid.test.FVGridExceptionTest"
        },
        .location = location,
    };

    const FVGridException exception{record};
    const std::string what{exception.what()};

    EXPECT_NE(what.find(location.file_name()), std::string::npos);
}

}  // namespace fvgrid