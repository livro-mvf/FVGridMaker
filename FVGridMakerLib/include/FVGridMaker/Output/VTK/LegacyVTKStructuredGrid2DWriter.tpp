// ----------------------------------------------------------------------------
// File: LegacyVTKStructuredGrid2DWriter.tpp
// Project: FVGridMaker
// Description: Template implementation for mapped legacy VTK output.
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
#include <FVGridMaker/Output/VTK/LegacyVTKRectilinearGrid2DWriter.h>

namespace fvgrid {

template <std::floating_point T>
void LegacyVTKStructuredGrid2DWriter::write(
    const BasicStructuredGrid2D<T>& grid,
    const std::filesystem::path& filepath
) {
    require<errors::core::InvalidArgument>(!grid.vtk_rectilinear(), id());

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
void LegacyVTKStructuredGrid2DWriter::write_geometry(
    std::ostream& output,
    const BasicStructuredGrid2D<T>& grid
) {
    output << "DATASET STRUCTURED_GRID\n"
           << "DIMENSIONS " << grid.num_faces_x() << " "
           << grid.num_faces_y() << " 1\n"
           << "POINTS " << grid.num_faces_x() * grid.num_faces_y()
           << " " << vtk_scalar_type_name<T>() << "\n";

    for (Size j = 0; j < grid.num_faces_y(); ++j) {
        for (Size i = 0; i < grid.num_faces_x(); ++i) {
            const BasicPhysicalPoint2D<T> point = grid.physical_vertex(i, j);
            output << point.x << " " << point.y << " " << point.z << "\n";
        }
    }

    output << "\n";
}

template <std::floating_point T>
void LegacyVTKStructuredGrid2DWriter::write_cell_measure_data(
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

}  // namespace fvgrid
