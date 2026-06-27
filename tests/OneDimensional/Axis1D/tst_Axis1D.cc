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
#include <string>
#include <string_view>
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/FVGridException.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/AxisGeometry1D.h>
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

TEST(Axis1D, ComputesCentersFromFacesUsingVolumeCenteredDefault) {
    const Axis1D axis{{0.0, 0.5, 1.0, 2.0}};

    ASSERT_EQ(axis.centers().size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(axis.centers()[0], 0.25);
    EXPECT_DOUBLE_EQ(axis.centers()[1], 0.75);
    EXPECT_DOUBLE_EQ(axis.centers()[2], 1.5);
}

TEST(Axis1D, ComputesDxFaces) {
    const Axis1D axis{{0.0, 0.5, 1.0, 2.0}};

    ASSERT_EQ(axis.dx_faces().size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(axis.dx_faces()[0], 0.5);
    EXPECT_DOUBLE_EQ(axis.dx_faces()[1], 0.5);
    EXPECT_DOUBLE_EQ(axis.dx_faces()[2], 1.0);
}

TEST(Axis1D, CellLengthsAliasDxFaces) {
    const Axis1D axis{{0.0, 0.5, 1.0, 2.0}};

    ASSERT_EQ(axis.cell_lengths().size(), axis.dx_faces().size());

    for (Size i = 0; i < axis.dx_faces().size(); ++i) {
        EXPECT_DOUBLE_EQ(axis.cell_lengths()[i], axis.dx_faces()[i]);
    }
}

TEST(Axis1D, ComputesDxCenters) {
    const Axis1D axis{{0.0, 0.5, 1.0, 2.0}};

    ASSERT_EQ(axis.dx_centers().size(), static_cast<Size>(4));
    EXPECT_DOUBLE_EQ(axis.dx_centers()[0], 0.25);
    EXPECT_DOUBLE_EQ(axis.dx_centers()[1], 0.5);
    EXPECT_DOUBLE_EQ(axis.dx_centers()[2], 0.75);
    EXPECT_DOUBLE_EQ(axis.dx_centers()[3], 0.5);
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

TEST(Axis1D, DetectsVolumeCenteredPatternByType) {
    const Axis1D axis{{0.0, 0.5, 1.0}};

    EXPECT_TRUE(axis.has_pattern<VolumeCentered1D>());
    EXPECT_FALSE(axis.has_pattern<FaceCentered1D>());
}

TEST(Axis1D, StoresExplicitFaceCenteredPatternName) {
    const Axis1D axis{
        {0.0, 0.5, 1.0},
        {0.25, 0.75},
        std::string{FaceCentered1D::name()}
    };

    EXPECT_EQ(axis.pattern_name(), FaceCentered1D::name());
}

TEST(Axis1D, DetectsExplicitFaceCenteredPatternByType) {
    const Axis1D axis{
        {0.0, 0.5, 1.0},
        {0.25, 0.75},
        std::string{FaceCentered1D::name()}
    };

    EXPECT_TRUE(axis.has_pattern<FaceCentered1D>());
    EXPECT_FALSE(axis.has_pattern<VolumeCentered1D>());
}

TEST(Axis1D, StoresCustomPatternName) {
    const Axis1D axis{
        {0.0, 0.5, 1.0},
        {0.25, 0.75},
        "CustomPattern1D"
    };

    EXPECT_EQ(axis.pattern_name(), std::string_view{"CustomPattern1D"});
}

TEST(Axis1D, RejectsEmptyPatternName) {
    try {
        const Axis1D axis{
            {0.0, 0.5, 1.0},
            {0.25, 0.75},
            ""
        };
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.CORE.INVALID_ARGUMENT"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Core"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"Axis1D"}
        );
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.axis.Axis1D"}
        );
        return;
    }

    FAIL() << "Axis1D did not reject an empty pattern name.";
}

TEST(Axis1D, RejectsEmptyPatternNameFromAxisGeometry) {
    try {
        AxisGeometry1D geometry{
            std::vector<Real>{0.0, 0.5, 1.0},
            std::vector<Real>{0.2, 0.8},
            ""
        };

        const Axis1D axis = Axis1D::from_geometry(std::move(geometry));
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.CORE.INVALID_ARGUMENT"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Core"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"Axis1D"}
        );
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.axis.Axis1D"}
        );
        return;
    }

    FAIL() << "Axis1D did not reject an empty pattern name from geometry.";
}

TEST(Axis1D, RejectsBuiltInPatternQueriesForCustomPatternName) {
    const Axis1D axis{
        {0.0, 0.5, 1.0},
        {0.25, 0.75},
        "CustomPattern1D"
    };

    EXPECT_FALSE(axis.has_pattern<VolumeCentered1D>());
    EXPECT_FALSE(axis.has_pattern<FaceCentered1D>());
}

TEST(Axis1D, OwnsDynamicPatternName) {
    Axis1D axis = [] {
        std::string dynamic_name{"DynamicPattern1D"};

        return Axis1D{
            {0.0, 0.5, 1.0},
            {0.25, 0.75},
            dynamic_name
        };
    }();

    EXPECT_EQ(axis.pattern_name(), std::string_view{"DynamicPattern1D"});
}

