// ----------------------------------------------------------------------------
// File: VolumeCentered1D.cpp
// Project: FVGridMaker
// Version: 0.1.0
// Description: Implements the volume-centred one-dimensional grid pattern.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <string>
#include <utility>
#include <vector>

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

AxisGeometry1D VolumeCentered1D::complete_geometry(
    std::vector<Real> faces,
    Domain1D
) {
    std::vector<Real> centers = centers_from_faces(faces);

    return AxisGeometry1D{
        std::move(faces),
        std::move(centers),
        std::string{VolumeCentered1D::name()}
    };
}

}  // namespace fvgrid