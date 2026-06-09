// ----------------------------------------------------------------------------
// File: tst_StructuredGrid2D.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Tests two-dimensional rectilinear structured FV grids.
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
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

TEST(StructuredGrid2D, StoresClassIdentity) {
    EXPECT_EQ(
        StructuredGrid2D::id().module(),
        std::string_view{"TwoDimensional"}
    );

    EXPECT_EQ(
        StructuredGrid2D::id().class_name(),
        std::string_view{"StructuredGrid2D"}
    );

    EXPECT_EQ(
        StructuredGrid2D::id().class_id(),
        std::string_view{"fvgrid.2d.grid.StructuredGrid2D"}
    );

    EXPECT_EQ(
        StructuredGrid2D::class_name(),
        std::string_view{"StructuredGrid2D"}
    );

    EXPECT_EQ(
        StructuredGrid2D::class_id(),
        std::string_view{"fvgrid.2d.grid.StructuredGrid2D"}
    );
}

TEST(StructuredGrid2D, BuildsFromTwoAxes) {
    const Axis1D x_axis{
        std::vector<Real>{0.0, 1.0, 3.0}
    };

    const Axis1D y_axis{
        std::vector<Real>{0.0, 2.0, 5.0, 6.0}
    };

    const StructuredGrid2D grid{x_axis, y_axis};

    EXPECT_EQ(grid.num_cells_x(), static_cast<Size>(2));
    EXPECT_EQ(grid.num_cells_y(), static_cast<Size>(3));
    EXPECT_EQ(grid.num_cells(), static_cast<Size>(6));

    EXPECT_EQ(grid.num_faces_x(), static_cast<Size>(3));
    EXPECT_EQ(grid.num_faces_y(), static_cast<Size>(4));
}

TEST(StructuredGrid2D, ReportsDomainBoundsAndLengths) {
    const Axis1D x_axis{
        std::vector<Real>{-1.0, 0.0, 2.0}
    };

    const Axis1D y_axis{
        std::vector<Real>{10.0, 11.0, 14.0}
    };

    const StructuredGrid2D grid{x_axis, y_axis};

    EXPECT_DOUBLE_EQ(grid.xmin(), -1.0);
    EXPECT_DOUBLE_EQ(grid.xmax(), 2.0);
    EXPECT_DOUBLE_EQ(grid.ymin(), 10.0);
    EXPECT_DOUBLE_EQ(grid.ymax(), 14.0);

    EXPECT_DOUBLE_EQ(grid.length_x(), 3.0);
    EXPECT_DOUBLE_EQ(grid.length_y(), 4.0);
}

TEST(StructuredGrid2D, UsesRowMajorLinearIndexing) {
    const Axis1D x_axis{
        std::vector<Real>{0.0, 1.0, 2.0, 3.0}
    };

    const Axis1D y_axis{
        std::vector<Real>{0.0, 10.0, 20.0}
    };

    const StructuredGrid2D grid{x_axis, y_axis};

    EXPECT_EQ(grid.linear_cell_index(0, 0), static_cast<Size>(0));
    EXPECT_EQ(grid.linear_cell_index(1, 0), static_cast<Size>(1));
    EXPECT_EQ(grid.linear_cell_index(2, 0), static_cast<Size>(2));

    EXPECT_EQ(grid.linear_cell_index(0, 1), static_cast<Size>(3));
    EXPECT_EQ(grid.linear_cell_index(1, 1), static_cast<Size>(4));
    EXPECT_EQ(grid.linear_cell_index(2, 1), static_cast<Size>(5));
}

TEST(StructuredGrid2D, ProvidesFaceAndCentreCoordinates) {
    const Axis1D x_axis{
        std::vector<Real>{0.0, 1.0, 3.0}
    };

    const Axis1D y_axis{
        std::vector<Real>{0.0, 2.0, 5.0, 6.0}
    };

    const StructuredGrid2D grid{x_axis, y_axis};

    EXPECT_DOUBLE_EQ(grid.x_face(0), 0.0);
    EXPECT_DOUBLE_EQ(grid.x_face(1), 1.0);
    EXPECT_DOUBLE_EQ(grid.x_face(2), 3.0);

    EXPECT_DOUBLE_EQ(grid.y_face(0), 0.0);
    EXPECT_DOUBLE_EQ(grid.y_face(1), 2.0);
    EXPECT_DOUBLE_EQ(grid.y_face(2), 5.0);
    EXPECT_DOUBLE_EQ(grid.y_face(3), 6.0);

    EXPECT_DOUBLE_EQ(grid.x_center(0), 0.5);
    EXPECT_DOUBLE_EQ(grid.x_center(1), 2.0);

    EXPECT_DOUBLE_EQ(grid.y_center(0), 1.0);
    EXPECT_DOUBLE_EQ(grid.y_center(1), 3.5);
    EXPECT_DOUBLE_EQ(grid.y_center(2), 5.5);
}

