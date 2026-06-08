// ----------------------------------------------------------------------------
// File: Uniform1D.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Implements uniform one-dimensional axis generation utilities.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/ErrorCatalog.h>
#include <FVGridMaker/ErrorHandling/ErrorCodes.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

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
        VolumeCentered1D::name()
    );
}

Axis1D Uniform1D::make(
    NVol nvol,
    Length length,
    XInit xinit,
    std::string_view pattern_name
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

    const Size cell_count = nvol.value();
    const Real dx = length.value() / static_cast<Real>(cell_count);
    const Real x0 = xinit.value();

    std::vector<Real> faces(cell_count + 1);

    for (Size i = 0; i <= cell_count; ++i) {
        faces[i] = x0 + static_cast<Real>(i) * dx;
    }

    return Axis1D{std::move(faces), pattern_name};
}

Axis1D uniform_axis_1d(
    NVol nvol,
    Length length,
    XInit xinit
) {
    return Uniform1D::make(nvol, length, xinit);
}

Axis1D uniform_axis_1d(
    NVol nvol,
    Length length,
    XInit xinit,
    std::string_view pattern_name
) {
    return Uniform1D::make(nvol, length, xinit, pattern_name);
}

}  // namespace fvgrid