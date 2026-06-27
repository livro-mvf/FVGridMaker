#include <limits>
#include <vector>

#include <gtest/gtest.h>

#include <FVGridMaker/ErrorHandling/FVGridException.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/TwoDimensional/CoordinateSystem2D/CoordinateMappingFactory2D.h>
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

namespace fvgrid {

TEST(CoordinateMeasureInvariants2D, RejectsNegativeCellMeasure) {
    const auto mapping=make_coordinate_mapping_2d(
        "Invalid","u","v",
        [](Real u,Real v){ return PhysicalPoint2D{u,v,0.0}; },
        [](CoordinateCell2D){ return -1.0; },
        [](Real,Real,Real){ return 1.0; },
        [](Real,Real,Real){ return 1.0; });
    EXPECT_THROW(
        StructuredGrid2D(Axis1D{{0.0,1.0}},Axis1D{{0.0,1.0}},mapping),
        FVGridException);
}

TEST(CoordinateMeasureInvariants2D, RejectsNonFiniteFaceMeasure) {
    const auto mapping=make_coordinate_mapping_2d(
        "Invalid","u","v",
        [](Real u,Real v){ return PhysicalPoint2D{u,v,0.0}; },
        [](CoordinateCell2D){ return 1.0; },
        [](Real,Real,Real){ return std::numeric_limits<Real>::infinity(); },
        [](Real,Real,Real){ return 1.0; });
    EXPECT_THROW(
        StructuredGrid2D(Axis1D{{0.0,1.0}},Axis1D{{0.0,1.0}},mapping),
        FVGridException);
}

TEST(CoordinateMeasureInvariants2D, RejectsNonFiniteMappedPoint) {
    const auto mapping=make_coordinate_mapping_2d(
        "InvalidMap","u","v",
        [](Real,Real){
            return PhysicalPoint2D{
                std::numeric_limits<Real>::quiet_NaN(),0.0,0.0
            };
        },
        [](CoordinateCell2D){ return 1.0; },
        [](Real,Real,Real){ return 1.0; },
        [](Real,Real,Real){ return 1.0; });

    EXPECT_THROW(
        StructuredGrid2D(Axis1D{{0.0,1.0}},Axis1D{{0.0,1.0}},mapping),
        FVGridException);
}
TEST(CoordinateMeasureInvariants2D, RejectsFaceMeasureIndexOutsideShape) {
    const StructuredGrid2D grid{Axis1D{{0.0,1.0}},Axis1D{{0.0,1.0}}};
    EXPECT_THROW((void)grid.first_face_measure(2,0),FVGridException);
    EXPECT_THROW((void)grid.second_face_measure(0,2),FVGridException);
}

}  // namespace fvgrid
