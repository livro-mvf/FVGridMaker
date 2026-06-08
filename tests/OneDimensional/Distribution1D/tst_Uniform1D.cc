// ----------------------------------------------------------------------------
// File: tst_Uniform1D.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Tests uniform one-dimensional axis generation utilities.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <string_view>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/FVGridException.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/FaceCentered1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

TEST(Uniform1D, BuildsUniformAxisWithDefaultPattern) {
    const Axis1D axis = Uniform1D::make(
        NVol{4},
        Length{1.0},
        XInit{0.0}
    );

    EXPECT_EQ(axis.num_cells(), static_cast<Size>(4));
    EXPECT_EQ(axis.num_faces(), static_cast<Size>(5));
    EXPECT_EQ(axis.pattern_name(), VolumeCentered1D::name());
}

TEST(Uniform1D, ComputesUniformFaces) {
    const Axis1D axis = Uniform1D::make(
        NVol{4},
        Length{1.0},
        XInit{0.0}
    );

    ASSERT_EQ(axis.faces().size(), static_cast<Size>(5));
    EXPECT_DOUBLE_EQ(axis.faces()[0], 0.0);
    EXPECT_DOUBLE_EQ(axis.faces()[1], 0.25);
    EXPECT_DOUBLE_EQ(axis.faces()[2], 0.5);
    EXPECT_DOUBLE_EQ(axis.faces()[3], 0.75);
    EXPECT_DOUBLE_EQ(axis.faces()[4], 1.0);
}

TEST(Uniform1D, ComputesUniformCenters) {
    const Axis1D axis = Uniform1D::make(
        NVol{4},
        Length{1.0},
        XInit{0.0}
    );

    ASSERT_EQ(axis.centers().size(), static_cast<Size>(4));
    EXPECT_DOUBLE_EQ(axis.centers()[0], 0.125);
    EXPECT_DOUBLE_EQ(axis.centers()[1], 0.375);
    EXPECT_DOUBLE_EQ(axis.centers()[2], 0.625);
    EXPECT_DOUBLE_EQ(axis.centers()[3], 0.875);
}

TEST(Uniform1D, ComputesUniformCellLengths) {
    const Axis1D axis = Uniform1D::make(
        NVol{4},
        Length{1.0},
        XInit{0.0}
    );

    ASSERT_EQ(axis.cell_lengths().size(), static_cast<Size>(4));
    EXPECT_DOUBLE_EQ(axis.cell_lengths()[0], 0.25);
    EXPECT_DOUBLE_EQ(axis.cell_lengths()[1], 0.25);
    EXPECT_DOUBLE_EQ(axis.cell_lengths()[2], 0.25);
    EXPECT_DOUBLE_EQ(axis.cell_lengths()[3], 0.25);
}

TEST(Uniform1D, SupportsShiftedInitialCoordinate) {
    const Axis1D axis = Uniform1D::make(
        NVol{2},
        Length{4.0},
        XInit{-1.0}
    );

    ASSERT_EQ(axis.faces().size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(axis.faces()[0], -1.0);
    EXPECT_DOUBLE_EQ(axis.faces()[1], 1.0);
    EXPECT_DOUBLE_EQ(axis.faces()[2], 3.0);

    ASSERT_EQ(axis.centers().size(), static_cast<Size>(2));
    EXPECT_DOUBLE_EQ(axis.centers()[0], 0.0);
    EXPECT_DOUBLE_EQ(axis.centers()[1], 2.0);
}

TEST(Uniform1D, SupportsExplicitPatternName) {
    const Axis1D axis = Uniform1D::make(
        NVol{4},
        Length{1.0},
        XInit{0.0},
        FaceCentered1D::name()
    );

    EXPECT_EQ(axis.pattern_name(), FaceCentered1D::name());
}

TEST(Uniform1D, SupportsPatternObject) {
    const Axis1D axis = Uniform1D::make(
        NVol{4},
        Length{1.0},
        XInit{0.0},
        FaceCentered1D{}
    );

    EXPECT_EQ(axis.pattern_name(), FaceCentered1D::name());
}

TEST(Uniform1D, FreeFunctionUsesDefaultPattern) {
    const Axis1D axis = uniform_axis_1d(
        NVol{4},
        Length{1.0},
        XInit{0.0}
    );

    EXPECT_EQ(axis.num_cells(), static_cast<Size>(4));
    EXPECT_EQ(axis.pattern_name(), VolumeCentered1D::name());
}

TEST(Uniform1D, FreeFunctionSupportsExplicitPatternName) {
    const Axis1D axis = uniform_axis_1d(
        NVol{4},
        Length{1.0},
        XInit{0.0},
        FaceCentered1D::name()
    );

    EXPECT_EQ(axis.pattern_name(), FaceCentered1D::name());
}

TEST(Uniform1D, FreeFunctionSupportsPatternObject) {
    const Axis1D axis = uniform_axis_1d(
        NVol{4},
        Length{1.0},
        XInit{0.0},
        FaceCentered1D{}
    );

    EXPECT_EQ(axis.pattern_name(), FaceCentered1D::name());
}

TEST(Uniform1D, RejectsZeroVolumes) {
    try {
        const Axis1D axis = Uniform1D::make(
            NVol{0},
            Length{1.0},
            XInit{0.0}
        );
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.GRID.INVALID_NVOL"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Grid"});
        EXPECT_EQ(exception.record().source.class_name(), std::string_view{"Uniform1D"});
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.distribution.Uniform1D"}
        );
        return;
    }

    FAIL() << "Uniform1D did not reject zero volumes.";
}

TEST(Uniform1D, RejectsZeroLength) {
    try {
        const Axis1D axis = Uniform1D::make(
            NVol{4},
            Length{0.0},
            XInit{0.0}
        );
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.GRID.INVALID_LENGTH"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Grid"});
        EXPECT_EQ(exception.record().source.class_name(), std::string_view{"Uniform1D"});
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.distribution.Uniform1D"}
        );
        return;
    }

    FAIL() << "Uniform1D did not reject zero length.";
}

TEST(Uniform1D, RejectsNegativeLength) {
    try {
        const Axis1D axis = Uniform1D::make(
            NVol{4},
            Length{-1.0},
            XInit{0.0}
        );
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.GRID.INVALID_LENGTH"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Grid"});
        EXPECT_EQ(exception.record().source.class_name(), std::string_view{"Uniform1D"});
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.distribution.Uniform1D"}
        );
        return;
    }

    FAIL() << "Uniform1D did not reject negative length.";
}

TEST(Axis1D, StreamsFormattedAxis) {
    const Axis1D axis{{0.0, 0.5, 1.0}};

    std::ostringstream stream;
    stream << axis;

    const std::string output = stream.str();

    EXPECT_NE(output.find("Axis1D"), std::string::npos);
    EXPECT_NE(output.find("xface[i]"), std::string::npos);
    EXPECT_NE(output.find("xcenter[i]"), std::string::npos);
    EXPECT_NE(output.find("dxface[i]"), std::string::npos);
    EXPECT_NE(output.find("dxcenter[i]"), std::string::npos);
}

}  // namespace fvgrid