// ----------------------------------------------------------------------------
// File: Quality2D.cc
// Project: FVGridMaker
// Description: Implements quality metrics for StructuredGrid2D.
// License: MIT
// ----------------------------------------------------------------------------

#include <algorithm>
#include <limits>
#include <span>

#include <FVGridMaker/TwoDimensional/Quality2D/Quality2D.h>

namespace fvgrid {

namespace {

struct MinMax final {
    Real min = std::numeric_limits<Real>::infinity();
    Real max = -std::numeric_limits<Real>::infinity();
};

[[nodiscard]] MinMax minmax_of(std::span<const Real> values) noexcept {
    MinMax result{};

    for (const Real value : values) {
        result.min = std::min(result.min, value);
        result.max = std::max(result.max, value);
    }

    return result;
}

[[nodiscard]] Real ratio(Real minimum, Real maximum) noexcept {
    return maximum / minimum;
}

[[nodiscard]] Real adjacent_ratio(Real left, Real right) noexcept {
    return std::max(left / right, right / left);
}

}  // namespace

QualityReport2D Quality2D::evaluate(const StructuredGrid2D& grid) {
    QualityReport2D report{};

    const MinMax cell_measure = minmax_of(grid.cell_measures());
    report.min_cell_measure = cell_measure.min;
    report.max_cell_measure = cell_measure.max;
    report.cell_measure_ratio = ratio(cell_measure.min, cell_measure.max);

    MinMax computational_area{};

    for (Size j = 0; j < grid.num_cells_y(); ++j) {
        for (Size i = 0; i < grid.num_cells_x(); ++i) {
            const Real area = grid.cell_area(i, j);
            computational_area.min = std::min(computational_area.min, area);
            computational_area.max = std::max(computational_area.max, area);
        }
    }

    report.min_computational_area = computational_area.min;
    report.max_computational_area = computational_area.max;
    report.computational_area_ratio = ratio(
        computational_area.min,
        computational_area.max
    );

    report.max_adjacent_cell_measure_ratio_first_direction = Real{1};

    for (Size j = 0; j < grid.num_cells_y(); ++j) {
        for (Size i = 1; i < grid.num_cells_x(); ++i) {
            report.max_adjacent_cell_measure_ratio_first_direction = std::max(
                report.max_adjacent_cell_measure_ratio_first_direction,
                adjacent_ratio(
                    grid.cell_measure(i - static_cast<Size>(1), j),
                    grid.cell_measure(i, j)
                )
            );
        }
    }

    report.max_adjacent_cell_measure_ratio_second_direction = Real{1};

    for (Size j = 1; j < grid.num_cells_y(); ++j) {
        for (Size i = 0; i < grid.num_cells_x(); ++i) {
            report.max_adjacent_cell_measure_ratio_second_direction = std::max(
                report.max_adjacent_cell_measure_ratio_second_direction,
                adjacent_ratio(
                    grid.cell_measure(i, j - static_cast<Size>(1)),
                    grid.cell_measure(i, j)
                )
            );
        }
    }

    const MinMax first_face_measure = minmax_of(grid.first_face_measures());
    report.min_first_face_measure = first_face_measure.min;
    report.max_first_face_measure = first_face_measure.max;

    const MinMax second_face_measure = minmax_of(grid.second_face_measures());
    report.min_second_face_measure = second_face_measure.min;
    report.max_second_face_measure = second_face_measure.max;

    return report;
}

}  // namespace fvgrid
