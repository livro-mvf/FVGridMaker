// ----------------------------------------------------------------------------
// File: tst_ID.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Tests the lightweight immutable identity type used by
//              FVGridMaker classes.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <string_view>
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