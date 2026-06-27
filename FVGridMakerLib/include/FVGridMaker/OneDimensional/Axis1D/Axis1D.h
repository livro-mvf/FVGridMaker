// ----------------------------------------------------------------------------
// File: Axis1D.h
// Project: FVGridMaker
// Version: 0.1.0
// Description: Defines a one-dimensional structured finite-volume axis.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

#include <concepts>
#include <iosfwd>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/AxisGeometry1D.h>

namespace fvgrid {

template <std::floating_point T>
class BasicAxis1D final {
public:
    using value_type = T;

    BasicAxis1D() = delete;

    explicit BasicAxis1D(std::vector<T> faces);

    BasicAxis1D(
        std::vector<T> faces,
        std::vector<T> centers,
        std::string pattern_name
    );

    [[nodiscard]] static BasicAxis1D from_geometry(
        BasicAxisGeometry1D<T> geometry
    );

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "Axis1D",
            "fvgrid.1d.axis.Axis1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] Size num_cells() const noexcept;
    [[nodiscard]] Size num_faces() const noexcept;

    [[nodiscard]] std::span<const T> faces() const noexcept;
    [[nodiscard]] std::span<const T> centers() const noexcept;

    [[nodiscard]] std::span<const T> dx_faces() const noexcept;
    [[nodiscard]] std::span<const T> dx_centers() const noexcept;

    [[nodiscard]] std::span<const T> cell_lengths() const noexcept;

    [[nodiscard]] T face(Size i) const noexcept;
    [[nodiscard]] T center(Size p) const noexcept;

    [[nodiscard]] T west_face(Size p) const noexcept;
    [[nodiscard]] T east_face(Size p) const noexcept;

    [[nodiscard]] T cell_length(Size p) const noexcept;
    [[nodiscard]] T center_distance(Size i) const noexcept;

    [[nodiscard]] T DxP(Size p) const noexcept;
    [[nodiscard]] T DxW(Size p) const noexcept;
    [[nodiscard]] T DxE(Size p) const noexcept;

    [[nodiscard]] T deltaxw(Size p) const noexcept;
    [[nodiscard]] T deltaxe(Size p) const noexcept;

    [[nodiscard]] T xmin() const noexcept;
    [[nodiscard]] T xmax() const noexcept;
    [[nodiscard]] T length() const noexcept;

    [[nodiscard]] std::string_view pattern_name() const noexcept;

    template <class Pattern>
        requires requires {
            { Pattern::name() } -> std::convertible_to<std::string_view>;
        }
    [[nodiscard]] bool has_pattern() const noexcept {
        return pattern_name() == Pattern::name();
    }

private:
    std::vector<T> faces_;
    std::vector<T> centers_;
    std::vector<T> dx_faces_;
    std::vector<T> dx_centers_;

    std::string pattern_name_{"VolumeCentered1D"};

    void validate_geometry() const;
    void rebuild_metrics();
};

using Axis1D = BasicAxis1D<double>;
using Axis1DFloat = BasicAxis1D<float>;
using Axis1DLongDouble = BasicAxis1D<long double>;

template <std::floating_point T>
std::ostream& operator<<(std::ostream& os, const BasicAxis1D<T>& axis);

}  // namespace fvgrid

#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.tpp>
