// ----------------------------------------------------------------------------
// File: Roberts1D.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Implements Roberts one-dimensional axis generation utilities.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <cmath>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/ErrorCatalog.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Roberts1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>

namespace fvgrid {

Axis1D Roberts1D::make(
    NVol nvol,
    Length length,
    XInit xinit,
    Beta beta
) {
    return make(
        nvol,
        length,
        xinit,
        beta,
        VolumeCentered1D{}
    );
}

void Roberts1D::validate_input(
    NVol nvol,
    Length length,
    Beta beta
) {
    require(
        nvol.value() > 0,
        error_catalog::kInvalidNVol,
        Roberts1D::id()
    );

    require(
        length.value() > static_cast<Real>(0.0),
        error_catalog::kInvalidLength,
        Roberts1D::id()
    );

    require(
        beta.value() > static_cast<Real>(1.0),
        error_catalog::kInvalidArgument,
        Roberts1D::id()
    );
}

Real Roberts1D::normalized_coordinate(
    Real eta,
    Real beta
) {
    constexpr Real alpha = static_cast<Real>(0.5);

    const Real ratio =
        (beta + static_cast<Real>(1.0)) /
        (beta - static_cast<Real>(1.0));

    const Real exponent = (eta - alpha) / (static_cast<Real>(1.0) - alpha);
    const Real q = std::pow(ratio, exponent);

    const Real numerator =
        (beta + static_cast<Real>(2.0) * alpha) * q +
        static_cast<Real>(2.0) * alpha -
        beta;

    const Real denominator =
        (static_cast<Real>(2.0) * alpha + static_cast<Real>(1.0)) *
        (q + static_cast<Real>(1.0));

    return numerator / denominator;
}

Axis1D roberts_axis_1d(
    NVol nvol,
    Length length,
    XInit xinit,
    Beta beta
) {
    return Roberts1D::make(
        nvol,
        length,
        xinit,
        beta
    );
}

}  // namespace fvgrid
