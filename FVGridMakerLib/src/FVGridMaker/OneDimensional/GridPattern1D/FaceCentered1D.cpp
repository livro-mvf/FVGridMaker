// ----------------------------------------------------------------------------
// File: FaceCentered1D.cpp
// Project: FVGridMaker
// Version: 0.1.0
// Description: Implements the face-centred one-dimensional grid pattern.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <string>
#include <algorithm>
#include <utility>
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/ErrorCatalog.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/FaceCentered1D.h>

namespace fvgrid {

std::vector<Real> FaceCentered1D::faces_from_centers(
    std::span<const Real> centers,
    Real x_min,
    Real x_max
) {
    require(
        !centers.empty(),
        error_catalog::kInvalidCenterCount,
        FaceCentered1D::id()
    );

    require(
        x_max > x_min,
        error_catalog::kInvalidLength,
        FaceCentered1D::id()
    );

    const bool centers_strictly_increasing =
        std::ranges::adjacent_find(
            centers,
            [](Real left, Real right) {
                return !(right > left);
            }
        ) == centers.end();

    require(
        centers_strictly_increasing,
        error_catalog::kNonIncreasingCenters,
        FaceCentered1D::id()
    );

    require(
        centers.front() > x_min,
        error_catalog::kOutOfRange,
        FaceCentered1D::id()
    );

    require(
        centers.back() < x_max,
        error_catalog::kOutOfRange,
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
    require(
        domain.has_bounds(),
        error_catalog::kInvalidArgument,
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