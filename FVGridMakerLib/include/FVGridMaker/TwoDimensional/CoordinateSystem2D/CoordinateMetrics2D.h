// ----------------------------------------------------------------------------
// File: CoordinateMetrics2D.h
// Project: FVGridMaker
// Description: Physical measures for supported two-dimensional coordinate maps.
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

#include <algorithm>
#include <cmath>
#include <numbers>
#include <string_view>

#include <FVGridMaker/TwoDimensional/CoordinateSystem2D/CoordinateSystem2D.h>

namespace fvgrid {

struct PhysicalVector2D final {
    Real x{};
    Real y{};
    Real z{};
};

struct SphericalAxisymmetricCoordinates2D final {
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

    [[nodiscard]] PhysicalPoint2D map(Real r, Real theta) const noexcept {
        return {r * std::sin(theta), r * std::cos(theta), Real{}};
    }

    [[nodiscard]] Real cell_measure(CoordinateCell2D c) const noexcept {
        return (Real{2} * std::numbers::pi_v<Real> / Real{3})
             * (c.first_max * c.first_max * c.first_max
                - c.first_min * c.first_min * c.first_min)
             * (std::cos(c.second_min) - std::cos(c.second_max));
    }
};

using CylindricalAxisymmetricCoordinates2D = AxisymmetricCoordinates2D;
using CylindricalPolarCoordinates2D = PolarCoordinates2D;

namespace coordinate_metrics {

inline constexpr Size default_metric_segments_2d = 32;

template <CoordinateMapping2D Mapping>
[[nodiscard]] Real geometric_jacobian(
    const Mapping& mapping,
    Real first,
    Real second,
    Real first_scale,
    Real second_scale
) {
    const Real h1 = std::max(
        std::abs(first_scale) * Real{1e-6},
        Real{1e-8}
    );
    const Real h2 = std::max(
        std::abs(second_scale) * Real{1e-6},
        Real{1e-8}
    );

    const PhysicalPoint2D left = mapping.map(first - h1, second);
    const PhysicalPoint2D right = mapping.map(first + h1, second);
    const PhysicalPoint2D below = mapping.map(first, second - h2);
    const PhysicalPoint2D above = mapping.map(first, second + h2);

    const PhysicalVector2D first_tangent{
        (right.x - left.x) / (Real{2} * h1),
        (right.y - left.y) / (Real{2} * h1),
        (right.z - left.z) / (Real{2} * h1)
    };
    const PhysicalVector2D second_tangent{
        (above.x - below.x) / (Real{2} * h2),
        (above.y - below.y) / (Real{2} * h2),
        (above.z - below.z) / (Real{2} * h2)
    };

    const Real cx = first_tangent.y * second_tangent.z
                  - first_tangent.z * second_tangent.y;
    const Real cy = first_tangent.z * second_tangent.x
                  - first_tangent.x * second_tangent.z;
    const Real cz = first_tangent.x * second_tangent.y
                  - first_tangent.y * second_tangent.x;

    return std::sqrt(cx * cx + cy * cy + cz * cz);
}

template <CoordinateMapping2D Mapping>
[[nodiscard]] Real jacobian(
    const Mapping& mapping,
    Real first,
    Real second,
    Real first_scale,
    Real second_scale
) {
    if constexpr (requires { mapping.jacobian(first, second); }) {
        return mapping.jacobian(first, second);
    }

    return geometric_jacobian(
        mapping,
        first,
        second,
        first_scale,
        second_scale
    );
}

[[nodiscard]] inline constexpr Real jacobian(
    const CartesianCoordinates2D&,
    Real,
    Real,
    Real,
    Real
) noexcept {
    return Real{1};
}

[[nodiscard]] inline constexpr Real jacobian(
    const PolarCoordinates2D&,
    Real r,
    Real,
    Real,
    Real
) noexcept {
    return r;
}

[[nodiscard]] inline constexpr Real jacobian(
    const AxisymmetricCoordinates2D&,
    Real r,
    Real,
    Real,
    Real
) noexcept {
    return Real{2} * std::numbers::pi_v<Real> * r;
}

[[nodiscard]] inline Real jacobian(
    const SphericalAxisymmetricCoordinates2D&,
    Real r,
    Real theta,
    Real,
    Real
) noexcept {
    return Real{2} * std::numbers::pi_v<Real> * r * r * std::sin(theta);
}

template <CoordinateMapping2D Mapping>
[[nodiscard]] Real first_face_measure(
    const Mapping& mapping,
    Real first,
    Real second_min,
    Real second_max
) {
    if constexpr (requires {
        mapping.first_face_measure(first, second_min, second_max);
    }) {
        return mapping.first_face_measure(first, second_min, second_max);
    }

    PhysicalPoint2D previous = mapping.map(first, second_min);
    Real measure{};

    for (Size n = 1; n <= default_metric_segments_2d; ++n) {
        const Real fraction = static_cast<Real>(n)
                            / static_cast<Real>(default_metric_segments_2d);
        const Real second = second_min + (second_max - second_min) * fraction;
        const PhysicalPoint2D current = mapping.map(first, second);

        measure += std::hypot(
            std::hypot(current.x - previous.x, current.y - previous.y),
            current.z - previous.z
        );
        previous = current;
    }

    return measure;
}

template <CoordinateMapping2D Mapping>
[[nodiscard]] Real second_face_measure(
    const Mapping& mapping,
    Real second,
    Real first_min,
    Real first_max
) {
    if constexpr (requires {
        mapping.second_face_measure(second, first_min, first_max);
    }) {
        return mapping.second_face_measure(second, first_min, first_max);
    }

    PhysicalPoint2D previous = mapping.map(first_min, second);
    Real measure{};

    for (Size n = 1; n <= default_metric_segments_2d; ++n) {
        const Real fraction = static_cast<Real>(n)
                            / static_cast<Real>(default_metric_segments_2d);
        const Real first = first_min + (first_max - first_min) * fraction;
        const PhysicalPoint2D current = mapping.map(first, second);

        measure += std::hypot(
            std::hypot(current.x - previous.x, current.y - previous.y),
            current.z - previous.z
        );
        previous = current;
    }

    return measure;
}

[[nodiscard]] inline Real first_face_measure(
    const PolarCoordinates2D&,
    Real r,
    Real a,
    Real b
) noexcept {
    return r * (b - a);
}

[[nodiscard]] inline Real first_face_measure(
    const AxisymmetricCoordinates2D&,
    Real r,
    Real a,
    Real b
) noexcept {
    return Real{2} * std::numbers::pi_v<Real> * r * (b - a);
}

[[nodiscard]] inline Real second_face_measure(
    const AxisymmetricCoordinates2D&,
    Real,
    Real a,
    Real b
) noexcept {
    return std::numbers::pi_v<Real> * (b * b - a * a);
}

[[nodiscard]] inline Real first_face_measure(
    const SphericalAxisymmetricCoordinates2D&,
    Real r,
    Real a,
    Real b
) noexcept {
    return Real{2} * std::numbers::pi_v<Real>
         * r * r * (std::cos(a) - std::cos(b));
}

[[nodiscard]] inline Real second_face_measure(
    const SphericalAxisymmetricCoordinates2D&,
    Real theta,
    Real a,
    Real b
) noexcept {
    return std::numbers::pi_v<Real> * std::sin(theta) * (b * b - a * a);
}

}  // namespace coordinate_metrics
}  // namespace fvgrid
