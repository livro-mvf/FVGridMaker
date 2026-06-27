// ----------------------------------------------------------------------------
// File: Quality1D.h
// Project: FVGridMaker
// Version: 0.1.0
// Description: Declares one-dimensional mesh quality metrics.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <string_view>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>

namespace fvgrid {

struct QualityMetrics1D final {
    Real min_cell_length = static_cast<Real>(0.0);
    Real max_cell_length = static_cast<Real>(0.0);
    Real cell_length_ratio = static_cast<Real>(0.0);
    Real max_adjacent_cell_ratio = static_cast<Real>(0.0);
};

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

    [[nodiscard]] static QualityMetrics1D metrics(const Axis1D& axis);

    [[nodiscard]] static Real min_cell_length(const Axis1D& axis);

    [[nodiscard]] static Real max_cell_length(const Axis1D& axis);

    [[nodiscard]] static Real cell_length_ratio(const Axis1D& axis);

    [[nodiscard]] static Real max_adjacent_cell_ratio(const Axis1D& axis);
};

[[nodiscard]] QualityMetrics1D quality_metrics_1d(const Axis1D& axis);

[[nodiscard]] Real min_cell_length_1d(const Axis1D& axis);

[[nodiscard]] Real max_cell_length_1d(const Axis1D& axis);

[[nodiscard]] Real cell_length_ratio_1d(const Axis1D& axis);

[[nodiscard]] Real max_adjacent_cell_ratio_1d(const Axis1D& axis);

}  // namespace fvgrid