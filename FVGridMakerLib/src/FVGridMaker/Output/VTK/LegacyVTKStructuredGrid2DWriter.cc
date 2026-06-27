// ----------------------------------------------------------------------------
// File: LegacyVTKStructuredGrid2DWriter.cc
// Project: FVGridMaker
// Description: Implements a legacy VTK writer for mapped structured 2D grids.
// License: MIT
// ----------------------------------------------------------------------------

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <ostream>
#include <system_error>

#include <FVGridMaker/ErrorHandling/BuiltInErrors.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>
#include <FVGridMaker/Output/VTK/LegacyVTKStructuredGrid2DWriter.h>

namespace fvgrid {

void LegacyVTKStructuredGrid2DWriter::write(
    const StructuredGrid2D& grid,
    const std::filesystem::path& filepath
) {
    require<errors::core::InvalidArgument>(
        !grid.vtk_rectilinear(),
        id()
    );

    const auto parent = filepath.parent_path();

    if (!parent.empty()) {
        std::error_code error;
        std::filesystem::create_directories(parent, error);

        require<errors::output::FileOpenFailed>(
            !error,
            id()
        );
    }

    std::ofstream output{filepath};

    require<errors::output::FileOpenFailed>(
        output.is_open(),
        id()
    );

    output << std::setprecision(17)
           << "# vtk DataFile Version 3.0\n"
           << "FVGridMaker StructuredGrid2D ("
           << grid.coordinate_system_name() << ")\n"
           << "ASCII\n";

    write_geometry(output, grid);
    write_cell_measure_data(output, grid);

    output.flush();

    require<errors::output::FileWriteFailed>(
        static_cast<bool>(output),
        id()
    );
}

void LegacyVTKStructuredGrid2DWriter::write_geometry(
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
            const PhysicalPoint2D point = grid.physical_vertex(i, j);
            output << point.x << ' ' << point.y << ' ' << point.z << '\n';
        }
    }

    output << '\n';
}

void LegacyVTKStructuredGrid2DWriter::write_cell_measure_data(
    std::ostream& output,
    const StructuredGrid2D& grid
) {
    output << "CELL_DATA " << grid.num_cells() << '\n'
           << "SCALARS cell_area double 1\n"
           << "LOOKUP_TABLE default\n";

    for (Size j = 0; j < grid.num_cells_y(); ++j) {
        for (Size i = 0; i < grid.num_cells_x(); ++i) {
            output << grid.cell_measure(i, j) << '\n';
        }
    }
}

}  // namespace fvgrid