TEST(Axis1D, StoresClassIdentity) {
    EXPECT_EQ(Axis1D::id().module(), std::string_view{"OneDimensional"});
    EXPECT_EQ(Axis1D::id().class_name(), std::string_view{"Axis1D"});
    EXPECT_EQ(
        Axis1D::id().class_id(),
        std::string_view{"fvgrid.1d.axis.Axis1D"}
    );

    EXPECT_EQ(Axis1D::class_name(), std::string_view{"Axis1D"});
    EXPECT_EQ(
        Axis1D::class_id(),
        std::string_view{"fvgrid.1d.axis.Axis1D"}
    );
}

TEST(Axis1D, BuildsFromCompleteFacesAndCenters) {
    const Axis1D axis{
        {0.0, 0.5, 1.0},
        {0.2, 0.8},
        "CustomPattern1D"
    };

    EXPECT_EQ(axis.num_faces(), static_cast<Size>(3));
    EXPECT_EQ(axis.num_cells(), static_cast<Size>(2));
    EXPECT_EQ(axis.pattern_name(), std::string_view{"CustomPattern1D"});

    ASSERT_EQ(axis.faces().size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(axis.faces()[0], 0.0);
    EXPECT_DOUBLE_EQ(axis.faces()[1], 0.5);
    EXPECT_DOUBLE_EQ(axis.faces()[2], 1.0);

    ASSERT_EQ(axis.centers().size(), static_cast<Size>(2));
    EXPECT_DOUBLE_EQ(axis.centers()[0], 0.2);
    EXPECT_DOUBLE_EQ(axis.centers()[1], 0.8);
}

TEST(Axis1D, BuildsFromAxisGeometry) {
    AxisGeometry1D geometry{
        std::vector<Real>{0.0, 0.5, 1.0},
        std::vector<Real>{0.2, 0.8},
        "CustomPattern1D"
    };

    const Axis1D axis = Axis1D::from_geometry(std::move(geometry));

    EXPECT_EQ(axis.num_faces(), static_cast<Size>(3));
    EXPECT_EQ(axis.num_cells(), static_cast<Size>(2));
    EXPECT_EQ(axis.pattern_name(), std::string_view{"CustomPattern1D"});

    ASSERT_EQ(axis.faces().size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(axis.faces()[0], 0.0);
    EXPECT_DOUBLE_EQ(axis.faces()[1], 0.5);
    EXPECT_DOUBLE_EQ(axis.faces()[2], 1.0);

    ASSERT_EQ(axis.centers().size(), static_cast<Size>(2));
    EXPECT_DOUBLE_EQ(axis.centers()[0], 0.2);
    EXPECT_DOUBLE_EQ(axis.centers()[1], 0.8);
}

TEST(Axis1D, ComputesMetricsFromCompleteFacesAndCenters) {
    const Axis1D axis{
        {0.0, 0.5, 1.0},
        {0.2, 0.8},
        "CustomPattern1D"
    };

    ASSERT_EQ(axis.dx_faces().size(), static_cast<Size>(2));
    EXPECT_DOUBLE_EQ(axis.dx_faces()[0], 0.5);
    EXPECT_DOUBLE_EQ(axis.dx_faces()[1], 0.5);

    ASSERT_EQ(axis.dx_centers().size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(axis.dx_centers()[0], 0.2);
    EXPECT_DOUBLE_EQ(axis.dx_centers()[1], 0.6);
    EXPECT_DOUBLE_EQ(axis.dx_centers()[2], 0.2);
}

TEST(Axis1D, ProvidesScalarCoordinateAccessors) {
    const Axis1D axis{{0.0, 0.5, 1.0, 2.0}};

    EXPECT_DOUBLE_EQ(axis.face(0), 0.0);
    EXPECT_DOUBLE_EQ(axis.face(1), 0.5);
    EXPECT_DOUBLE_EQ(axis.face(2), 1.0);
    EXPECT_DOUBLE_EQ(axis.face(3), 2.0);

    EXPECT_DOUBLE_EQ(axis.center(0), 0.25);
    EXPECT_DOUBLE_EQ(axis.center(1), 0.75);
    EXPECT_DOUBLE_EQ(axis.center(2), 1.5);
}

TEST(Axis1D, ProvidesCellFaceAccessors) {
    const Axis1D axis{{0.0, 0.5, 1.0, 2.0}};

    EXPECT_DOUBLE_EQ(axis.west_face(0), 0.0);
    EXPECT_DOUBLE_EQ(axis.east_face(0), 0.5);

    EXPECT_DOUBLE_EQ(axis.west_face(1), 0.5);
    EXPECT_DOUBLE_EQ(axis.east_face(1), 1.0);

    EXPECT_DOUBLE_EQ(axis.west_face(2), 1.0);
    EXPECT_DOUBLE_EQ(axis.east_face(2), 2.0);
}

TEST(Axis1D, ProvidesFiniteVolumeDistanceAccessors) {
    const Axis1D axis{{0.0, 0.5, 1.0, 2.0}};

    EXPECT_DOUBLE_EQ(axis.cell_length(0), 0.5);
    EXPECT_DOUBLE_EQ(axis.cell_length(1), 0.5);
    EXPECT_DOUBLE_EQ(axis.cell_length(2), 1.0);

    EXPECT_DOUBLE_EQ(axis.center_distance(0), 0.25);
    EXPECT_DOUBLE_EQ(axis.center_distance(1), 0.5);
    EXPECT_DOUBLE_EQ(axis.center_distance(2), 0.75);
    EXPECT_DOUBLE_EQ(axis.center_distance(3), 0.5);

    EXPECT_DOUBLE_EQ(axis.DxP(0), 0.5);
    EXPECT_DOUBLE_EQ(axis.DxP(1), 0.5);
    EXPECT_DOUBLE_EQ(axis.DxP(2), 1.0);

    EXPECT_DOUBLE_EQ(axis.DxW(1), 0.5);
    EXPECT_DOUBLE_EQ(axis.DxW(2), 0.5);

    EXPECT_DOUBLE_EQ(axis.DxE(0), 0.5);
    EXPECT_DOUBLE_EQ(axis.DxE(1), 1.0);

    EXPECT_DOUBLE_EQ(axis.deltaxw(1), 0.5);
    EXPECT_DOUBLE_EQ(axis.deltaxw(2), 0.75);

    EXPECT_DOUBLE_EQ(axis.deltaxe(0), 0.5);
    EXPECT_DOUBLE_EQ(axis.deltaxe(1), 0.75);
}

TEST(Axis1D, RejectsEmptyFacesInVolumeCenteredReconstruction) {
    try {
        const Axis1D axis{std::vector<Real>{}};
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.GRID.INVALID_FACE_COUNT"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Grid"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"VolumeCentered1D"}
        );
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.grid_pattern.VolumeCentered1D"}
        );
        return;
    }

    FAIL() << "Axis1D did not reject an empty face array.";
}

