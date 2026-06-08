// ----------------------------------------------------------------------------
// File: tst_StrongTypes.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Tests the strongly typed wrappers used by the FVGridMaker core.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <type_traits>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/StrongTypes.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

TEST(StrongReal, StoresRealValue) {
    const StrongReal<Real, LengthTag> value{1.25};

    EXPECT_DOUBLE_EQ(value.value(), 1.25);
}

TEST(StrongInteger, StoresIntegerValue) {
    const StrongInteger<Size, NVolTag> value{42};

    EXPECT_EQ(value.value(), static_cast<Size>(42));
}

TEST(StrongTypes, NVolUsesSize) {
    const NVol nvol{100};

    static_assert(std::is_same_v<NVol::value_type, Size>);
    static_assert(std::is_same_v<NVol::tag_type, NVolTag>);
    EXPECT_EQ(nvol.value(), static_cast<Size>(100));
}

TEST(StrongTypes, LengthUsesReal) {
    const Length length{2.5};

    static_assert(std::is_same_v<Length::value_type, Real>);
    static_assert(std::is_same_v<Length::tag_type, LengthTag>);
    EXPECT_DOUBLE_EQ(length.value(), 2.5);
}

TEST(StrongTypes, XInitUsesReal) {
    const XInit xinit{-1.0};

    static_assert(std::is_same_v<XInit::value_type, Real>);
    static_assert(std::is_same_v<XInit::tag_type, XInitTag>);
    EXPECT_DOUBLE_EQ(xinit.value(), -1.0);
}

TEST(StrongTypes, XFinalUsesReal) {
    const XFinal xfinal{3.0};

    static_assert(std::is_same_v<XFinal::value_type, Real>);
    static_assert(std::is_same_v<XFinal::tag_type, XFinalTag>);
    EXPECT_DOUBLE_EQ(xfinal.value(), 3.0);
}

TEST(StrongTypes, SeedUsesUInt64) {
    const Seed seed{123456};

    static_assert(std::is_same_v<Seed::value_type, UInt64>);
    static_assert(std::is_same_v<Seed::tag_type, SeedTag>);
    EXPECT_EQ(seed.value(), static_cast<UInt64>(123456));
}

TEST(StrongTypes, DifferentStrongTypesAreDifferentTypes) {
    static_assert(!std::is_same_v<NVol, Seed>);
    static_assert(!std::is_same_v<Length, XInit>);
    static_assert(!std::is_same_v<XInit, XFinal>);
    static_assert(!std::is_same_v<Length, StrongReal<Real, XInitTag>>);
    static_assert(!std::is_same_v<NVol, StrongInteger<Size, SeedTag>>);

    SUCCEED();
}

}  // namespace fvgrid