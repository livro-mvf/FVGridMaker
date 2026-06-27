// ----------------------------------------------------------------------------
// File: Axis1DCSVWriter.h
// Project: FVGridMaker
// Version: 0.1.0
// Description: Declares a CSV writer for one-dimensional axes.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

#include <concepts>
#include <filesystem>
#include <iosfwd>
#include <string_view>

#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>

namespace fvgrid {

class Axis1DCSVWriter final {
public:
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "Output",
            "Axis1DCSVWriter",
            "fvgrid.output.csv.Axis1DCSVWriter"
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
        const BasicAxis1D<T>& axis,
        std::ostream& output
    );

    template <std::floating_point T>
    static void write(
        const BasicAxis1D<T>& axis,
        const std::filesystem::path& filepath
    );
};

}  // namespace fvgrid

#include <FVGridMaker/Output/CSV/Axis1DCSVWriter.tpp>
