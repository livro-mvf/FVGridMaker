// ----------------------------------------------------------------------------
// File: tst_ID.cc
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Tests the lightweight immutable identity type used by
//              FVGridMaker classes.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <string_view>
#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/ID.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

TEST(ID, DefaultIdentityIsUnknown) {
    constexpr ID id;

    EXPECT_EQ(id.module(), std::string_view{"Unknown"});
    EXPECT_EQ(id.class_name(), std::string_view{"Unknown"});
    EXPECT_EQ(id.class_id(), std::string_view{"fvgrid.unknown.Unknown"});
}

TEST(ID, StoresModuleClassNameAndClassId) {
    constexpr ID id{
        "OneDimensional",
        "Axis1D",
        "fvgrid.1d.axis.Axis1D"
    };

    EXPECT_EQ(id.module(), std::string_view{"OneDimensional"});
    EXPECT_EQ(id.class_name(), std::string_view{"Axis1D"});
    EXPECT_EQ(id.class_id(), std::string_view{"fvgrid.1d.axis.Axis1D"});
}

TEST(ID, IsFinalAndNotPolymorphic) {
    static_assert(std::is_final_v<ID>);
    static_assert(!std::is_polymorphic_v<ID>);

    SUCCEED();
}

TEST(ID, IsLightweightValueType) {
    static_assert(std::is_copy_constructible_v<ID>);
    static_assert(std::is_copy_assignable_v<ID>);
    static_assert(std::is_move_constructible_v<ID>);
    static_assert(std::is_move_assignable_v<ID>);
    static_assert(std::is_nothrow_copy_constructible_v<ID>);
    static_assert(std::is_nothrow_move_constructible_v<ID>);

    SUCCEED();
}

TEST(ID, IsCopyConstructible) {
    constexpr ID original{
        "Core",
        "ID",
        "fvgrid.core.ID"
    };

    constexpr ID copy{original};

    EXPECT_EQ(copy.module(), original.module());
    EXPECT_EQ(copy.class_name(), original.class_name());
    EXPECT_EQ(copy.class_id(), original.class_id());
}

TEST(ID, IsMoveConstructible) {
    ID original{
        "Core",
        "ID",
        "fvgrid.core.ID"
    };

    const ID moved{std::move(original)};

    EXPECT_EQ(moved.module(), std::string_view{"Core"});
    EXPECT_EQ(moved.class_name(), std::string_view{"ID"});
    EXPECT_EQ(moved.class_id(), std::string_view{"fvgrid.core.ID"});
}

}  // namespace fvgrid
