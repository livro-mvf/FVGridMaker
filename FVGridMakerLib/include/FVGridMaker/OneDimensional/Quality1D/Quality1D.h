// ----------------------------------------------------------------------------
// File: Quality1D.h
// Project: FVGridMaker
// Version: 0.1.0
// Description: Declares one-dimensional mesh quality metrics.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

#include <concepts>
#include <string_view>

#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>

namespace fvgrid {

template <std::floating_point T>
struct BasicQualityMetrics1D final {
    using value_type = T;

    T min_cell_length{};
    T max_cell_length{};
    T cell_length_ratio{};
    T max_adjacent_cell_ratio{};
};

using QualityMetrics1D = BasicQualityMetrics1D<double>;
using QualityMetrics1DFloat = BasicQualityMetrics1D<float>;
using QualityMetrics1DLongDouble = BasicQualityMetrics1D<long double>;

class Quality1D final {
public:
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "Quality1D",
            "fvgrid.1d.quality.Quality1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    template <std::floating_point T>
    [[nodiscard]] static BasicQualityMetrics1D<T> metrics(
        const BasicAxis1D<T>& axis
    );

    template <std::floating_point T>
    [[nodiscard]] static T min_cell_length(const BasicAxis1D<T>& axis);

    template <std::floating_point T>
    [[nodiscard]] static T max_cell_length(const BasicAxis1D<T>& axis);

    template <std::floating_point T>
    [[nodiscard]] static T cell_length_ratio(const BasicAxis1D<T>& axis);

    template <std::floating_point T>
    [[nodiscard]] static T max_adjacent_cell_ratio(
        const BasicAxis1D<T>& axis
    );
};

template <std::floating_point T>
[[nodiscard]] BasicQualityMetrics1D<T> quality_metrics_1d(
    const BasicAxis1D<T>& axis
);

[[nodiscard]] QualityMetrics1D quality_metrics_1d(const Axis1D& axis);

template <std::floating_point T>
[[nodiscard]] T min_cell_length_1d(const BasicAxis1D<T>& axis);

[[nodiscard]] Real min_cell_length_1d(const Axis1D& axis);

template <std::floating_point T>
[[nodiscard]] T max_cell_length_1d(const BasicAxis1D<T>& axis);

[[nodiscard]] Real max_cell_length_1d(const Axis1D& axis);

template <std::floating_point T>
[[nodiscard]] T cell_length_ratio_1d(const BasicAxis1D<T>& axis);

[[nodiscard]] Real cell_length_ratio_1d(const Axis1D& axis);

template <std::floating_point T>
[[nodiscard]] T max_adjacent_cell_ratio_1d(const BasicAxis1D<T>& axis);

[[nodiscard]] Real max_adjacent_cell_ratio_1d(const Axis1D& axis);

}  // namespace fvgrid

#include <FVGridMaker/OneDimensional/Quality1D/Quality1D.tpp>
