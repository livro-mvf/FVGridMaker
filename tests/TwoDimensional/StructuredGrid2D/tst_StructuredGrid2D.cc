// ----------------------------------------------------------------------------
// File: tst_StructuredGrid2D.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Tests two-dimensional rectilinear structured FV grids.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------
#include <cmath>
#include <numbers>
#include <sstream>
#include <string>

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
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/FaceCentered1D.h>
#include <FVGridMaker/TwoDimensional/CoordinateSystem2D/CoordinateSystem2D.h>
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

TEST(StructuredGrid2D, BuildsPolarGeometryFromTwoIndependentAxes) {
    const Axis1D radius{std::vector<Real>{1.0, 2.0}};
    const Axis1D angle{std::vector<Real>{0.0, std::numbers::pi / 2.0}};
    const StructuredGrid2D grid{radius, angle, PolarCoordinates2D{}};

    const PhysicalPoint2D point = grid.physical_face_point(1, 1);
    EXPECT_NEAR(point.x, 0.0, 1.0e-14);
    EXPECT_NEAR(point.y, 2.0, 1.0e-14);
    EXPECT_NEAR(grid.cell_measure(0, 0), 3.0 * std::numbers::pi / 4.0, 1.0e-14);
    EXPECT_EQ(grid.coordinate_system_name(), std::string_view{"Polar"});
}

TEST(StructuredGrid2D, AcceptsUserDefinedCoordinateTraits) {
    const Axis1D q{std::vector<Real>{0.0, 1.0}};
    const Axis1D s{std::vector<Real>{0.0, 2.0}};
    const auto mapping = make_coordinate_mapping_2d(
        "Parabolic", "q", "s",
        [](Real first, Real second) {
            return PhysicalPoint2D{first, second + first * first, 0.0};
        },
        [](CoordinateCell2D cell) {
            return (cell.first_max - cell.first_min)
                 * (cell.second_max - cell.second_min);
        }
    );
    const StructuredGrid2D grid{q, s, mapping};

    const PhysicalPoint2D point = grid.physical_face_point(1, 1);
    EXPECT_DOUBLE_EQ(point.x, 1.0);
    EXPECT_DOUBLE_EQ(point.y, 3.0);
    EXPECT_EQ(grid.coordinate_system_name(), std::string_view{"Parabolic"});
}

TEST(StructuredGrid2D, PrintsCoordinateSystemAndIndependentAxes) {
    const StructuredGrid2D grid{
        Axis1D{std::vector<Real>{0.0, 1.0}},
        Axis1D{std::vector<Real>{0.0, 2.0}}
    };
    std::ostringstream output;
    output << grid;
    EXPECT_NE(output.str().find("Cartesian"), std::string::npos);
    EXPECT_NE(
        output.str().find("first independent Axis1D"),
        std::string::npos
    );
}

TEST(StructuredGrid2D, BuildsFromDefaultGeneratedFaceCenteredAxes) {
    StructuredGrid2D grid{
        Uniform1D::make(NVol{2}, Length{2.0}, XInit{0.0}),
        Uniform1D::make(NVol{3}, Length{3.0}, XInit{0.0})
    };

    EXPECT_EQ(grid.first_axis().pattern_name(), FaceCentered1D::name());
    EXPECT_EQ(grid.second_axis().pattern_name(), FaceCentered1D::name());
    EXPECT_EQ(grid.num_cells_x(), static_cast<Size>(2));
    EXPECT_EQ(grid.num_cells_y(), static_cast<Size>(3));
}
TEST(StructuredGrid2D, BuildsFromTwoVolumeCenteredAxes) {
    const StructuredGrid2D grid{
        Axis1D{std::vector<Real>{0.0, 1.0, 2.0}},
        Axis1D{std::vector<Real>{0.0, 0.5, 1.0}}
    };

    EXPECT_EQ(grid.first_axis().pattern_name(), std::string_view{"VolumeCentered1D"});
    EXPECT_EQ(grid.second_axis().pattern_name(), std::string_view{"VolumeCentered1D"});
}

TEST(StructuredGrid2D, RejectsIncompatibleAxisPatterns) {
    const Axis1D volume_centered{std::vector<Real>{0.0, 1.0, 2.0}};
    const Axis1D face_centered_like{
        std::vector<Real>{0.0, 1.0, 2.0},
        std::vector<Real>{0.5, 1.5},
        std::string{FaceCentered1D::name()}
    };

    try {
        [[maybe_unused]] const StructuredGrid2D grid{
            volume_centered,
            face_centered_like
        };
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.OPERATION.INCOMPATIBLE_GRID_PATTERNS"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Operation"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"StructuredGrid2D"}
        );
        return;
    }

    FAIL() << "StructuredGrid2D accepted incompatible axis patterns.";
}

