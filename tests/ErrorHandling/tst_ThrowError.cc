// ----------------------------------------------------------------------------
// File: tst_ThrowError.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Tests helper functions for throwing FVGridMaker exceptions.
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
#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/ErrorHandling/FVGridException.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

namespace {

[[nodiscard]] constexpr ID test_id() noexcept {
    return ID{
        "ErrorHandling",
        "ThrowErrorTest",
        "fvgrid.test.ThrowErrorTest"
    };
}

}  // namespace

TEST(ThrowError, ThrowErrorThrowsFVGridException) {
    EXPECT_THROW(
        throw_error(
            "FVGRID.TEST.THROW",
            "throw test message",
            "Test",
            test_id()
        ),
        FVGridException
    );
}

TEST(ThrowError, ThrowErrorStoresRecordFields) {
    try {
        throw_error(
            "FVGRID.TEST.RECORD",
            "record test message",
            "Test",
            test_id()
        );
    } catch (const FVGridException& exception) {
        EXPECT_EQ(exception.record().code, std::string_view{"FVGRID.TEST.RECORD"});
        EXPECT_EQ(exception.record().message, "record test message");
        EXPECT_EQ(exception.record().category, std::string_view{"Test"});
        EXPECT_EQ(exception.record().source.module(), std::string_view{"ErrorHandling"});
        EXPECT_EQ(exception.record().source.class_name(), std::string_view{"ThrowErrorTest"});
        EXPECT_EQ(exception.record().source.class_id(), std::string_view{"fvgrid.test.ThrowErrorTest"});
        return;
    }

    FAIL() << "throw_error did not throw FVGridException.";
}

TEST(ThrowError, ThrowErrorStoresExplicitSourceLocation) {
    constexpr auto location = std::source_location::current();

    try {
        throw_error(
            "FVGRID.TEST.LOCATION",
            "location test message",
            "Test",
            test_id(),
            location
        );
    } catch (const FVGridException& exception) {
        EXPECT_EQ(exception.record().location.line(), location.line());
        EXPECT_STREQ(exception.record().location.file_name(), location.file_name());
        return;
    }

    FAIL() << "throw_error did not throw FVGridException.";
}

TEST(Require, DoesNothingWhenConditionIsTrue) {
    EXPECT_NO_THROW(
        require(
            true,
            "FVGRID.TEST.REQUIRE",
            "require test message",
            "Test",
            test_id()
        )
    );
}

TEST(Require, ThrowsWhenConditionIsFalse) {
    EXPECT_THROW(
        require(
            false,
            "FVGRID.TEST.REQUIRE",
            "require test message",
            "Test",
            test_id()
        ),
        FVGridException
    );
}

TEST(Require, StoresRecordFieldsWhenConditionIsFalse) {
    try {
        require(
            false,
            "FVGRID.TEST.REQUIRE.RECORD",
            "require record message",
            "Test",
            test_id()
        );
    } catch (const FVGridException& exception) {
        EXPECT_EQ(exception.record().code, std::string_view{"FVGRID.TEST.REQUIRE.RECORD"});
        EXPECT_EQ(exception.record().message, "require record message");
        EXPECT_EQ(exception.record().category, std::string_view{"Test"});
        EXPECT_EQ(exception.record().source.module(), std::string_view{"ErrorHandling"});
        EXPECT_EQ(exception.record().source.class_name(), std::string_view{"ThrowErrorTest"});
        EXPECT_EQ(exception.record().source.class_id(), std::string_view{"fvgrid.test.ThrowErrorTest"});
        return;
    }

    FAIL() << "require did not throw FVGridException.";
}

}  // namespace fvgrid