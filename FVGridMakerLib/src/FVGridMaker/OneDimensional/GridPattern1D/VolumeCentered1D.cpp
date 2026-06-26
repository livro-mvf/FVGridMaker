// ----------------------------------------------------------------------------
// File: VolumeCentered1D.cpp
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
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
#include <FVGridMaker/ErrorHandling/BuiltInErrors.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/CentersFromFaces1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/ConstantWeight1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Domain1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>

namespace fvgrid {
namespace {

[[nodiscard]] CentersFromFaces1D<ConstantWeight1D>
make_midpoint_reconstructor() {
    return CentersFromFaces1D{ConstantWeight1D{0.5}};
}

void validate_face_count_for_volume_centered(Size count) {
    require<errors::grid::InvalidFaceCount>(
        count >= static_cast<Size>(2),
        VolumeCentered1D::id()
    );
}

}  // namespace

std::vector<Real> VolumeCentered1D::centers_from_faces(
    std::span<const Real> faces
) {
    validate_face_count_for_volume_centered(faces.size());

    std::vector<Real> face_values(faces.begin(), faces.end());

    AxisGeometry1D geometry = make_midpoint_reconstructor().complete_geometry(
        std::move(face_values),
        Domain1D::none()
    );

    return std::move(geometry.centers);
}

AxisGeometry1D VolumeCentered1D::complete_geometry(
    std::vector<Real> faces,
    Domain1D domain
) {
    validate_face_count_for_volume_centered(faces.size());

    AxisGeometry1D geometry = make_midpoint_reconstructor().complete_geometry(
        std::move(faces),
        domain
    );

    geometry.pattern_name = std::string{VolumeCentered1D::name()};

    return geometry;
}

}  // namespace fvgrid