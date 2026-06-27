// ----------------------------------------------------------------------------
// File: Uniform1D.tpp
// Project: FVGridMaker
// Description: Template implementation for BasicUniform1D.
// ----------------------------------------------------------------------------

#pragma once

#include <cmath>
#include <utility>
#include <vector>

#include <FVGridMaker/ErrorHandling/BuiltInErrors.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/AxisGeometry1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Domain1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>

namespace fvgrid {

template <std::floating_point T>
BasicAxis1D<T> BasicUniform1D<T>::make(
    NVol nvol,
    BasicLength<T> length,
    BasicXInit<T> xinit
) {
    validate_input(nvol, length, xinit);

    const Size cell_count = nvol.value();
    const T dx = length.value() / static_cast<T>(cell_count);
    const T x0 = xinit.value();

    std::vector<T> faces(cell_count + static_cast<Size>(1));

    for (Size i = 0; i <= cell_count; ++i) {
        faces[i] = x0 + static_cast<T>(i) * dx;
    }

    BasicAxisGeometry1D<T> geometry = VolumeCentered1D::complete_geometry(
        std::move(faces),
        BasicDomain1D<T>::from_length(xinit, length)
    );

    return BasicAxis1D<T>::from_geometry(std::move(geometry));
}

template <std::floating_point T>
void BasicUniform1D<T>::validate_input(
    NVol nvol,
    BasicLength<T> length,
    BasicXInit<T> xinit
) {
    require<errors::grid::InvalidNVol>(
        nvol.value() > 0,
        BasicUniform1D::id()
    );

    require<errors::grid::InvalidLength>(
        std::isfinite(length.value()) && length.value() > T{0},
        BasicUniform1D::id()
    );

    require<errors::core::InvalidArgument>(
        std::isfinite(xinit.value()),
        BasicUniform1D::id()
    );
}

template <std::floating_point T>
BasicAxis1D<T> uniform_axis_1d(
    NVol nvol,
    BasicLength<T> length,
    BasicXInit<T> xinit
) {
    return BasicUniform1D<T>::make(nvol, length, xinit);
}

inline Axis1D uniform_axis_1d(
    NVol nvol,
    Length length,
    XInit xinit
) {
    return Uniform1D::make(nvol, length, xinit);
}

template <std::floating_point T>
BasicAxis1D<T> uniform_axis_1d(
    Size nvol,
    T xinit,
    T xfinal
) {
    require<errors::grid::InvalidLength>(
        std::isfinite(xinit) && std::isfinite(xfinal) && xfinal > xinit,
        BasicUniform1D<T>::id()
    );

    return BasicUniform1D<T>::make(
        NVol{nvol},
        BasicLength<T>{xfinal - xinit},
        BasicXInit<T>{xinit}
    );
}

inline Axis1D uniform_axis_1d(
    Size nvol,
    double xinit,
    double xfinal
) {
    return uniform_axis_1d<double>(nvol, xinit, xfinal);
}

}  // namespace fvgrid
