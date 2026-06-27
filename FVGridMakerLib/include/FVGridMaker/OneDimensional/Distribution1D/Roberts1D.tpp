// ----------------------------------------------------------------------------
// File: Roberts1D.tpp
// Project: FVGridMaker
// Description: Template implementation for BasicRoberts1D.
// ----------------------------------------------------------------------------

#pragma once

#include <cmath>

#include <FVGridMaker/ErrorHandling/BuiltInErrors.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>

namespace fvgrid {

template <std::floating_point T>
BasicAxis1D<T> BasicRoberts1D<T>::make(
    NVol nvol,
    BasicLength<T> length,
    BasicXInit<T> xinit,
    BasicBeta<T> beta
) {
    return make(nvol, length, xinit, beta, VolumeCentered1D{});
}

template <std::floating_point T>
void BasicRoberts1D<T>::validate_input(
    NVol nvol,
    BasicLength<T> length,
    BasicXInit<T> xinit,
    BasicBeta<T> beta
) {
    require<errors::grid::InvalidNVol>(
        nvol.value() > 0,
        BasicRoberts1D::id()
    );

    require<errors::grid::InvalidLength>(
        std::isfinite(length.value()) && length.value() > T{0},
        BasicRoberts1D::id()
    );

    require<errors::core::InvalidArgument>(
        std::isfinite(xinit.value()),
        BasicRoberts1D::id()
    );

    require<errors::grid::InvalidBeta>(
        std::isfinite(beta.value()) && beta.value() > T{1} + beta_margin(),
        BasicRoberts1D::id()
    );
}

template <std::floating_point T>
T BasicRoberts1D<T>::normalized_coordinate(T eta, T beta) {
    constexpr T alpha = T{0.5};

    const T ratio = (beta + T{1}) / (beta - T{1});
    const T exponent = (eta - alpha) / (T{1} - alpha);
    const T q = std::pow(ratio, exponent);

    const T numerator =
        (beta + T{2} * alpha) * q + T{2} * alpha - beta;

    const T denominator = (T{2} * alpha + T{1}) * (q + T{1});

    return numerator / denominator;
}

template <std::floating_point T>
BasicAxis1D<T> roberts_axis_1d(
    NVol nvol,
    BasicLength<T> length,
    BasicXInit<T> xinit,
    BasicBeta<T> beta
) {
    return BasicRoberts1D<T>::make(nvol, length, xinit, beta);
}

inline Axis1D roberts_axis_1d(
    NVol nvol,
    Length length,
    XInit xinit,
    Beta beta
) {
    return Roberts1D::make(nvol, length, xinit, beta);
}

template <std::floating_point T>
BasicAxis1D<T> roberts_axis_1d(
    Size nvol,
    T xinit,
    T xfinal,
    T beta
) {
    require<errors::grid::InvalidLength>(
        std::isfinite(xinit) && std::isfinite(xfinal) && xfinal > xinit,
        BasicRoberts1D<T>::id()
    );

    return BasicRoberts1D<T>::make(
        NVol{nvol},
        BasicLength<T>{xfinal - xinit},
        BasicXInit<T>{xinit},
        BasicBeta<T>{beta}
    );
}

inline Axis1D roberts_axis_1d(
    Size nvol,
    double xinit,
    double xfinal,
    double beta
) {
    return roberts_axis_1d<double>(nvol, xinit, xfinal, beta);
}

}  // namespace fvgrid
