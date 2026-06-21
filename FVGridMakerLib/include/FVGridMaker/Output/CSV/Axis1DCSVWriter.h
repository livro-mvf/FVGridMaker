// ----------------------------------------------------------------------------
// File: Axis1DCSVWriter.h
// Project: FVGridMaker
// Version: 0.1.0
// Description: Declares a CSV writer for one-dimensional axes.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <filesystem>
#include <iosfwd>
#include <string_view>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
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

    static void write(
        const Axis1D& axis,
        std::ostream& output
    );

    static void write(
        const Axis1D& axis,
        const std::filesystem::path& filepath
    );
};

}  // namespace fvgrid
