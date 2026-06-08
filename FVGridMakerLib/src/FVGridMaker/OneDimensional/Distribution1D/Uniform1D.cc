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
#include <utility>
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/ErrorCatalog.h>
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

    // Volume-centred uniform construction:
    //
    //   1. Uniform1D generates the face coordinates.
    //   2. VolumeCentered1D reconstructs the centre coordinates from faces.
    //   3. Axis1D stores the completed geometry and computes the metrics.
    //
    // This file intentionally implements only the volume-centred construction
    // path. Face-centred uniform generation will be reintroduced later through
    // a dedicated construction path.
    for (Size i = 0; i <= cell_count; ++i) {
        faces[i] = x0 + static_cast<Real>(i) * dx;
    }

    std::vector<Real> centers = VolumeCentered1D::centers_from_faces(faces);

    return Axis1D{
        std::move(faces),
        std::move(centers),
        pattern_name
    };
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