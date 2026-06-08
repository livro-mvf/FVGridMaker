// ----------------------------------------------------------------------------
// File: Uniform1D.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Implements uniform one-dimensional axis generation utilities.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/ErrorCatalog.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>

namespace fvgrid {

Axis1D Uniform1D::make(
    NVol nvol,
    Length length,
    XInit xinit
) {
    return make(
        nvol,
        length,
        xinit,
        VolumeCentered1D{}
    );
}

void Uniform1D::validate_input(
    NVol nvol,
    Length length
) {
    require(
        nvol.value() > 0,
        error_catalog::kInvalidNVol,
        Uniform1D::id()
    );

    require(
        length.value() > static_cast<Real>(0.0),
        error_catalog::kInvalidLength,
        Uniform1D::id()
    );
}

Axis1D uniform_axis_1d(
    NVol nvol,
    Length length,
    XInit xinit
) {
    return Uniform1D::make(
        nvol,
        length,
        xinit
    );
}

}  // namespace fvgrid