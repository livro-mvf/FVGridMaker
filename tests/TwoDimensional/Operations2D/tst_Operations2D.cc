#include <string>
#include <string_view>
#include <vector>

#include <gtest/gtest.h>

#include <FVGridMaker/ErrorHandling/FVGridException.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/FaceCentered1D.h>
#include <FVGridMaker/TwoDimensional/CoordinateSystem2D/CoordinateSystem2D.h>
#include <FVGridMaker/TwoDimensional/Operations2D/Operations2D.h>
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

namespace fvgrid {

namespace {

Axis1D face_centered_axis() {
    return Axis1D{
        std::vector<Real>{0.0, 1.0, 2.0},
        std::vector<Real>{0.5, 1.5},
        std::string{FaceCentered1D::name()}
    };
}

}  // namespace

TEST(LogicalBox2D, EmptyBoxHasNoArea) {
    const LogicalBox2D box = LogicalBox2D::empty();

    EXPECT_TRUE(box.is_empty());
    EXPECT_FALSE(box.is_area());
    EXPECT_DOUBLE_EQ(box.computational_area(), 0.0);
}

TEST(LogicalBox2D, AreaBoxComputesLogicalArea) {
    const LogicalBox2D box = LogicalBox2D::from_intervals(
        AxisInterval1D::from_bounds(0.0, 2.0),
        AxisInterval1D::from_bounds(1.0, 4.0)
    );

    EXPECT_FALSE(box.is_empty());
    EXPECT_TRUE(box.is_area());
    EXPECT_DOUBLE_EQ(box.computational_area(), 6.0);
}

TEST(Operations2D, StoresClassIdentity) {
    EXPECT_EQ(Operations2D::id().module(), std::string_view{"TwoDimensional"});
    EXPECT_EQ(Operations2D::id().class_name(), std::string_view{"Operations2D"});
    EXPECT_EQ(
        Operations2D::id().class_id(),
        std::string_view{"fvgrid.2d.operations.Operations2D"}
    );
}

TEST(Operations2D, BuildsDomainBox) {
    const StructuredGrid2D grid{
        Axis1D{std::vector<Real>{-1.0, 0.0, 2.0}},
        Axis1D{std::vector<Real>{10.0, 12.0}}
    };

    const LogicalBox2D box = Operations2D::domain_box(grid);

    EXPECT_TRUE(box.is_area());
    EXPECT_DOUBLE_EQ(box.first_interval().lower(), -1.0);
    EXPECT_DOUBLE_EQ(box.first_interval().upper(), 2.0);
    EXPECT_DOUBLE_EQ(box.second_interval().lower(), 10.0);
    EXPECT_DOUBLE_EQ(box.second_interval().upper(), 12.0);
}

TEST(Operations2D, IntersectsOverlappingCartesianGrids) {
    const StructuredGrid2D left{
        Axis1D{std::vector<Real>{0.0, 1.0, 2.0}},
        Axis1D{std::vector<Real>{0.0, 1.0, 2.0}}
    };
    const StructuredGrid2D right{
        Axis1D{std::vector<Real>{1.0, 2.0, 3.0}},
        Axis1D{std::vector<Real>{0.5, 1.5}}
    };

    const LogicalBox2D box = Operations2D::require_box_intersection(left, right);

    EXPECT_TRUE(box.is_area());
    EXPECT_DOUBLE_EQ(box.first_interval().lower(), 1.0);
    EXPECT_DOUBLE_EQ(box.first_interval().upper(), 2.0);
    EXPECT_DOUBLE_EQ(box.second_interval().lower(), 0.5);
    EXPECT_DOUBLE_EQ(box.second_interval().upper(), 1.5);
}

TEST(Operations2D, EmptyIntersectionIsRejectedWhenAreaIsRequired) {
    const StructuredGrid2D left{
        Axis1D{std::vector<Real>{0.0, 1.0}},
        Axis1D{std::vector<Real>{0.0, 1.0}}
    };
    const StructuredGrid2D right{
        Axis1D{std::vector<Real>{2.0, 3.0}},
        Axis1D{std::vector<Real>{0.0, 1.0}}
    };

    EXPECT_THROW(
        (void)Operations2D::require_box_intersection(left, right),
        FVGridException
    );
}

TEST(Operations2D, LineIntersectionIsRejectedWhenAreaIsRequired) {
    const StructuredGrid2D left{
        Axis1D{std::vector<Real>{0.0, 1.0}},
        Axis1D{std::vector<Real>{0.0, 1.0}}
    };
    const StructuredGrid2D right{
        Axis1D{std::vector<Real>{1.0, 2.0}},
        Axis1D{std::vector<Real>{0.0, 1.0}}
    };

    const LogicalBox2D line = Operations2D::intersection(left, right);
    EXPECT_FALSE(line.is_empty());
    EXPECT_FALSE(line.is_area());
    EXPECT_THROW(
        (void)Operations2D::require_box_intersection(left, right),
        FVGridException
    );
}

TEST(Operations2D, ClipsCartesianGridToLogicalBox) {
    const StructuredGrid2D grid{
        Axis1D{std::vector<Real>{0.0, 1.0, 2.0, 3.0}},
        Axis1D{std::vector<Real>{0.0, 1.0, 2.0}}
    };
    const LogicalBox2D box = LogicalBox2D::from_intervals(
        AxisInterval1D::from_bounds(0.5, 2.5),
        AxisInterval1D::from_bounds(0.25, 1.75)
    );

    const StructuredGrid2D clipped = Operations2D::clip_to_logical_box(
        grid,
        box
    );

    ASSERT_EQ(clipped.first_axis().faces().size(), 4U);
    EXPECT_DOUBLE_EQ(clipped.first_face(0), 0.5);
    EXPECT_DOUBLE_EQ(clipped.first_face(1), 1.0);
    EXPECT_DOUBLE_EQ(clipped.first_face(2), 2.0);
    EXPECT_DOUBLE_EQ(clipped.first_face(3), 2.5);

    ASSERT_EQ(clipped.second_axis().faces().size(), 3U);
    EXPECT_DOUBLE_EQ(clipped.second_face(0), 0.25);
    EXPECT_DOUBLE_EQ(clipped.second_face(1), 1.0);
    EXPECT_DOUBLE_EQ(clipped.second_face(2), 1.75);
}

TEST(Operations2D, ClipsMappedGridWhenMappingIsSupplied) {
    const StructuredGrid2D grid{
        Axis1D{std::vector<Real>{1.0, 2.0, 3.0}},
        Axis1D{std::vector<Real>{0.0, 1.0, 2.0}},
        PolarCoordinates2D{}
    };
    const LogicalBox2D box = LogicalBox2D::from_intervals(
        AxisInterval1D::from_bounds(1.5, 2.5),
        AxisInterval1D::from_bounds(0.5, 1.5)
    );

    const StructuredGrid2D clipped = Operations2D::clip_to_logical_box(
        grid,
        box,
        PolarCoordinates2D{}
    );

    EXPECT_EQ(clipped.coordinate_system_name(), std::string_view{"Polar"});
    EXPECT_FALSE(clipped.vtk_rectilinear());
    EXPECT_DOUBLE_EQ(clipped.first_face(0), 1.5);
    EXPECT_DOUBLE_EQ(clipped.second_face(0), 0.5);
}

TEST(Operations2D, RejectsClipWithoutMappingForNonCartesianGrid) {
    const StructuredGrid2D grid{
        Axis1D{std::vector<Real>{1.0, 2.0}},
        Axis1D{std::vector<Real>{0.0, 1.0}},
        PolarCoordinates2D{}
    };
    const LogicalBox2D box = Operations2D::domain_box(grid);

    EXPECT_THROW(
        (void)Operations2D::clip_to_logical_box(grid, box),
        FVGridException
    );
}

TEST(Operations2D, RejectsIncompatiblePatternsBetweenGrids) {
    const StructuredGrid2D volume_grid{
        Axis1D{std::vector<Real>{0.0, 1.0, 2.0}},
        Axis1D{std::vector<Real>{0.0, 1.0, 2.0}}
    };
    const StructuredGrid2D face_grid{face_centered_axis(), face_centered_axis()};

    EXPECT_THROW(
        (void)Operations2D::intersection(volume_grid, face_grid),
        FVGridException
    );
}

TEST(Operations2D, ClipsFaceCenteredGridToLogicalBox) {
    const StructuredGrid2D face_grid{face_centered_axis(), face_centered_axis()};
    const LogicalBox2D box = LogicalBox2D::from_intervals(
        AxisInterval1D::from_bounds(0.25, 1.75),
        AxisInterval1D::from_bounds(0.25, 1.75)
    );

    const StructuredGrid2D clipped = Operations2D::clip_to_logical_box(
        face_grid,
        box
    );

    EXPECT_EQ(clipped.first_axis().pattern_name(), FaceCentered1D::name());
    EXPECT_EQ(clipped.second_axis().pattern_name(), FaceCentered1D::name());
    EXPECT_DOUBLE_EQ(clipped.first_face(0), 0.25);
    EXPECT_DOUBLE_EQ(clipped.first_face(clipped.num_faces_x() - 1), 1.75);
    EXPECT_DOUBLE_EQ(clipped.second_face(0), 0.25);
    EXPECT_DOUBLE_EQ(clipped.second_face(clipped.num_faces_y() - 1), 1.75);
}

TEST(Operations2D, RejectsInvalidTolerance) {
    EXPECT_THROW(Operations2D::validate_tolerance(-1.0), FVGridException);
}

}  // namespace fvgrid
