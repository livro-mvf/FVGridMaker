// ----------------------------------------------------------------------------
// File: VolumeCentered1D.cpp
// Project: FVGridMaker
// Version: 0.1.0
// Description: Implements the volume-centred one-dimensional grid pattern.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/ErrorCatalog.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>

namespace fvgrid {

std::vector<Real> VolumeCentered1D::centers_from_faces(
    std::span<const Real> faces
) {
    require(
        faces.size() >= 2,
        error_catalog::kInvalidFaceCount,
        VolumeCentered1D::id()
    );

    const Size cell_count = faces.size() - 1;
    std::vector<Real> centers(cell_count);

    constexpr Real half = static_cast<Real>(0.5);

    for (Size i = 0; i < cell_count; ++i) {
        centers[i] = half * (faces[i] + faces[i + 1]);
    }

    return centers;
}

}  // namespace fvgrid