// ----------------------------------------------------------------------------
// File: FaceCentered1D.cpp
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Implements the face-centred one-dimensional grid pattern.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <algorithm>
#include <string>
#include <utility>
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/BuiltInErrors.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/FaceCentered1D.h>

namespace fvgrid {

std::vector<Real> FaceCentered1D::faces_from_centers(
    std::span<const Real> centers,
    Real x_min,
    Real x_max
) {
    require<errors::grid::InvalidCenterCount>(
        !centers.empty(),
        FaceCentered1D::id()
    );

    require<errors::grid::InvalidLength>(
        x_max > x_min,
        FaceCentered1D::id()
    );

    const bool centers_strictly_increasing =
        std::ranges::adjacent_find(
            centers,
            [](Real left, Real right) {
                return !(right > left);
            }
        ) == centers.end();

    require<errors::grid::NonIncreasingCenters>(
        centers_strictly_increasing,
        FaceCentered1D::id()
    );

    require<errors::core::OutOfRange>(
        centers.front() > x_min,
        FaceCentered1D::id()
    );

    require<errors::core::OutOfRange>(
        centers.back() < x_max,
        FaceCentered1D::id()
    );

    const Size cell_count = centers.size();

    std::vector<Real> faces(cell_count + 1);

    faces.front() = x_min;
    faces.back() = x_max;

    for (Size i = 1; i < cell_count; ++i) {
        faces[i] = static_cast<Real>(0.5) * (centers[i - 1] + centers[i]);
    }

    return faces;
}

AxisGeometry1D FaceCentered1D::complete_geometry(
    std::vector<Real> centers,
    Domain1D domain
) {
    require<errors::core::InvalidArgument>(
        domain.has_bounds(),
        FaceCentered1D::id()
    );

    std::vector<Real> faces = faces_from_centers(
        centers,
        domain.xmin(),
        domain.xmax()
    );

    return AxisGeometry1D{
        std::move(faces),
        std::move(centers),
        std::string{FaceCentered1D::name()}
    };
}

}  // namespace fvgrid