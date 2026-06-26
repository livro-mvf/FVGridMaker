// ----------------------------------------------------------------------------
// File: tst_Types.cc
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Tests the fundamental type aliases used by the FVGridMaker core.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <cstddef>
#include <cstdint>
#include <type_traits>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/Types.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

TEST(Types, RealIsDouble) {
    static_assert(std::is_same_v<Real, double>);
    SUCCEED();
}

TEST(Types, IndexIsSignedPointerSizedInteger) {
    static_assert(std::is_same_v<Index, std::ptrdiff_t>);
    static_assert(std::is_signed_v<Index>);
    SUCCEED();
}

TEST(Types, SizeIsUnsignedSizeType) {
    static_assert(std::is_same_v<Size, std::size_t>);
    static_assert(std::is_unsigned_v<Size>);
    SUCCEED();
}

TEST(Types, SignedIntegerAliasesMatchStdTypes) {
    static_assert(std::is_same_v<Int8, std::int8_t>);
    static_assert(std::is_same_v<Int16, std::int16_t>);
    static_assert(std::is_same_v<Int32, std::int32_t>);
    static_assert(std::is_same_v<Int64, std::int64_t>);
    SUCCEED();
}

TEST(Types, UnsignedIntegerAliasesMatchStdTypes) {
    static_assert(std::is_same_v<UInt8, std::uint8_t>);
    static_assert(std::is_same_v<UInt16, std::uint16_t>);
    static_assert(std::is_same_v<UInt32, std::uint32_t>);
    static_assert(std::is_same_v<UInt64, std::uint64_t>);
    SUCCEED();
}

}  // namespace fvgrid
