// ----------------------------------------------------------------------------
// File: CoordinateMetrics2D.h
// Project: FVGridMaker
// Description: Physical measures for supported two-dimensional coordinate maps.
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

#include <algorithm>
#include <cmath>
#include <concepts>
#include <limits>
#include <numbers>
#include <string_view>

#include <FVGridMaker/TwoDimensional/CoordinateSystem2D/CoordinateSystem2D.h>

namespace fvgrid {

template <std::floating_point T>
struct BasicPhysicalVector2D final {
    using value_type = T;

    T x{};
    T y{};
    T z{};
};

using PhysicalVector2D = BasicPhysicalVector2D<double>;
using PhysicalVector2DFloat = BasicPhysicalVector2D<float>;
using PhysicalVector2DLongDouble = BasicPhysicalVector2D<long double>;

template <std::floating_point T>
struct BasicSphericalAxisymmetricCoordinates2D final {
    using value_type = T;

    [[nodiscard]] constexpr std::string_view name() const noexcept {
        return "SphericalAxisymmetric";
    }

    [[nodiscard]] constexpr std::string_view first_coordinate_name()
        const noexcept {
        return "r";
    }

    [[nodiscard]] constexpr std::string_view second_coordinate_name()
        const noexcept {
        return "theta";
    }

    [[nodiscard]] constexpr bool vtk_rectilinear() const noexcept {
        return false;
    }

    [[nodiscard]] BasicPhysicalPoint2D<T> map(T r, T theta) const noexcept {
        return {r * std::sin(theta), r * std::cos(theta), T{}};
    }

