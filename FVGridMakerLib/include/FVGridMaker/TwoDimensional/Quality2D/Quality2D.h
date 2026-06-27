// ----------------------------------------------------------------------------
// File: Quality2D.h
// Project: FVGridMaker
// Description: Lightweight quality metrics for StructuredGrid2D.
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

#include <string_view>

#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

namespace fvgrid {

struct QualityReport2D final {
    Real min_cell_measure{};
    Real max_cell_measure{};
    Real cell_measure_ratio{};

    Real min_computational_area{};
    Real max_computational_area{};
    Real computational_area_ratio{};

    Real max_adjacent_cell_measure_ratio_first_direction{};
    Real max_adjacent_cell_measure_ratio_second_direction{};

    Real min_first_face_measure{};
    Real max_first_face_measure{};
    Real min_second_face_measure{};
    Real max_second_face_measure{};
};

class Quality2D final {
public:
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "TwoDimensional",
            "Quality2D",
            "fvgrid.2d.quality.Quality2D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] static QualityReport2D evaluate(
        const StructuredGrid2D& grid
    );
};

}  // namespace fvgrid
