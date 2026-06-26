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
#include <FVGridMaker/OneDimensional/GridPattern1D/ConstantWeight1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Domain1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/FaceCentered1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/FacesFromCenters1D.h>

namespace fvgrid {
namespace {

[[nodiscard]] FacesFromCenters1D<ConstantWeight1D>
make_midpoint_reconstructor() {
    return FacesFromCenters1D{ConstantWeight1D{0.5}};
}

void validate_domain_for_face_centered(Domain1D domain) {
    require<errors::core::InvalidArgument>(
        domain.has_bounds(),
        FaceCentered1D::id()
    );

    require<errors::grid::InvalidLength>(
        domain.xmax() > domain.xmin(),
        FaceCentered1D::id()
    );
}

void validate_centers_for_face_centered(
    std::span<const Real> centers,
    Real x_min,
    Real x_max
) {
    require<errors::grid::InvalidCenterCount>(
        !centers.empty(),
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
}

}  // namespace

std::vector<Real> FaceCentered1D::faces_from_centers(
    std::span<const Real> centers,
    Real x_min,
    Real x_max
) {
    validate_domain_for_face_centered(
        Domain1D::from_bounds(XInit{x_min}, XFinal{x_max})
    );

    validate_centers_for_face_centered(
        centers,
        x_min,
        x_max
    );

    std::vector<Real> center_values(centers.begin(), centers.end());

    AxisGeometry1D geometry = make_midpoint_reconstructor().complete_geometry(
        std::move(center_values),
        Domain1D::from_bounds(XInit{x_min}, XFinal{x_max})
    );

    return std::move(geometry.faces);
}

AxisGeometry1D FaceCentered1D::complete_geometry(
    std::vector<Real> centers,
    Domain1D domain
) {
    validate_domain_for_face_centered(domain);

    validate_centers_for_face_centered(
        centers,
        domain.xmin(),
        domain.xmax()
    );

    AxisGeometry1D geometry = make_midpoint_reconstructor().complete_geometry(
        std::move(centers),
        domain
    );

    geometry.pattern_name = std::string{FaceCentered1D::name()};

    return geometry;
}

}  // namespace fvgrid