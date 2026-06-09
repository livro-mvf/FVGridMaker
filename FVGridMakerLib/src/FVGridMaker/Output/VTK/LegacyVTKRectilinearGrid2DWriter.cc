// ----------------------------------------------------------------------------
// File: LegacyVTKRectilinearGrid2DWriter.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Implements a legacy VTK writer for 2D rectilinear grids.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <ios>
#include <ostream>
#include <span>
#include <string_view>
#include <system_error>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/ErrorCatalog.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>
#include <FVGridMaker/Output/VTK/LegacyVTKRectilinearGrid2DWriter.h>

namespace fvgrid {

void LegacyVTKRectilinearGrid2DWriter::write(
    const StructuredGrid2D& grid,
    const std::filesystem::path& filepath
) {
    const std::filesystem::path parent_path = filepath.parent_path();

    if (!parent_path.empty()) {
        std::error_code error;
        std::filesystem::create_directories(parent_path, error);

        require(
            !error,
            error_catalog::kOutputFileOpenFailed,
            LegacyVTKRectilinearGrid2DWriter::id()
        );
    }

    std::ofstream output{filepath};

    require(
        output.is_open(),
        error_catalog::kOutputFileOpenFailed,
        LegacyVTKRectilinearGrid2DWriter::id()
    );

    output << std::setprecision(17);

    write_header(output, grid);
    write_coordinates(output, grid);
    write_cell_area_data(output, grid);

    output.flush();

    require(
        static_cast<bool>(output),
        error_catalog::kOutputFileWriteFailed,
        LegacyVTKRectilinearGrid2DWriter::id()
    );
}

void LegacyVTKRectilinearGrid2DWriter::write_header(
    std::ostream& output,
    const StructuredGrid2D& grid
) {
    output << "# vtk DataFile Version 3.0\n";
    output << "FVGridMaker StructuredGrid2D\n";
    output << "ASCII\n";
    output << "DATASET RECTILINEAR_GRID\n";
    output << "DIMENSIONS "
           << grid.num_faces_x() << ' '
           << grid.num_faces_y() << ' '
           << 1 << "\n\n";
}

void LegacyVTKRectilinearGrid2DWriter::write_coordinate_array(
    std::ostream& output,
    std::string_view vtk_name,
    std::span<const Real> coordinates
) {
    output << vtk_name << ' '
           << coordinates.size()
           << " double\n";

    for (const Real coordinate : coordinates) {
        output << coordinate << '\n';
    }

    output << '\n';
}

void LegacyVTKRectilinearGrid2DWriter::write_coordinates(
    std::ostream& output,
    const StructuredGrid2D& grid
) {
    write_coordinate_array(
        output,
        "X_COORDINATES",
        grid.x_axis().faces()
    );

    write_coordinate_array(
        output,
        "Y_COORDINATES",
        grid.y_axis().faces()
    );

    output << "Z_COORDINATES 1 double\n";
    output << static_cast<Real>(0.0) << "\n\n";
}

void LegacyVTKRectilinearGrid2DWriter::write_cell_area_data(
    std::ostream& output,
    const StructuredGrid2D& grid
) {
    output << "CELL_DATA " << grid.num_cells() << '\n';
    output << "SCALARS cell_area double 1\n";
    output << "LOOKUP_TABLE default\n";

    for (Size j = 0; j < grid.num_cells_y(); ++j) {
        for (Size i = 0; i < grid.num_cells_x(); ++i) {
            output << grid.cell_area(i, j) << '\n';
        }
    }
}

}  // namespace fvgrid