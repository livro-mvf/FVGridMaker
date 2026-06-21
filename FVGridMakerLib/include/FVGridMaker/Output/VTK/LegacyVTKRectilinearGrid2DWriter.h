// ----------------------------------------------------------------------------
// File: LegacyVTKRectilinearGrid2DWriter.h
// Project: FVGridMaker
// Description: Legacy VTK writer for separable two-dimensional grids.
// License: MIT
// ----------------------------------------------------------------------------
#pragma once

#include <filesystem>
#include <iosfwd>
#include <span>
#include <string_view>

#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

namespace fvgrid {

class LegacyVTKRectilinearGrid2DWriter final {
public:
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{"Output", "LegacyVTKRectilinearGrid2DWriter",
                  "fvgrid.output.vtk.LegacyVTKRectilinearGrid2DWriter"};
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    static void write(
        const StructuredGrid2D& grid,
        const std::filesystem::path& filepath
    );

private:
    static void write_rectilinear_geometry(
        std::ostream& output,
        const StructuredGrid2D& grid
    );
    static void write_structured_geometry(
        std::ostream& output,
        const StructuredGrid2D& grid
    );
    static void write_coordinate_array(
        std::ostream& output,
        std::string_view vtk_name,
        std::span<const Real> coordinates
    );
    static void write_cell_measure_data(
        std::ostream& output,
        const StructuredGrid2D& grid
    );
};

void write_vtk(
    const StructuredGrid2D& grid,
    const std::filesystem::path& filepath
);

}  // namespace fvgrid
