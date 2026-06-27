// ----------------------------------------------------------------------------
// File: Axis1DCSVWriter.tpp
// Project: FVGridMaker
// Description: Template implementation for Axis1D CSV output.
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
#include <FVGridMaker/OneDimensional/Axis1D/Detail/Axis1DRows.h>

namespace fvgrid {

template <std::floating_point T>
void Axis1DCSVWriter::write(
    const BasicAxis1D<T>& axis,
    std::ostream& output
) {
    output << std::setprecision(std::numeric_limits<T>::max_digits10);
    output << "i,xF,xC,Dx,dx\n";

    detail::for_each_axis1d_row(
        axis,
        [&output](
            Size index,
            T face,
            T center,
            T dx_face,
            T dx_center,
            bool has_cell_data
        ) {
            output << index << "," << face << ",";

            if (has_cell_data) {
                output << center << "," << dx_face << ",";
            } else {
                output << ",,";
            }

            output << dx_center << "\n";
        }
    );

    output.flush();

    require<errors::output::FileWriteFailed>(
        static_cast<bool>(output),
        Axis1DCSVWriter::id()
    );
}

template <std::floating_point T>
void Axis1DCSVWriter::write(
    const BasicAxis1D<T>& axis,
    const std::filesystem::path& filepath
) {
    const std::filesystem::path parent_path = filepath.parent_path();

    if (!parent_path.empty()) {
        std::error_code error;
        std::filesystem::create_directories(parent_path, error);

        require<errors::output::FileOpenFailed>(
            !error,
            Axis1DCSVWriter::id()
        );
    }

    std::ofstream output{filepath};

    require<errors::output::FileOpenFailed>(
        output.is_open(),
        Axis1DCSVWriter::id()
    );

    write(axis, output);
}

}  // namespace fvgrid
