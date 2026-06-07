#include <FVGridMaker/FVGridMaker.h>

#include <gtest/gtest.h>

TEST(FVGridMakerMinimal, VersionIsAvailable) {
    EXPECT_STREQ(fvgrid::version(), "0.1.0");
}
