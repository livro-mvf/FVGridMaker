// ----------------------------------------------------------------------------
// File: tst_LegacyVTKRectilinearGrid2DWriter.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Tests the legacy VTK writer for 2D rectilinear grids.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/FVGridException.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/Output/VTK/LegacyVTKRectilinearGrid2DWriter.h>
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

namespace {

std::string read_text_file(
    const std::filesystem::path& filepath
) {
    std::ifstream input{filepath};
    std::ostringstream buffer;
    buffer << input.rdbuf();

    return buffer.str();
}

StructuredGrid2D make_test_grid() {
    const Axis1D x_axis{
        std::vector<Real>{0.0, 1.0, 3.0}
    };

    const Axis1D y_axis{
        std::vector<Real>{0.0, 2.0, 5.0}
    };

    return StructuredGrid2D{x_axis, y_axis};
}

}  // namespace

TEST(LegacyVTKRectilinearGrid2DWriter, StoresClassIdentity) {
    EXPECT_EQ(
        LegacyVTKRectilinearGrid2DWriter::id().module(),
        std::string_view{"Output"}
    );

    EXPECT_EQ(
        LegacyVTKRectilinearGrid2DWriter::id().class_name(),
        std::string_view{"LegacyVTKRectilinearGrid2DWriter"}
    );

    EXPECT_EQ(
        LegacyVTKRectilinearGrid2DWriter::id().class_id(),
        std::string_view{
            "fvgrid.output.vtk.LegacyVTKRectilinearGrid2DWriter"
        }
    );

    EXPECT_EQ(
        LegacyVTKRectilinearGrid2DWriter::class_name(),
        std::string_view{"LegacyVTKRectilinearGrid2DWriter"}
    );

    EXPECT_EQ(
        LegacyVTKRectilinearGrid2DWriter::class_id(),
        std::string_view{
            "fvgrid.output.vtk.LegacyVTKRectilinearGrid2DWriter"
        }
    );
}

TEST(LegacyVTKRectilinearGrid2DWriter, WritesLegacyRectilinearGridFile) {
    const StructuredGrid2D grid = make_test_grid();

    const std::filesystem::path output_path =
        std::filesystem::temp_directory_path() /
        "fvgridmaker_tst_rectilinear_grid_2d.vtk";

    std::filesystem::remove(output_path);

    LegacyVTKRectilinearGrid2DWriter::write(grid, output_path);

    EXPECT_TRUE(std::filesystem::exists(output_path));

    const std::string content = read_text_file(output_path);

    EXPECT_NE(
        content.find("# vtk DataFile Version 3.0"),
        std::string::npos
    );

    EXPECT_NE(
        content.find("FVGridMaker StructuredGrid2D"),
        std::string::npos
    );

    EXPECT_NE(
        content.find("ASCII"),
        std::string::npos
    );

    EXPECT_NE(
        content.find("DATASET RECTILINEAR_GRID"),
        std::string::npos
    );

    EXPECT_NE(
        content.find("DIMENSIONS 3 3 1"),
        std::string::npos
    );

    EXPECT_NE(
        content.find("X_COORDINATES 3 double"),
        std::string::npos
    );

    EXPECT_NE(
        content.find("Y_COORDINATES 3 double"),
        std::string::npos
    );

    EXPECT_NE(
        content.find("Z_COORDINATES 1 double"),
        std::string::npos
    );

    EXPECT_NE(
        content.find("CELL_DATA 4"),
        std::string::npos
    );

    EXPECT_NE(
        content.find("SCALARS cell_area double 1"),
        std::string::npos
    );

    EXPECT_NE(
        content.find("LOOKUP_TABLE default"),
        std::string::npos
    );

    std::filesystem::remove(output_path);
}

TEST(LegacyVTKRectilinearGrid2DWriter, WritesCoordinatesFromGridFaces) {
    const StructuredGrid2D grid = make_test_grid();

    const std::filesystem::path output_path =
        std::filesystem::temp_directory_path() /
        "fvgridmaker_tst_rectilinear_grid_coordinates.vtk";

    std::filesystem::remove(output_path);

    LegacyVTKRectilinearGrid2DWriter::write(grid, output_path);

    const std::string content = read_text_file(output_path);

    EXPECT_NE(content.find("0\n1\n3\n"), std::string::npos);
    EXPECT_NE(content.find("0\n2\n5\n"), std::string::npos);

    std::filesystem::remove(output_path);
}

TEST(LegacyVTKRectilinearGrid2DWriter, WritesCellAreasInRowMajorOrder) {
    const StructuredGrid2D grid = make_test_grid();

    const std::filesystem::path output_path =
        std::filesystem::temp_directory_path() /
        "fvgridmaker_tst_rectilinear_grid_cell_area.vtk";

    std::filesystem::remove(output_path);

    LegacyVTKRectilinearGrid2DWriter::write(grid, output_path);

    const std::string content = read_text_file(output_path);

    const std::string expected_cell_area_block =
        "SCALARS cell_area double 1\n"
        "LOOKUP_TABLE default\n"
        "2\n"
        "4\n"
        "3\n"
        "6\n";

    EXPECT_NE(
        content.find(expected_cell_area_block),
        std::string::npos
    );

    std::filesystem::remove(output_path);
}

TEST(LegacyVTKRectilinearGrid2DWriter, CreatesParentDirectories) {
    const StructuredGrid2D grid = make_test_grid();

    const std::filesystem::path output_path =
        std::filesystem::temp_directory_path() /
        "fvgridmaker_vtk_writer_nested" /
        "structured_grid_2d.vtk";

    std::filesystem::remove_all(output_path.parent_path());

    LegacyVTKRectilinearGrid2DWriter::write(grid, output_path);

    EXPECT_TRUE(std::filesystem::exists(output_path));

    std::filesystem::remove_all(output_path.parent_path());
}

TEST(LegacyVTKRectilinearGrid2DWriter, RejectsFileOpenFailure) {
    const StructuredGrid2D grid = make_test_grid();

    const std::filesystem::path directory_path =
        std::filesystem::temp_directory_path();

    try {
        LegacyVTKRectilinearGrid2DWriter::write(grid, directory_path);
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.OUTPUT.FILE_OPEN_FAILED"}
        );

        EXPECT_EQ(
            exception.record().category,
            std::string_view{"Output"}
        );

        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"LegacyVTKRectilinearGrid2DWriter"}
        );

        return;
    }

    FAIL() << "The VTK writer accepted a directory as an output file.";
}

}  // namespace fvgrid