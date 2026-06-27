// ----------------------------------------------------------------------------
// File: LegacyVTKRectilinearGrid2DWriter.tpp
// Project: FVGridMaker
// Description: Template implementation for rectilinear legacy VTK output.
// ----------------------------------------------------------------------------

#pragma once

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <limits>
#include <ostream>
#include <system_error>

#include <FVGridMaker/ErrorHandling/BuiltInErrors.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>
#include <FVGridMaker/Output/VTK/LegacyVTKStructuredGrid2DWriter.h>

namespace fvgrid {

template <std::floating_point T>
void LegacyVTKRectilinearGrid2DWriter::write(
    const BasicStructuredGrid2D<T>& grid,
    const std::filesystem::path& filepath
) {
    require<errors::core::InvalidArgument>(grid.vtk_rectilinear(), id());

    const auto parent = filepath.parent_path();

    if (!parent.empty()) {
        std::error_code error;
        std::filesystem::create_directories(parent, error);
        require<errors::output::FileOpenFailed>(!error, id());
    }

    std::ofstream output{filepath};
    require<errors::output::FileOpenFailed>(output.is_open(), id());

    output << std::setprecision(std::numeric_limits<T>::max_digits10)
           << "# vtk DataFile Version 3.0\n"
           << "FVGridMaker StructuredGrid2D ("
           << grid.coordinate_system_name() << ")\n"
           << "ASCII\n";

    write_geometry(output, grid);
    write_cell_measure_data(output, grid);

    output.flush();
    require<errors::output::FileWriteFailed>(static_cast<bool>(output), id());
}

template <std::floating_point T>
void LegacyVTKRectilinearGrid2DWriter::write_geometry(
    std::ostream& output,
    const BasicStructuredGrid2D<T>& grid
) {
    output << "DATASET RECTILINEAR_GRID\n"
           << "DIMENSIONS " << grid.num_faces_x() << " "
           << grid.num_faces_y() << " 1\n\n";

    write_coordinate_array(output, "X_COORDINATES", grid.first_axis().faces());
    write_coordinate_array(output, "Y_COORDINATES", grid.second_axis().faces());

    output << "Z_COORDINATES 1 double\n0\n\n";
}

template <std::floating_point T>
void LegacyVTKRectilinearGrid2DWriter::write_coordinate_array(
    std::ostream& output,
    std::string_view vtk_name,
    std::span<const T> coordinates
) {
    output << vtk_name << " " << coordinates.size() << " "
           << vtk_scalar_type_name<T>() << "\n";

    for (const T coordinate : coordinates) {
        output << coordinate << "\n";
    }

    output << "\n";
}

template <std::floating_point T>
void LegacyVTKRectilinearGrid2DWriter::write_cell_measure_data(
    std::ostream& output,
    const BasicStructuredGrid2D<T>& grid
) {
    output << "CELL_DATA " << grid.num_cells() << "\n"
           << "SCALARS cell_measure " << vtk_scalar_type_name<T>() << " 1\n"
           << "LOOKUP_TABLE default\n";

    for (Size j = 0; j < grid.num_cells_y(); ++j) {
        for (Size i = 0; i < grid.num_cells_x(); ++i) {
            output << grid.cell_measure(i, j) << "\n";
        }
    }
}

template <std::floating_point T>
void write_vtk(
    const BasicStructuredGrid2D<T>& grid,
    const std::filesystem::path& filepath
) {
    if (grid.vtk_rectilinear()) {
        LegacyVTKRectilinearGrid2DWriter::write(grid, filepath);
        return;
    }

    LegacyVTKStructuredGrid2DWriter::write(grid, filepath);
}

}  // namespace fvgrid