TEST(StructuredGrid2D, ComputesCellLengths) {
    const Axis1D x_axis{
        std::vector<Real>{0.0, 1.0, 3.0}
    };

    const Axis1D y_axis{
        std::vector<Real>{0.0, 2.0, 5.0, 6.0}
    };

    const StructuredGrid2D grid{x_axis, y_axis};

    EXPECT_DOUBLE_EQ(grid.x_cell_length(0), 1.0);
    EXPECT_DOUBLE_EQ(grid.x_cell_length(1), 2.0);

    EXPECT_DOUBLE_EQ(grid.y_cell_length(0), 2.0);
    EXPECT_DOUBLE_EQ(grid.y_cell_length(1), 3.0);
    EXPECT_DOUBLE_EQ(grid.y_cell_length(2), 1.0);
}

TEST(StructuredGrid2D, ComputesCartesianCellAreas) {
    const Axis1D x_axis{
        std::vector<Real>{0.0, 1.0, 3.0}
    };

    const Axis1D y_axis{
        std::vector<Real>{0.0, 2.0, 5.0, 6.0}
    };

    const StructuredGrid2D grid{x_axis, y_axis};

    EXPECT_DOUBLE_EQ(grid.cell_area(0, 0), 2.0);
    EXPECT_DOUBLE_EQ(grid.cell_area(1, 0), 4.0);

    EXPECT_DOUBLE_EQ(grid.cell_area(0, 1), 3.0);
    EXPECT_DOUBLE_EQ(grid.cell_area(1, 1), 6.0);

    EXPECT_DOUBLE_EQ(grid.cell_area(0, 2), 1.0);
    EXPECT_DOUBLE_EQ(grid.cell_area(1, 2), 2.0);
}

TEST(StructuredGrid2D, ComputesCartesianFaceLengths) {
    const Axis1D x_axis{
        std::vector<Real>{0.0, 1.0, 3.0}
    };

    const Axis1D y_axis{
        std::vector<Real>{0.0, 2.0, 5.0, 6.0}
    };

    const StructuredGrid2D grid{x_axis, y_axis};

    EXPECT_DOUBLE_EQ(grid.vertical_face_length(0), 2.0);
    EXPECT_DOUBLE_EQ(grid.vertical_face_length(1), 3.0);
    EXPECT_DOUBLE_EQ(grid.vertical_face_length(2), 1.0);

    EXPECT_DOUBLE_EQ(grid.horizontal_face_length(0), 1.0);
    EXPECT_DOUBLE_EQ(grid.horizontal_face_length(1), 2.0);
}

TEST(StructuredGrid2D, GivesAccessToInputAxes) {
    const Axis1D x_axis{
        std::vector<Real>{0.0, 1.0, 3.0}
    };

    const Axis1D y_axis{
        std::vector<Real>{0.0, 2.0, 5.0, 6.0}
    };

    const StructuredGrid2D grid{x_axis, y_axis};

    EXPECT_DOUBLE_EQ(grid.x_axis().xmin(), 0.0);
    EXPECT_DOUBLE_EQ(grid.x_axis().xmax(), 3.0);

    EXPECT_DOUBLE_EQ(grid.y_axis().xmin(), 0.0);
    EXPECT_DOUBLE_EQ(grid.y_axis().xmax(), 6.0);
}

TEST(StructuredGrid2D, RejectsInvalidCellIndex) {
    const Axis1D x_axis{
        std::vector<Real>{0.0, 1.0, 3.0}
    };

    const Axis1D y_axis{
        std::vector<Real>{0.0, 2.0, 5.0, 6.0}
    };

    const StructuredGrid2D grid{x_axis, y_axis};

    try {
        [[maybe_unused]] const Size index =
            grid.linear_cell_index(static_cast<Size>(2), static_cast<Size>(0));
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.CORE.OUT_OF_RANGE"}
        );

        EXPECT_EQ(exception.record().category, std::string_view{"Core"});

        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"StructuredGrid2D"}
        );

        return;
    }

    FAIL() << "StructuredGrid2D accepted an invalid cell index.";
}

TEST(StructuredGrid2D, RejectsInvalidFaceIndex) {
    const Axis1D x_axis{
        std::vector<Real>{0.0, 1.0, 3.0}
    };

    const Axis1D y_axis{
        std::vector<Real>{0.0, 2.0, 5.0, 6.0}
    };

    const StructuredGrid2D grid{x_axis, y_axis};

    try {
        [[maybe_unused]] const Real face =
            grid.x_face(static_cast<Size>(3));
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.CORE.OUT_OF_RANGE"}
        );

        EXPECT_EQ(exception.record().category, std::string_view{"Core"});

        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"StructuredGrid2D"}
        );

        return;
    }

    FAIL() << "StructuredGrid2D accepted an invalid face index.";
}

}  // namespace fvgrid