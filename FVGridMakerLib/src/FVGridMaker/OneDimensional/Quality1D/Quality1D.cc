// ----------------------------------------------------------------------------
// File: Quality1D.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Implements one-dimensional mesh quality metrics.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <algorithm>
#include <span>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/OneDimensional/Quality1D/Quality1D.h>

namespace fvgrid {

namespace {

[[nodiscard]] Real ratio_between(
    Real left,
    Real right
) noexcept {
    if (left > right) {
        return left / right;
    }

    return right / left;
}

}  // namespace

QualityMetrics1D Quality1D::metrics(
    const Axis1D& axis
) {
    const std::span<const Real> lengths = axis.cell_lengths();

    const auto [min_it, max_it] =
        std::ranges::minmax_element(lengths);

    QualityMetrics1D result{};
    result.min_cell_length = *min_it;
    result.max_cell_length = *max_it;
    result.cell_length_ratio =
        result.max_cell_length / result.min_cell_length;
    result.max_adjacent_cell_ratio =
        Quality1D::max_adjacent_cell_ratio(axis);

    return result;
}

Real Quality1D::min_cell_length(
    const Axis1D& axis
) {
    const std::span<const Real> lengths = axis.cell_lengths();

    return *std::ranges::min_element(lengths);
}

Real Quality1D::max_cell_length(
    const Axis1D& axis
) {
    const std::span<const Real> lengths = axis.cell_lengths();

    return *std::ranges::max_element(lengths);
}

Real Quality1D::cell_length_ratio(
    const Axis1D& axis
) {
    const QualityMetrics1D values = Quality1D::metrics(axis);

    return values.cell_length_ratio;
}

Real Quality1D::max_adjacent_cell_ratio(
    const Axis1D& axis
) {
    const std::span<const Real> lengths = axis.cell_lengths();

    if (lengths.size() < static_cast<Size>(2)) {
        return static_cast<Real>(1.0);
    }

    Real max_ratio = static_cast<Real>(1.0);

    for (Size i = 1; i < lengths.size(); ++i) {
        const Real local_ratio =
            ratio_between(lengths[i - static_cast<Size>(1)], lengths[i]);

        if (local_ratio > max_ratio) {
            max_ratio = local_ratio;
        }
    }

    return max_ratio;
}

QualityMetrics1D quality_metrics_1d(
    const Axis1D& axis
) {
    return Quality1D::metrics(axis);
}

Real min_cell_length_1d(
    const Axis1D& axis
) {
    return Quality1D::min_cell_length(axis);
}

Real max_cell_length_1d(
    const Axis1D& axis
) {
    return Quality1D::max_cell_length(axis);
}

Real cell_length_ratio_1d(
    const Axis1D& axis
) {
    return Quality1D::cell_length_ratio(axis);
}

Real max_adjacent_cell_ratio_1d(
    const Axis1D& axis
) {
    return Quality1D::max_adjacent_cell_ratio(axis);
}

}  // namespace fvgrid