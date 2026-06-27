// ----------------------------------------------------------------------------
// File: Quality2D.tpp
// Project: FVGridMaker
// Description: Template implementation for Quality2D.
// ----------------------------------------------------------------------------

#pragma once

#include <algorithm>
#include <limits>
#include <span>

namespace fvgrid {

namespace detail {

template <std::floating_point T>
struct MinMax2D final {
    T min = std::numeric_limits<T>::infinity();
    T max = -std::numeric_limits<T>::infinity();
};

template <std::floating_point T>
[[nodiscard]] MinMax2D<T> minmax_of(std::span<const T> values) noexcept {
    MinMax2D<T> result{};

    for (const T value : values) {
        result.min = std::min(result.min, value);
        result.max = std::max(result.max, value);
    }

    return result;
}

template <std::floating_point T>
[[nodiscard]] T ratio(T minimum, T maximum) noexcept {
    return maximum / minimum;
}

template <std::floating_point T>
[[nodiscard]] T adjacent_ratio(T left, T right) noexcept {
    return std::max(left / right, right / left);
}

}  // namespace detail

template <std::floating_point T>
BasicQualityReport2D<T> Quality2D::evaluate(
    const BasicStructuredGrid2D<T>& grid
) {
    BasicQualityReport2D<T> report{};

    const detail::MinMax2D<T> cell_measure =
        detail::minmax_of(grid.cell_measures());
    report.min_cell_measure = cell_measure.min;
    report.max_cell_measure = cell_measure.max;
    report.cell_measure_ratio = detail::ratio(cell_measure.min, cell_measure.max);

    detail::MinMax2D<T> computational_area{};

    for (Size j = 0; j < grid.num_cells_y(); ++j) {
        for (Size i = 0; i < grid.num_cells_x(); ++i) {
            const T area = grid.cell_logical_area(i, j);
            computational_area.min = std::min(computational_area.min, area);
            computational_area.max = std::max(computational_area.max, area);
        }
    }

    report.min_computational_area = computational_area.min;
    report.max_computational_area = computational_area.max;
    report.computational_area_ratio = detail::ratio(
        computational_area.min,
        computational_area.max
    );

    report.max_adjacent_cell_measure_ratio_first_direction = T{1};

    for (Size j = 0; j < grid.num_cells_y(); ++j) {
        for (Size i = 1; i < grid.num_cells_x(); ++i) {
            report.max_adjacent_cell_measure_ratio_first_direction = std::max(
                report.max_adjacent_cell_measure_ratio_first_direction,
                detail::adjacent_ratio(
                    grid.cell_measure(i - static_cast<Size>(1), j),
                    grid.cell_measure(i, j)
                )
            );
        }
    }

    report.max_adjacent_cell_measure_ratio_second_direction = T{1};

    for (Size j = 1; j < grid.num_cells_y(); ++j) {
        for (Size i = 0; i < grid.num_cells_x(); ++i) {
            report.max_adjacent_cell_measure_ratio_second_direction = std::max(
                report.max_adjacent_cell_measure_ratio_second_direction,
                detail::adjacent_ratio(
                    grid.cell_measure(i, j - static_cast<Size>(1)),
                    grid.cell_measure(i, j)
                )
            );
        }
    }

    const detail::MinMax2D<T> first_face_measure =
        detail::minmax_of(grid.first_face_measures());
    report.min_first_face_measure = first_face_measure.min;
    report.max_first_face_measure = first_face_measure.max;

    const detail::MinMax2D<T> second_face_measure =
        detail::minmax_of(grid.second_face_measures());
    report.min_second_face_measure = second_face_measure.min;
    report.max_second_face_measure = second_face_measure.max;

    return report;
}

}  // namespace fvgrid
