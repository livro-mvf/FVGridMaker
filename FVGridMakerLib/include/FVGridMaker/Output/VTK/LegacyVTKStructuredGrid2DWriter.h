// ----------------------------------------------------------------------------
// File: LegacyVTKStructuredGrid2DWriter.h
// Project: FVGridMaker
// Description: Legacy VTK writer for mapped two-dimensional structured grids.
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

#include <concepts>
#include <filesystem>
#include <iosfwd>
#include <string_view>

#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

namespace fvgrid {

class LegacyVTKStructuredGrid2DWriter final {
public:
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "Output",
            "LegacyVTKStructuredGrid2DWriter",
            "fvgrid.output.vtk.LegacyVTKStructuredGrid2DWriter"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    template <std::floating_point T>
    static void write(
        const BasicStructuredGrid2D<T>& grid,
        const std::filesystem::path& filepath
    );

private:
    template <std::floating_point T>
    static void write_geometry(
        std::ostream& output,
        const BasicStructuredGrid2D<T>& grid
    );

    template <std::floating_point T>
    static void write_cell_measure_data(
        std::ostream& output,
        const BasicStructuredGrid2D<T>& grid
    );
};

}  // namespace fvgrid

#include <FVGridMaker/Output/VTK/LegacyVTKStructuredGrid2DWriter.tpp>
