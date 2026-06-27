// ----------------------------------------------------------------------------
// File: Uniform1D.cc
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Implements canonical uniform one-dimensional axis generation.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <cmath>
#include <utility>
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/BuiltInErrors.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/AxisGeometry1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Domain1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>

namespace fvgrid {

Axis1D Uniform1D::make(
    NVol nvol,
    Length length,
    XInit xinit
) {
    validate_input(nvol, length, xinit);

    const Size cell_count = nvol.value();
    const Real dx = length.value() / static_cast<Real>(cell_count);
    const Real x0 = xinit.value();

    std::vector<Real> faces(cell_count + static_cast<Size>(1));

    for (Size i = 0; i <= cell_count; ++i) {
        faces[i] = x0 + static_cast<Real>(i) * dx;
    }

    AxisGeometry1D geometry = VolumeCentered1D::complete_geometry(
        std::move(faces),
        Domain1D::from_length(xinit, length)
    );

    return Axis1D::from_geometry(std::move(geometry));
}

void Uniform1D::validate_input(
    NVol nvol,
    Length length,
    XInit xinit
) {
    require<errors::grid::InvalidNVol>(
        nvol.value() > 0,
        Uniform1D::id()
    );

    require<errors::grid::InvalidLength>(
        std::isfinite(length.value()) &&
            length.value() > static_cast<Real>(0.0),
        Uniform1D::id()
    );

    require<errors::core::InvalidArgument>(
        std::isfinite(xinit.value()),
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