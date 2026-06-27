#include <filesystem>
#include <fstream>
#include <numbers>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include <gtest/gtest.h>

#include <FVGridMaker/ErrorHandling/FVGridException.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/Output/VTK/LegacyVTKStructuredGrid2DWriter.h>
#include <FVGridMaker/TwoDimensional/CoordinateSystem2D/CoordinateSystem2D.h>
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

namespace fvgrid {

namespace {

std::string read_text_file(const std::filesystem::path& filepath) {
    std::ifstream input{filepath};
    std::ostringstream buffer;
    buffer << input.rdbuf();
    return buffer.str();
}

}  // namespace

TEST(LegacyVTKStructuredGrid2DWriter, StoresClassIdentity) {
    EXPECT_EQ(
        LegacyVTKStructuredGrid2DWriter::id().module(),
        std::string_view{"Output"}
    );
    EXPECT_EQ(
        LegacyVTKStructuredGrid2DWriter::id().class_name(),
        std::string_view{"LegacyVTKStructuredGrid2DWriter"}
    );
    EXPECT_EQ(
        LegacyVTKStructuredGrid2DWriter::id().class_id(),
        std::string_view{"fvgrid.output.vtk.LegacyVTKStructuredGrid2DWriter"}
    );
}

TEST(LegacyVTKStructuredGrid2DWriter, WritesPolarGridAsStructuredGrid) {
    const StructuredGrid2D grid{
        Axis1D{std::vector<Real>{1.0, 2.0}},
        Axis1D{std::vector<Real>{0.0, std::numbers::pi / 2.0}},
        PolarCoordinates2D{}
    };
    const std::filesystem::path path = std::filesystem::temp_directory_path()
        / "fvgridmaker_structured_polar_grid_2d.vtk";

    LegacyVTKStructuredGrid2DWriter::write(grid, path);
    const std::string content = read_text_file(path);

    EXPECT_NE(content.find("DATASET STRUCTURED_GRID"), std::string::npos);
    EXPECT_NE(content.find("DIMENSIONS 2 2 1"), std::string::npos);
    EXPECT_NE(content.find("POINTS 4 double"), std::string::npos);
    EXPECT_NE(content.find("StructuredGrid2D (Polar)"), std::string::npos);
    EXPECT_NE(content.find("CELL_DATA 1"), std::string::npos);
    EXPECT_EQ(content.find("DATASET RECTILINEAR_GRID"), std::string::npos);

    std::filesystem::remove(path);
}

TEST(LegacyVTKStructuredGrid2DWriter, RejectsRectilinearGrid) {
    const StructuredGrid2D grid{
        Axis1D{std::vector<Real>{0.0, 1.0}},
        Axis1D{std::vector<Real>{0.0, 1.0}}
    };
    const std::filesystem::path path = std::filesystem::temp_directory_path()
        / "fvgridmaker_structured_rejects_rectilinear.vtk";

    EXPECT_THROW(
        LegacyVTKStructuredGrid2DWriter::write(grid, path),
        FVGridException
    );

    std::filesystem::remove(path);
}

}  // namespace fvgrid
