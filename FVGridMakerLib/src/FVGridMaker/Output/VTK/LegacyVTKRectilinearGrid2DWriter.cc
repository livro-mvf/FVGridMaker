// ----------------------------------------------------------------------------
// File: LegacyVTKRectilinearGrid2DWriter.cc
// Project: FVGridMaker
// License: MIT
// ----------------------------------------------------------------------------
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <ostream>
#include <system_error>

#include <FVGridMaker/ErrorHandling/ErrorCatalog.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>
#include <FVGridMaker/Output/VTK/LegacyVTKRectilinearGrid2DWriter.h>

namespace fvgrid {

void LegacyVTKRectilinearGrid2DWriter::write(
    const StructuredGrid2D& grid,
    const std::filesystem::path& filepath
) {
    const auto parent = filepath.parent_path();
    if (!parent.empty()) {
        std::error_code error;
        std::filesystem::create_directories(parent, error);
        require(!error, error_catalog::kOutputFileOpenFailed, id());
    }

    std::ofstream output{filepath};
    require(output.is_open(), error_catalog::kOutputFileOpenFailed, id());
    output << std::setprecision(17)
           << "# vtk DataFile Version 3.0\n"
           << "FVGridMaker StructuredGrid2D (" << grid.coordinate_system_name() << ")\n"
           << "ASCII\n";

    if (grid.vtk_rectilinear()) {
        write_rectilinear_geometry(output, grid);
    } else {
        write_structured_geometry(output, grid);
    }
    write_cell_measure_data(output, grid);
    output.flush();
    require(static_cast<bool>(output), error_catalog::kOutputFileWriteFailed, id());
}

void LegacyVTKRectilinearGrid2DWriter::write_rectilinear_geometry(
    std::ostream& output,
    const StructuredGrid2D& grid
) {
    output << "DATASET RECTILINEAR_GRID\n"
           << "DIMENSIONS " << grid.num_faces_x() << ' '
           << grid.num_faces_y() << " 1\n\n";
    write_coordinate_array(output, "X_COORDINATES", grid.first_axis().faces());
    write_coordinate_array(output, "Y_COORDINATES", grid.second_axis().faces());
    output << "Z_COORDINATES 1 double\n0\n\n";
}

void LegacyVTKRectilinearGrid2DWriter::write_structured_geometry(
    std::ostream& output,
    const StructuredGrid2D& grid
) {
    output << "DATASET STRUCTURED_GRID\n"
           << "DIMENSIONS " << grid.num_faces_x() << ' '
           << grid.num_faces_y() << " 1\n"
           << "POINTS " << grid.num_faces_x() * grid.num_faces_y()
           << " double\n";
    for (Size j = 0; j < grid.num_faces_y(); ++j) {
        for (Size i = 0; i < grid.num_faces_x(); ++i) {
            const PhysicalPoint2D point = grid.physical_face_point(i, j);
            output << point.x << ' ' << point.y << ' ' << point.z << '\n';
        }
    }
    output << '\n';
}

void LegacyVTKRectilinearGrid2DWriter::write_coordinate_array(
    std::ostream& output,
    std::string_view vtk_name,
    std::span<const Real> coordinates
) {
    output << vtk_name << ' ' << coordinates.size() << " double\n";
    for (const Real coordinate : coordinates) output << coordinate << '\n';
    output << '\n';
}

void LegacyVTKRectilinearGrid2DWriter::write_cell_measure_data(
    std::ostream& output,
    const StructuredGrid2D& grid
) {
    output << "CELL_DATA " << grid.num_cells() << '\n'
           << "SCALARS cell_area double 1\n"
           << "LOOKUP_TABLE default\n";
    for (Size j = 0; j < grid.num_cells_y(); ++j)
        for (Size i = 0; i < grid.num_cells_x(); ++i)
            output << grid.cell_measure(i, j) << '\n';
}

void write_vtk(
    const StructuredGrid2D& grid,
    const std::filesystem::path& filepath
) {
    LegacyVTKRectilinearGrid2DWriter::write(grid, filepath);
}

}  // namespace fvgrid
