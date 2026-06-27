// ----------------------------------------------------------------------------
// File: Quality1D.tpp
// Project: FVGridMaker
// Description: Template implementation for Quality1D.
// ----------------------------------------------------------------------------

#pragma once

#include <algorithm>
#include <span>

namespace fvgrid {

namespace detail {

template <std::floating_point T>
[[nodiscard]] T ratio_between(T left, T right) noexcept {
    if (left > right) {
        return left / right;
    }

    return right / left;
}

}  // namespace detail

template <std::floating_point T>
BasicQualityMetrics1D<T> Quality1D::metrics(const BasicAxis1D<T>& axis) {
    const std::span<const T> lengths = axis.cell_lengths();
    const auto [min_it, max_it] = std::ranges::minmax_element(lengths);

    BasicQualityMetrics1D<T> result{};
    result.min_cell_length = *min_it;
    result.max_cell_length = *max_it;
    result.cell_length_ratio = result.max_cell_length / result.min_cell_length;
    result.max_adjacent_cell_ratio = Quality1D::max_adjacent_cell_ratio(axis);

    return result;
}

template <std::floating_point T>
T Quality1D::min_cell_length(const BasicAxis1D<T>& axis) {
    const std::span<const T> lengths = axis.cell_lengths();
    return *std::ranges::min_element(lengths);
}

template <std::floating_point T>
T Quality1D::max_cell_length(const BasicAxis1D<T>& axis) {
    const std::span<const T> lengths = axis.cell_lengths();
    return *std::ranges::max_element(lengths);
}

template <std::floating_point T>
T Quality1D::cell_length_ratio(const BasicAxis1D<T>& axis) {
    const BasicQualityMetrics1D<T> values = Quality1D::metrics(axis);
    return values.cell_length_ratio;
}

template <std::floating_point T>
T Quality1D::max_adjacent_cell_ratio(const BasicAxis1D<T>& axis) {
    const std::span<const T> lengths = axis.cell_lengths();

    if (lengths.size() < static_cast<Size>(2)) {
        return T{1};
    }

    T max_ratio = T{1};

    for (Size i = 1; i < lengths.size(); ++i) {
        const T local_ratio = detail::ratio_between(
            lengths[i - static_cast<Size>(1)],
            lengths[i]
        );

        if (local_ratio > max_ratio) {
            max_ratio = local_ratio;
        }
    }

    return max_ratio;
}

template <std::floating_point T>
BasicQualityMetrics1D<T> quality_metrics_1d(const BasicAxis1D<T>& axis) {
    return Quality1D::metrics(axis);
}

inline QualityMetrics1D quality_metrics_1d(const Axis1D& axis) {
    return Quality1D::metrics(axis);
}

template <std::floating_point T>
T min_cell_length_1d(const BasicAxis1D<T>& axis) {
    return Quality1D::min_cell_length(axis);
}

inline Real min_cell_length_1d(const Axis1D& axis) {
    return Quality1D::min_cell_length(axis);
}

template <std::floating_point T>
T max_cell_length_1d(const BasicAxis1D<T>& axis) {
    return Quality1D::max_cell_length(axis);
}

inline Real max_cell_length_1d(const Axis1D& axis) {
    return Quality1D::max_cell_length(axis);
}

template <std::floating_point T>
T cell_length_ratio_1d(const BasicAxis1D<T>& axis) {
    return Quality1D::cell_length_ratio(axis);
}

inline Real cell_length_ratio_1d(const Axis1D& axis) {
    return Quality1D::cell_length_ratio(axis);
}

template <std::floating_point T>
T max_adjacent_cell_ratio_1d(const BasicAxis1D<T>& axis) {
    return Quality1D::max_adjacent_cell_ratio(axis);
}

inline Real max_adjacent_cell_ratio_1d(const Axis1D& axis) {
    return Quality1D::max_adjacent_cell_ratio(axis);
}

}  // namespace fvgrid
