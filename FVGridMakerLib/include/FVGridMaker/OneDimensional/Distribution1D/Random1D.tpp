// ----------------------------------------------------------------------------
// File: Random1D.tpp
// Project: FVGridMaker
// Description: Template implementation for BasicRandom1D.
// ----------------------------------------------------------------------------

#pragma once

#include <cmath>
#include <limits>
#include <random>
#include <vector>

#include <FVGridMaker/ErrorHandling/BuiltInErrors.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>

namespace fvgrid {

template <std::floating_point T>
BasicAxis1D<T> BasicRandom1D<T>::make(
    NVol nvol,
    BasicLength<T> length,
    BasicXInit<T> xinit,
    Seed seed
) {
    return make(
        nvol,
        length,
        xinit,
        seed,
        BasicMinSpacing<T>{T{}},
        VolumeCentered1D{}
    );
}

template <std::floating_point T>
BasicAxis1D<T> BasicRandom1D<T>::make(
    NVol nvol,
    BasicLength<T> length,
    BasicXInit<T> xinit,
    Seed seed,
    BasicMinSpacing<T> min_spacing
) {
    return make(
        nvol,
        length,
        xinit,
        seed,
        min_spacing,
        VolumeCentered1D{}
    );
}

template <std::floating_point T>
void BasicRandom1D<T>::validate_input(
    NVol nvol,
    BasicLength<T> length,
    BasicXInit<T> xinit,
    BasicMinSpacing<T> min_spacing,
    Size interval_count
) {
    require<errors::grid::InvalidNVol>(
        nvol.value() > 0,
        BasicRandom1D::id()
    );

    require<errors::grid::InvalidLength>(
        std::isfinite(length.value()) && length.value() > T{0},
        BasicRandom1D::id()
    );

    require<errors::core::InvalidArgument>(
        std::isfinite(xinit.value()),
        BasicRandom1D::id()
    );

    require<errors::grid::InvalidMinSpacing>(
        std::isfinite(min_spacing.value()) && min_spacing.value() >= T{0},
        BasicRandom1D::id()
    );

    const T required_minimum =
        min_spacing.value() * static_cast<T>(interval_count);

    require<errors::grid::ImpossibleMinSpacing>(
        required_minimum <= length.value(),
        BasicRandom1D::id()
    );
}

template <std::floating_point T>
std::vector<T> BasicRandom1D<T>::build_random_partition(
    Size interval_count,
    T total_length,
    T min_spacing,
    std::mt19937_64& random_engine
) {
    std::vector<T> partition(interval_count);

    const T required_minimum = min_spacing * static_cast<T>(interval_count);
    const T remaining_length = total_length - required_minimum;

    if (remaining_length == T{0}) {
        for (Size i = 0; i < interval_count; ++i) {
            partition[i] = min_spacing;
        }

        return partition;
    }

    std::uniform_real_distribution<T> distribution{T{0}, T{1}};

    T weight_sum = T{0};

    for (Size i = 0; i < interval_count; ++i) {
        partition[i] = distribution(random_engine) +
            std::numeric_limits<T>::epsilon();
        weight_sum += partition[i];
    }

    for (Size i = 0; i < interval_count; ++i) {
        partition[i] =
            min_spacing + remaining_length * partition[i] / weight_sum;
    }

    return partition;
}

template <std::floating_point T>
BasicAxis1D<T> random_axis_1d(
    NVol nvol,
    BasicLength<T> length,
    BasicXInit<T> xinit,
    Seed seed
) {
    return BasicRandom1D<T>::make(nvol, length, xinit, seed);
}

inline Axis1D random_axis_1d(
    NVol nvol,
    Length length,
    XInit xinit,
    Seed seed
) {
    return Random1D::make(nvol, length, xinit, seed);
}

template <std::floating_point T>
BasicAxis1D<T> random_axis_1d(
    NVol nvol,
    BasicLength<T> length,
    BasicXInit<T> xinit,
    Seed seed,
    BasicMinSpacing<T> min_spacing
) {
    return BasicRandom1D<T>::make(nvol, length, xinit, seed, min_spacing);
}

inline Axis1D random_axis_1d(
    NVol nvol,
    Length length,
    XInit xinit,
    Seed seed,
    MinSpacing min_spacing
) {
    return Random1D::make(nvol, length, xinit, seed, min_spacing);
}

template <std::floating_point T>
BasicAxis1D<T> random_axis_1d(
    Size nvol,
    T xinit,
    T xfinal,
    UInt64 seed
) {
    require<errors::grid::InvalidLength>(
        std::isfinite(xinit) && std::isfinite(xfinal) && xfinal > xinit,
        BasicRandom1D<T>::id()
    );

    return BasicRandom1D<T>::make(
        NVol{nvol},
        BasicLength<T>{xfinal - xinit},
        BasicXInit<T>{xinit},
        Seed{seed}
    );
}

inline Axis1D random_axis_1d(
    Size nvol,
    double xinit,
    double xfinal,
    UInt64 seed
) {
    return random_axis_1d<double>(nvol, xinit, xfinal, seed);
}

}  // namespace fvgrid
