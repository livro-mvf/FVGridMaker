// ----------------------------------------------------------------------------
// File: Axis1DCSVWriter.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Implements a CSV writer for one-dimensional axes.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <ostream>
#include <system_error>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/ErrorCatalog.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>
#include <FVGridMaker/Output/CSV/Axis1DCSVWriter.h>

namespace fvgrid {

void Axis1DCSVWriter::write(
    const Axis1D& axis,
    std::ostream& output
) {
    output << std::setprecision(17);
    output << "i,xF,xC,Dx,dx\n";

    const Size nvol = axis.num_cells();

    for (Size i = 0; i < nvol; ++i) {
        output << i << ','
               << axis.faces()[i] << ','
               << axis.centers()[i] << ','
               << axis.cell_lengths()[i] << ','
               << axis.dx_centers()[i] << '\n';
    }

    output << nvol << ','
           << axis.faces()[nvol] << ",,,"
           << axis.dx_centers()[nvol] << '\n';

    output.flush();

    require(
        static_cast<bool>(output),
        error_catalog::kOutputFileWriteFailed,
        Axis1DCSVWriter::id()
    );
}

void Axis1DCSVWriter::write(
    const Axis1D& axis,
    const std::filesystem::path& filepath
) {
    const std::filesystem::path parent_path = filepath.parent_path();

    if (!parent_path.empty()) {
        std::error_code error;
        std::filesystem::create_directories(parent_path, error);

        require(
            !error,
            error_catalog::kOutputFileOpenFailed,
            Axis1DCSVWriter::id()
        );
    }

    std::ofstream output{filepath};

    require(
        output.is_open(),
        error_catalog::kOutputFileOpenFailed,
        Axis1DCSVWriter::id()
    );

    write(axis, output);
}

}  // namespace fvgrid