TEST(StructuredGrid2D, ProvidesLogicalAccessorsAndCartesianAliases) {
    const StructuredGrid2D grid{
        Axis1D{std::vector<Real>{0.0, 1.0, 3.0}},
        Axis1D{std::vector<Real>{0.0, 2.0, 5.0}}
    };

    EXPECT_DOUBLE_EQ(grid.first_face(2), 3.0);
    EXPECT_DOUBLE_EQ(grid.second_face(2), 5.0);
    EXPECT_DOUBLE_EQ(grid.first_center(1), 2.0);
    EXPECT_DOUBLE_EQ(grid.second_center(1), 3.5);
    EXPECT_DOUBLE_EQ(grid.first_cell_length(1), 2.0);
    EXPECT_DOUBLE_EQ(grid.second_cell_length(1), 3.0);

    EXPECT_DOUBLE_EQ(grid.x_face(2), grid.first_face(2));
    EXPECT_DOUBLE_EQ(grid.y_face(2), grid.second_face(2));
    EXPECT_DOUBLE_EQ(grid.x_center(1), grid.first_center(1));
    EXPECT_DOUBLE_EQ(grid.y_center(1), grid.second_center(1));
    EXPECT_DOUBLE_EQ(grid.x_cell_length(1), grid.first_cell_length(1));
    EXPECT_DOUBLE_EQ(grid.y_cell_length(1), grid.second_cell_length(1));
}

TEST(StructuredGrid2D, RejectsOutOfBoundsLogicalAccessors) {
    const StructuredGrid2D grid{
        Axis1D{std::vector<Real>{0.0, 1.0}},
        Axis1D{std::vector<Real>{0.0, 1.0}}
    };

    EXPECT_THROW((void)grid.first_face(2), FVGridException);
    EXPECT_THROW((void)grid.second_face(2), FVGridException);
    EXPECT_THROW((void)grid.first_center(1), FVGridException);
    EXPECT_THROW((void)grid.second_center(1), FVGridException);
}

TEST(StructuredGrid2D, ProvidesExplicitCartesianPhysicalPoints) {
    const StructuredGrid2D grid{
        Axis1D{std::vector<Real>{0.0, 1.0}},
        Axis1D{std::vector<Real>{0.0, 2.0}}
    };

    const PhysicalPoint2D vertex = grid.physical_vertex(1, 1);
    EXPECT_DOUBLE_EQ(vertex.x, 1.0);
    EXPECT_DOUBLE_EQ(vertex.y, 2.0);
    EXPECT_DOUBLE_EQ(vertex.z, 0.0);

    const PhysicalPoint2D center = grid.physical_cell_center(0, 0);
    EXPECT_DOUBLE_EQ(center.x, 0.5);
    EXPECT_DOUBLE_EQ(center.y, 1.0);
    EXPECT_DOUBLE_EQ(center.z, 0.0);

    const PhysicalPoint2D first_face_center =
        grid.physical_first_face_center(1, 0);
    EXPECT_DOUBLE_EQ(first_face_center.x, 1.0);
    EXPECT_DOUBLE_EQ(first_face_center.y, 1.0);

    const PhysicalPoint2D second_face_center =
        grid.physical_second_face_center(0, 1);
    EXPECT_DOUBLE_EQ(second_face_center.x, 0.5);
    EXPECT_DOUBLE_EQ(second_face_center.y, 2.0);
}

TEST(StructuredGrid2D, ProvidesExplicitPolarPhysicalPoints) {
    const StructuredGrid2D grid{
        Axis1D{std::vector<Real>{1.0, 3.0}},
        Axis1D{std::vector<Real>{0.0, std::numbers::pi / 2.0}},
        PolarCoordinates2D{}
    };

    const PhysicalPoint2D center = grid.physical_cell_center(0, 0);
    EXPECT_NEAR(center.x, std::numbers::sqrt2, 1.0e-14);
    EXPECT_NEAR(center.y, std::numbers::sqrt2, 1.0e-14);

    const PhysicalPoint2D first_face_center =
        grid.physical_first_face_center(0, 0);
    EXPECT_NEAR(first_face_center.x, std::numbers::sqrt2 / 2.0, 1.0e-14);
    EXPECT_NEAR(first_face_center.y, std::numbers::sqrt2 / 2.0, 1.0e-14);

    const PhysicalPoint2D second_face_center =
        grid.physical_second_face_center(0, 1);
    EXPECT_NEAR(second_face_center.x, 0.0, 1.0e-14);
    EXPECT_NEAR(second_face_center.y, 2.0, 1.0e-14);
}

TEST(StructuredGrid2D, OwnsFunctionalMappingNames) {
    const StructuredGrid2D grid = [] {
        std::string system_name = "Warped";
        std::string first_name = "q";
        std::string second_name = "s";
        const auto mapping = make_coordinate_mapping_2d(
            system_name,
            first_name,
            second_name,
            [](Real first, Real second) {
                return PhysicalPoint2D{first, second + first, 0.0};
            },
            [](CoordinateCell2D cell) {
                return (cell.first_max - cell.first_min)
                     * (cell.second_max - cell.second_min);
            }
        );

        return StructuredGrid2D{
            Axis1D{std::vector<Real>{0.0, 1.0}},
            Axis1D{std::vector<Real>{0.0, 1.0}},
            mapping
        };
    }();

    EXPECT_EQ(grid.coordinate_system_name(), std::string_view{"Warped"});
    EXPECT_EQ(grid.first_coordinate_name(), std::string_view{"q"});
    EXPECT_EQ(grid.second_coordinate_name(), std::string_view{"s"});
    EXPECT_DOUBLE_EQ(grid.physical_vertex(1, 1).y, 2.0);
}
}  // namespace fvgrid
