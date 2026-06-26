// ----------------------------------------------------------------------------
// File: Random1D.cc
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Implements random one-dimensional axis generation utilities.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <limits>
#include <random>
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/BuiltInErrors.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Random1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>

namespace fvgrid {

Axis1D Random1D::make(
    NVol nvol,
    Length length,
    XInit xinit,
    Seed seed
) {
    return make(
        nvol,
        length,
        xinit,
        seed,
        MinSpacing{static_cast<Real>(0.0)},
        VolumeCentered1D{}
    );
}

Axis1D Random1D::make(
    NVol nvol,
    Length length,
    XInit xinit,
    Seed seed,
    MinSpacing min_spacing
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

void Random1D::validate_input(
    NVol nvol,
    Length length,
    MinSpacing min_spacing,
    Size interval_count
) {
    require<errors::grid::InvalidNVol>(
        nvol.value() > 0,
        Random1D::id()
    );

    require<errors::grid::InvalidLength>(
        length.value() > static_cast<Real>(0.0),
        Random1D::id()
    );

    require<errors::grid::InvalidMinSpacing>(
        min_spacing.value() >= static_cast<Real>(0.0),
        Random1D::id()
    );

    const Real required_minimum =
        min_spacing.value() * static_cast<Real>(interval_count);

    require<errors::grid::ImpossibleMinSpacing>(
        required_minimum <= length.value(),
        Random1D::id()
    );
}

std::vector<Real> Random1D::build_random_partition(
    Size interval_count,
    Real total_length,
    Real min_spacing,
    std::mt19937_64& random_engine
) {
    std::vector<Real> weights(interval_count);
    std::vector<Real> partition(interval_count);

    const Real required_minimum =
        min_spacing * static_cast<Real>(interval_count);

    const Real remaining_length = total_length - required_minimum;

    if (remaining_length == static_cast<Real>(0.0)) {
        for (Size i = 0; i < interval_count; ++i) {
            partition[i] = min_spacing;
        }

        return partition;
    }

    std::uniform_real_distribution<Real> distribution{
        static_cast<Real>(0.0),
        static_cast<Real>(1.0)
    };

    Real weight_sum = static_cast<Real>(0.0);

    for (Size i = 0; i < interval_count; ++i) {
        weights[i] =
            distribution(random_engine) + std::numeric_limits<Real>::epsilon();
        weight_sum += weights[i];
    }

    for (Size i = 0; i < interval_count; ++i) {
        partition[i] =
            min_spacing + remaining_length * weights[i] / weight_sum;
    }

    return partition;
}

Axis1D random_axis_1d(
    NVol nvol,
    Length length,
    XInit xinit,
    Seed seed
) {
    return Random1D::make(
        nvol,
        length,
        xinit,
        seed
    );
}

Axis1D random_axis_1d(
    NVol nvol,
    Length length,
    XInit xinit,
    Seed seed,
    MinSpacing min_spacing
) {
    return Random1D::make(
        nvol,
        length,
        xinit,
        seed,
        min_spacing
    );
}

}  // namespace fvgrid