// ----------------------------------------------------------------------------
// File: Quality2D.h
// Project: FVGridMaker
// Description: Lightweight quality metrics for StructuredGrid2D.
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

#include <concepts>
#include <string_view>

#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

namespace fvgrid {

template <std::floating_point T>
struct BasicQualityReport2D final {
    using value_type = T;

    T min_cell_measure{};
    T max_cell_measure{};
    T cell_measure_ratio{};

    T min_computational_area{};
    T max_computational_area{};
    T computational_area_ratio{};

    T max_adjacent_cell_measure_ratio_first_direction{};
    T max_adjacent_cell_measure_ratio_second_direction{};

    T min_first_face_measure{};
    T max_first_face_measure{};
    T min_second_face_measure{};
    T max_second_face_measure{};
};

using QualityReport2D = BasicQualityReport2D<double>;
using QualityReport2DFloat = BasicQualityReport2D<float>;
using QualityReport2DLongDouble = BasicQualityReport2D<long double>;

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

    template <std::floating_point T>
    [[nodiscard]] static BasicQualityReport2D<T> evaluate(
        const BasicStructuredGrid2D<T>& grid
    );
};

}  // namespace fvgrid

#include <FVGridMaker/TwoDimensional/Quality2D/Quality2D.tpp>
