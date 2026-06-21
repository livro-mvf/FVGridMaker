// ----------------------------------------------------------------------------
// File: CoordinateSystem2D.h
// Project: FVGridMaker
// Description: Compile-time coordinate traits for two-dimensional grids.
// License: MIT
// ----------------------------------------------------------------------------
#pragma once

#include <cmath>
#include <concepts>
#include <numbers>
#include <string>
#include <string_view>
#include <utility>

#include <FVGridMaker/Core/Types.h>

namespace fvgrid {

struct PhysicalPoint2D final {
    Real x{};
    Real y{};
    Real z{};
};

struct CoordinateCell2D final {
    Real first_min{};
    Real first_max{};
    Real second_min{};
    Real second_max{};
};

template <class Mapping>
concept CoordinateMapping2D = requires(
    const Mapping& mapping,
    Real first,
    Real second,
    CoordinateCell2D cell
) {
    { mapping.name() } -> std::convertible_to<std::string_view>;
    { mapping.first_coordinate_name() } -> std::convertible_to<std::string_view>;
    { mapping.second_coordinate_name() } -> std::convertible_to<std::string_view>;
    { mapping.vtk_rectilinear() } -> std::convertible_to<bool>;
    { mapping.map(first, second) } -> std::same_as<PhysicalPoint2D>;
    { mapping.cell_measure(cell) } -> std::convertible_to<Real>;
};

struct CartesianCoordinates2D final {
    [[nodiscard]] constexpr std::string_view name() const noexcept {
        return "Cartesian";
    }
    [[nodiscard]] constexpr std::string_view first_coordinate_name() const noexcept {
        return "x";
    }
    [[nodiscard]] constexpr std::string_view second_coordinate_name() const noexcept {
        return "y";
    }
    [[nodiscard]] constexpr bool vtk_rectilinear() const noexcept { return true; }
    [[nodiscard]] constexpr PhysicalPoint2D map(Real x, Real y) const noexcept {
        return {x, y, Real{}};
    }
    [[nodiscard]] constexpr Real cell_measure(CoordinateCell2D c) const noexcept {
        return (c.first_max - c.first_min) * (c.second_max - c.second_min);
    }
};

struct PolarCoordinates2D final {
    [[nodiscard]] constexpr std::string_view name() const noexcept {
        return "Polar";
    }
    [[nodiscard]] constexpr std::string_view first_coordinate_name() const noexcept {
        return "r";
    }
    [[nodiscard]] constexpr std::string_view second_coordinate_name() const noexcept {
        return "theta";
    }
    [[nodiscard]] constexpr bool vtk_rectilinear() const noexcept { return false; }
    [[nodiscard]] PhysicalPoint2D map(Real r, Real theta) const noexcept {
        return {r * std::cos(theta), r * std::sin(theta), Real{}};
    }
    [[nodiscard]] constexpr Real cell_measure(CoordinateCell2D c) const noexcept {
        return Real{0.5} * (c.first_max * c.first_max
                          - c.first_min * c.first_min)
             * (c.second_max - c.second_min);
    }
};

struct AxisymmetricCoordinates2D final {
    [[nodiscard]] constexpr std::string_view name() const noexcept {
        return "Axisymmetric";
    }
    [[nodiscard]] constexpr std::string_view first_coordinate_name() const noexcept {
        return "r";
    }
    [[nodiscard]] constexpr std::string_view second_coordinate_name() const noexcept {
        return "z";
    }
    [[nodiscard]] constexpr bool vtk_rectilinear() const noexcept { return true; }
    [[nodiscard]] constexpr PhysicalPoint2D map(Real r, Real z) const noexcept {
        return {r, z, Real{}};
    }
    [[nodiscard]] constexpr Real cell_measure(CoordinateCell2D c) const noexcept {
        return std::numbers::pi_v<Real>
             * (c.first_max * c.first_max - c.first_min * c.first_min)
             * (c.second_max - c.second_min);
    }
};

template <class Map, class Measure>
class FunctionalCoordinateMapping2D final {
public:
    FunctionalCoordinateMapping2D(
        std::string name,
        std::string first_name,
        std::string second_name,
        Map map,
        Measure measure,
        bool vtk_rectilinear = false
    )
        : name_(std::move(name)),
          first_name_(std::move(first_name)),
          second_name_(std::move(second_name)),
          map_(std::move(map)),
          measure_(std::move(measure)),
          vtk_rectilinear_(vtk_rectilinear) {}

    [[nodiscard]] std::string_view name() const noexcept { return name_; }
    [[nodiscard]] std::string_view first_coordinate_name() const noexcept {
        return first_name_;
    }
    [[nodiscard]] std::string_view second_coordinate_name() const noexcept {
        return second_name_;
    }
    [[nodiscard]] bool vtk_rectilinear() const noexcept {
        return vtk_rectilinear_;
    }
    [[nodiscard]] PhysicalPoint2D map(Real first, Real second) const {
        return map_(first, second);
    }
    [[nodiscard]] Real cell_measure(CoordinateCell2D cell) const {
        return measure_(cell);
    }

private:
    std::string name_;
    std::string first_name_;
    std::string second_name_;
    Map map_;
    Measure measure_;
    bool vtk_rectilinear_{};
};

template <class Map, class Measure>
[[nodiscard]] auto make_coordinate_mapping_2d(
    std::string name,
    std::string first_name,
    std::string second_name,
    Map map,
    Measure measure,
    bool vtk_rectilinear = false
) {
    return FunctionalCoordinateMapping2D<Map, Measure>{
        std::move(name), std::move(first_name), std::move(second_name),
        std::move(map), std::move(measure), vtk_rectilinear
    };
}

}  // namespace fvgrid
