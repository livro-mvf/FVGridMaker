// ----------------------------------------------------------------------------
// File: tst_Axis1D.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Tests the one-dimensional structured finite-volume axis.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <string_view>
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/FVGridException.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/FaceCentered1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

TEST(Axis1D, BuildsFromStrictlyIncreasingFaces) {
    const Axis1D axis{{0.0, 0.5, 1.0, 2.0}};

    EXPECT_EQ(axis.num_faces(), static_cast<Size>(4));
    EXPECT_EQ(axis.num_cells(), static_cast<Size>(3));
}

TEST(Axis1D, StoresFaces) {
    const Axis1D axis{{0.0, 0.5, 1.0}};

    ASSERT_EQ(axis.faces().size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(axis.faces()[0], 0.0);
    EXPECT_DOUBLE_EQ(axis.faces()[1], 0.5);
    EXPECT_DOUBLE_EQ(axis.faces()[2], 1.0);
}

TEST(Axis1D, ComputesCenters) {
    const Axis1D axis{{0.0, 0.5, 1.0, 2.0}};

    ASSERT_EQ(axis.centers().size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(axis.centers()[0], 0.25);
    EXPECT_DOUBLE_EQ(axis.centers()[1], 0.75);
    EXPECT_DOUBLE_EQ(axis.centers()[2], 1.5);
}

TEST(Axis1D, ComputesCellLengths) {
    const Axis1D axis{{0.0, 0.5, 1.0, 2.0}};

    ASSERT_EQ(axis.cell_lengths().size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(axis.cell_lengths()[0], 0.5);
    EXPECT_DOUBLE_EQ(axis.cell_lengths()[1], 0.5);
    EXPECT_DOUBLE_EQ(axis.cell_lengths()[2], 1.0);
}

TEST(Axis1D, ComputesBoundsAndLength) {
    const Axis1D axis{{-1.0, 0.0, 2.0}};

    EXPECT_DOUBLE_EQ(axis.xmin(), -1.0);
    EXPECT_DOUBLE_EQ(axis.xmax(), 2.0);
    EXPECT_DOUBLE_EQ(axis.length(), 3.0);
}

TEST(Axis1D, UsesVolumeCenteredPatternByDefault) {
    const Axis1D axis{{0.0, 0.5, 1.0}};

    EXPECT_EQ(axis.pattern_name(), VolumeCentered1D::name());
}

TEST(Axis1D, StoresExplicitFaceCenteredPattern) {
    const Axis1D axis{
        {0.0, 0.5, 1.0},
        FaceCentered1D::name()
    };

    EXPECT_EQ(axis.pattern_name(), FaceCentered1D::name());
}

TEST(Axis1D, StoresCustomPatternName) {
    const Axis1D axis{
        {0.0, 0.5, 1.0},
        "CustomPattern1D"
    };

    EXPECT_EQ(axis.pattern_name(), std::string_view{"CustomPattern1D"});
}

TEST(Axis1D, RejectsEmptyFaces) {
    try {
        const Axis1D axis{{}};
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.GRID.INVALID_FACE_COUNT"}
        );
        return;
    }

    FAIL() << "Axis1D did not reject an empty face array.";
}

TEST(Axis1D, RejectsSingleFace) {
    try {
        const Axis1D axis{{0.0}};
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.GRID.INVALID_FACE_COUNT"}
        );
        return;
    }

    FAIL() << "Axis1D did not reject a single face.";
}

TEST(Axis1D, RejectsRepeatedFaces) {
    try {
        const Axis1D axis{{0.0, 0.5, 0.5, 1.0}};
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.GRID.NON_INCREASING_FACES"}
        );
        return;
    }

    FAIL() << "Axis1D did not reject repeated faces.";
}

TEST(Axis1D, RejectsDecreasingFaces) {
    try {
        const Axis1D axis{{0.0, 1.0, 0.5}};
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.GRID.NON_INCREASING_FACES"}
        );
        return;
    }

    FAIL() << "Axis1D did not reject decreasing faces.";
}

}  // namespace fvgrid