TEST(Axis1D, RejectsSingleFaceInVolumeCenteredReconstruction) {
    try {
        const Axis1D axis{{0.0}};
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.GRID.INVALID_FACE_COUNT"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Grid"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"VolumeCentered1D"}
        );
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.grid_pattern.VolumeCentered1D"}
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
        EXPECT_EQ(exception.record().category, std::string_view{"Grid"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"Axis1D"}
        );
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.axis.Axis1D"}
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
        EXPECT_EQ(exception.record().category, std::string_view{"Grid"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"Axis1D"}
        );
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.axis.Axis1D"}
        );
        return;
    }

    FAIL() << "Axis1D did not reject decreasing faces.";
}

TEST(Axis1D, RejectsWrongCenterCount) {
    try {
        const Axis1D axis{
            {0.0, 0.5, 1.0},
            {0.25, 0.75, 0.95},
            "CustomPattern1D"
        };
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.GRID.INVALID_CENTER_COUNT"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Grid"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"Axis1D"}
        );
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.axis.Axis1D"}
        );
        return;
    }

    FAIL() << "Axis1D did not reject a wrong centre count.";
}

TEST(Axis1D, RejectsRepeatedCenters) {
    try {
        const Axis1D axis{
            {0.0, 0.5, 1.0},
            {0.25, 0.25},
            "CustomPattern1D"
        };
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.GRID.NON_INCREASING_CENTERS"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Grid"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"Axis1D"}
        );
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.axis.Axis1D"}
        );
        return;
    }

    FAIL() << "Axis1D did not reject repeated centres.";
}

TEST(Axis1D, RejectsDecreasingCenters) {
    try {
        const Axis1D axis{
            {0.0, 0.5, 1.0},
            {0.75, 0.25},
            "CustomPattern1D"
        };
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.GRID.NON_INCREASING_CENTERS"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Grid"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"Axis1D"}
        );
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.axis.Axis1D"}
        );
        return;
    }

    FAIL() << "Axis1D did not reject decreasing centres.";
}

TEST(Axis1D, RejectsFirstCenterOutsideLeftBoundary) {
    try {
        const Axis1D axis{
            {0.0, 0.5, 1.0},
            {-0.1, 0.75},
            "CustomPattern1D"
        };
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.CORE.OUT_OF_RANGE"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Core"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"Axis1D"}
        );
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.axis.Axis1D"}
        );
        return;
    }

    FAIL() << "Axis1D did not reject a centre outside the left boundary.";
}

TEST(Axis1D, RejectsLastCenterOutsideRightBoundary) {
    try {
        const Axis1D axis{
            {0.0, 0.5, 1.0},
            {0.25, 1.1},
            "CustomPattern1D"
        };
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.CORE.OUT_OF_RANGE"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Core"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"Axis1D"}
        );
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.axis.Axis1D"}
        );
        return;
    }

    FAIL() << "Axis1D did not reject a centre outside the right boundary.";
}

}  // namespace fvgrid