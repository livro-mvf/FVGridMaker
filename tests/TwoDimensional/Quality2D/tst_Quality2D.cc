#include <string_view>
#include <vector>

#include <gtest/gtest.h>

#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/TwoDimensional/Quality2D/Quality2D.h>
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

namespace fvgrid {

TEST(Quality2D, StoresClassIdentity) {
    EXPECT_EQ(Quality2D::id().module(), std::string_view{"TwoDimensional"});
    EXPECT_EQ(Quality2D::id().class_name(), std::string_view{"Quality2D"});
    EXPECT_EQ(
        Quality2D::id().class_id(),
        std::string_view{"fvgrid.2d.quality.Quality2D"}
    );
}

TEST(Quality2D, UniformCartesianGridHasUnitRatios) {
    const StructuredGrid2D grid{
        Axis1D{std::vector<Real>{0.0, 1.0, 2.0}},
        Axis1D{std::vector<Real>{0.0, 1.0, 2.0}}
    };

    const QualityReport2D report = Quality2D::evaluate(grid);

    EXPECT_DOUBLE_EQ(report.min_cell_measure, 1.0);
    EXPECT_DOUBLE_EQ(report.max_cell_measure, 1.0);
    EXPECT_DOUBLE_EQ(report.cell_measure_ratio, 1.0);
    EXPECT_DOUBLE_EQ(report.min_computational_area, 1.0);
    EXPECT_DOUBLE_EQ(report.max_computational_area, 1.0);
    EXPECT_DOUBLE_EQ(report.computational_area_ratio, 1.0);
    EXPECT_DOUBLE_EQ(
        report.max_adjacent_cell_measure_ratio_first_direction,
        1.0
    );
    EXPECT_DOUBLE_EQ(
        report.max_adjacent_cell_measure_ratio_second_direction,
        1.0
    );
    EXPECT_DOUBLE_EQ(report.min_first_face_measure, 1.0);
    EXPECT_DOUBLE_EQ(report.max_first_face_measure, 1.0);
    EXPECT_DOUBLE_EQ(report.min_second_face_measure, 1.0);
    EXPECT_DOUBLE_EQ(report.max_second_face_measure, 1.0);
}

TEST(Quality2D, NonUniformCartesianGridReportsHandComputedValues) {
    const StructuredGrid2D grid{
        Axis1D{std::vector<Real>{0.0, 1.0, 3.0}},
        Axis1D{std::vector<Real>{0.0, 1.0, 4.0}}
    };

    const QualityReport2D report = Quality2D::evaluate(grid);

    EXPECT_DOUBLE_EQ(report.min_cell_measure, 1.0);
    EXPECT_DOUBLE_EQ(report.max_cell_measure, 6.0);
    EXPECT_DOUBLE_EQ(report.cell_measure_ratio, 6.0);
    EXPECT_DOUBLE_EQ(report.min_computational_area, 1.0);
    EXPECT_DOUBLE_EQ(report.max_computational_area, 6.0);
    EXPECT_DOUBLE_EQ(report.computational_area_ratio, 6.0);
    EXPECT_DOUBLE_EQ(
        report.max_adjacent_cell_measure_ratio_first_direction,
        2.0
    );
    EXPECT_DOUBLE_EQ(
        report.max_adjacent_cell_measure_ratio_second_direction,
        3.0
    );
    EXPECT_DOUBLE_EQ(report.min_first_face_measure, 1.0);
    EXPECT_DOUBLE_EQ(report.max_first_face_measure, 3.0);
    EXPECT_DOUBLE_EQ(report.min_second_face_measure, 1.0);
    EXPECT_DOUBLE_EQ(report.max_second_face_measure, 2.0);
}

}  // namespace fvgrid
