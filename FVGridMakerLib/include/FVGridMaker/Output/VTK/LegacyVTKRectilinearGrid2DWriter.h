// ----------------------------------------------------------------------------
// File: LegacyVTKRectilinearGrid2DWriter.h
// Project: FVGridMaker
// Description: Legacy VTK writer for rectilinear two-dimensional grids.
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

#include <concepts>
#include <filesystem>
#include <iosfwd>
#include <span>
#include <string_view>

#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

namespace fvgrid {

template <std::floating_point T>
constexpr std::string_view vtk_scalar_type_name() noexcept {
    if constexpr (std::same_as<T, float>) {
        return "float";
    } else {
        return "double";
    }
}

class LegacyVTKRectilinearGrid2DWriter final {
public:
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "Output",
            "LegacyVTKRectilinearGrid2DWriter",
            "fvgrid.output.vtk.LegacyVTKRectilinearGrid2DWriter"
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
    static void write_coordinate_array(
        std::ostream& output,
        std::string_view vtk_name,
        std::span<const T> coordinates
    );

    template <std::floating_point T>
    static void write_cell_measure_data(
        std::ostream& output,
        const BasicStructuredGrid2D<T>& grid
    );
};

template <std::floating_point T>
void write_vtk(
    const BasicStructuredGrid2D<T>& grid,
    const std::filesystem::path& filepath
);

}  // namespace fvgrid

#include <FVGridMaker/Output/VTK/LegacyVTKRectilinearGrid2DWriter.tpp>
