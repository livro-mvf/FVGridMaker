#include <filesystem>
#include <fstream>
#include <numbers>
#include <sstream>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/Output/VTK/LegacyVTKRectilinearGrid2DWriter.h>
#include <FVGridMaker/TwoDimensional/CoordinateSystem2D/CoordinateSystem2D.h>
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

namespace fvgrid {

TEST(MappedVTKGrid2D, WritesPolarGridAsStructuredGrid) {
    const StructuredGrid2D grid{
        Axis1D{std::vector<Real>{1.0, 2.0}},
        Axis1D{std::vector<Real>{0.0, std::numbers::pi / 2.0}},
        PolarCoordinates2D{}
    };
    const auto path = std::filesystem::temp_directory_path()
                    / "fvgridmaker_polar_grid_2d.vtk";

    write_vtk(grid, path);
    std::ifstream input{path};
    std::ostringstream buffer;
    buffer << input.rdbuf();
    const std::string content = buffer.str();

    EXPECT_NE(content.find("DATASET STRUCTURED_GRID"), std::string::npos);
    EXPECT_NE(content.find("POINTS 4 double"), std::string::npos);
    EXPECT_NE(content.find("StructuredGrid2D (Polar)"), std::string::npos);
    std::filesystem::remove(path);
}

}  // namespace fvgrid
