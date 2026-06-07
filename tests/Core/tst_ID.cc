// ----------------------------------------------------------------------------
// File: tst_ID.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Tests the lightweight ID type used by the FVGridMaker core.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/ID.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

TEST(ID, DefaultClassNameIsAvailable) {
    const ID id;

    EXPECT_EQ(id.class_name(), "ID");
}

TEST(ID, DefaultClassIdIsAvailable) {
    const ID id;

    EXPECT_EQ(id.class_id(), "fvgrid.core.ID");
}

TEST(ID, IsCopyConstructible) {
    const ID original;
    const ID copy{original};

    EXPECT_EQ(copy.class_name(), original.class_name());
    EXPECT_EQ(copy.class_id(), original.class_id());
}

TEST(ID, IsMoveConstructible) {
    ID original;
    const ID moved{std::move(original)};

    EXPECT_EQ(moved.class_name(), "ID");
    EXPECT_EQ(moved.class_id(), "fvgrid.core.ID");
}

}  // namespace fvgrid