    [[nodiscard]] T cell_measure(BasicCoordinateCell2D<T> c) const noexcept {
        return (T{2} * std::numbers::pi_v<T> / T{3}) *
            (c.first_max * c.first_max * c.first_max -
             c.first_min * c.first_min * c.first_min) *
            (std::cos(c.second_min) - std::cos(c.second_max));
    }
};

using SphericalAxisymmetricCoordinates2D =
    BasicSphericalAxisymmetricCoordinates2D<double>;
using SphericalAxisymmetricCoordinates2DFloat =
    BasicSphericalAxisymmetricCoordinates2D<float>;
using SphericalAxisymmetricCoordinates2DLongDouble =
    BasicSphericalAxisymmetricCoordinates2D<long double>;

using CylindricalAxisymmetricCoordinates2D = AxisymmetricCoordinates2D;
using CylindricalPolarCoordinates2D = PolarCoordinates2D;

namespace coordinate_metrics {

inline constexpr Size default_metric_segments_2d = 32;

template <class Mapping, std::floating_point T>
    requires CoordinateMapping2DFor<Mapping, T>
[[nodiscard]] T geometric_jacobian(
    const Mapping& mapping,
    T first,
    T second,
    T first_scale,
    T second_scale
) {
    const T perturbation = std::sqrt(std::numeric_limits<T>::epsilon());
    const T minimum_step = static_cast<T>(100) * std::numeric_limits<T>::epsilon();

    const T h1 = std::max(std::abs(first_scale) * perturbation, minimum_step);
    const T h2 = std::max(std::abs(second_scale) * perturbation, minimum_step);

    const BasicPhysicalPoint2D<T> left = mapping.map(first - h1, second);
    const BasicPhysicalPoint2D<T> right = mapping.map(first + h1, second);
    const BasicPhysicalPoint2D<T> below = mapping.map(first, second - h2);
    const BasicPhysicalPoint2D<T> above = mapping.map(first, second + h2);

    const BasicPhysicalVector2D<T> first_tangent{
        (right.x - left.x) / (T{2} * h1),
        (right.y - left.y) / (T{2} * h1),
        (right.z - left.z) / (T{2} * h1)
    };
    const BasicPhysicalVector2D<T> second_tangent{
        (above.x - below.x) / (T{2} * h2),
        (above.y - below.y) / (T{2} * h2),
        (above.z - below.z) / (T{2} * h2)
    };

    const T cx = first_tangent.y * second_tangent.z -
        first_tangent.z * second_tangent.y;
    const T cy = first_tangent.z * second_tangent.x -
        first_tangent.x * second_tangent.z;
    const T cz = first_tangent.x * second_tangent.y -
        first_tangent.y * second_tangent.x;

    return std::sqrt(cx * cx + cy * cy + cz * cz);
}

template <class Mapping, std::floating_point T>
    requires CoordinateMapping2DFor<Mapping, T>
[[nodiscard]] T jacobian(
    const Mapping& mapping,
    T first,
    T second,
    T first_scale,
    T second_scale
) {
    if constexpr (requires { mapping.jacobian(first, second); }) {
        return static_cast<T>(mapping.jacobian(first, second));
    }

    return geometric_jacobian(mapping, first, second, first_scale, second_scale);
}

template <std::floating_point T>
[[nodiscard]] constexpr T jacobian(
    const BasicCartesianCoordinates2D<T>&,
    T,
    T,
    T,
    T
) noexcept {
    return T{1};
}

template <std::floating_point T>
[[nodiscard]] constexpr T jacobian(
    const BasicPolarCoordinates2D<T>&,
    T r,
    T,
    T,
    T
) noexcept {
    return r;
}

template <std::floating_point T>
[[nodiscard]] constexpr T jacobian(
    const BasicAxisymmetricCoordinates2D<T>&,
    T r,
    T,
    T,
    T
) noexcept {
    return T{2} * std::numbers::pi_v<T> * r;
}

template <std::floating_point T>
[[nodiscard]] T jacobian(
    const BasicSphericalAxisymmetricCoordinates2D<T>&,
    T r,
    T theta,
    T,
    T
) noexcept {
    return T{2} * std::numbers::pi_v<T> * r * r * std::sin(theta);
}

template <class Mapping, std::floating_point T>
    requires CoordinateMapping2DFor<Mapping, T>
[[nodiscard]] T first_face_measure(
    const Mapping& mapping,
    T first,
    T second_min,
    T second_max
) {
    if constexpr (requires {
        mapping.first_face_measure(first, second_min, second_max);
    }) {
        return static_cast<T>(mapping.first_face_measure(first, second_min, second_max));
    }

    BasicPhysicalPoint2D<T> previous = mapping.map(first, second_min);
    T measure{};

    for (Size n = 1; n <= default_metric_segments_2d; ++n) {
        const T fraction = static_cast<T>(n) /
            static_cast<T>(default_metric_segments_2d);
        const T second = second_min + (second_max - second_min) * fraction;
        const BasicPhysicalPoint2D<T> current = mapping.map(first, second);

        measure += std::hypot(
            std::hypot(current.x - previous.x, current.y - previous.y),
            current.z - previous.z
        );
        previous = current;
    }

    return measure;
}

template <class Mapping, std::floating_point T>
    requires CoordinateMapping2DFor<Mapping, T>
[[nodiscard]] T second_face_measure(
    const Mapping& mapping,
    T second,
    T first_min,
    T first_max
) {
    if constexpr (requires {
        mapping.second_face_measure(second, first_min, first_max);
    }) {
        return static_cast<T>(mapping.second_face_measure(second, first_min, first_max));
    }

    BasicPhysicalPoint2D<T> previous = mapping.map(first_min, second);
    T measure{};

    for (Size n = 1; n <= default_metric_segments_2d; ++n) {
        const T fraction = static_cast<T>(n) /
            static_cast<T>(default_metric_segments_2d);
        const T first = first_min + (first_max - first_min) * fraction;
        const BasicPhysicalPoint2D<T> current = mapping.map(first, second);

        measure += std::hypot(
            std::hypot(current.x - previous.x, current.y - previous.y),
            current.z - previous.z
        );
        previous = current;
    }

    return measure;
}

template <std::floating_point T>
[[nodiscard]] T first_face_measure(
    const BasicPolarCoordinates2D<T>&,
    T r,
    T a,
    T b
) noexcept {
    return r * (b - a);
}

template <std::floating_point T>
[[nodiscard]] T first_face_measure(
    const BasicAxisymmetricCoordinates2D<T>&,
    T r,
    T a,
    T b
) noexcept {
    return T{2} * std::numbers::pi_v<T> * r * (b - a);
}

template <std::floating_point T>
[[nodiscard]] T second_face_measure(
    const BasicAxisymmetricCoordinates2D<T>&,
    T,
    T a,
    T b
) noexcept {
    return std::numbers::pi_v<T> * (b * b - a * a);
}

template <std::floating_point T>
[[nodiscard]] T first_face_measure(
    const BasicSphericalAxisymmetricCoordinates2D<T>&,
    T r,
    T a,
    T b
) noexcept {
    return T{2} * std::numbers::pi_v<T> * r * r *
        (std::cos(a) - std::cos(b));
}

template <std::floating_point T>
[[nodiscard]] T second_face_measure(
    const BasicSphericalAxisymmetricCoordinates2D<T>&,
    T theta,
    T a,
    T b
) noexcept {
    return std::numbers::pi_v<T> * std::sin(theta) * (b * b - a * a);
}

}  // namespace coordinate_metrics
}  // namespace fvgrid
