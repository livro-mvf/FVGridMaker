// ----------------------------------------------------------------------------
// File: CentersFromFaces1D.h
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Defines weighted center reconstruction from 1D face coordinates.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <cmath>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/ErrorHandling/BuiltInErrors.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/AxisGeometry1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/CoordinateTags1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Domain1D.h>

namespace fvgrid {

template <class WeightRule>
class CentersFromFaces1D final {
public:
    using primary_coordinates = FaceCoordinates1D;

    explicit CentersFromFaces1D(WeightRule weights)
        : weights_{std::move(weights)} {}

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "CentersFromFaces1D",
            "fvgrid.1d.grid_pattern.CentersFromFaces1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view name() noexcept {
        return "CentersFromFaces1D";
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] static constexpr std::string_view primary_coordinates_name()
        noexcept {
        return "faces";
    }

    [[nodiscard]] static constexpr std::string_view secondary_coordinates()
        noexcept {
        return "centers";
    }

    [[nodiscard]] AxisGeometry1D complete_geometry(
        std::vector<Real> faces,
        Domain1D
    ) const {
        validate_face_count(faces.size());

        std::vector<Real> centers;
        centers.reserve(faces.size() - static_cast<Size>(1));

        for (Size p = 0; p + static_cast<Size>(1) < faces.size(); ++p) {
            const Real theta = weights_(p);
            validate_weight(theta);

            centers.push_back(
                (static_cast<Real>(1.0) - theta) * faces[p] +
                theta * faces[p + static_cast<Size>(1)]
            );
        }

        return AxisGeometry1D{
            std::move(faces),
            std::move(centers),
            std::string{name()}
        };
    }

private:
    static void validate_face_count(Size count) {
        require<errors::grid::InvalidFaceCount>(
            count >= static_cast<Size>(2),
            CentersFromFaces1D::id()
        );
    }

    static void validate_weight(Real theta) {
        require<errors::core::InvalidArgument>(
            std::isfinite(theta) &&
            theta > static_cast<Real>(0.0) &&
            theta < static_cast<Real>(1.0),
            CentersFromFaces1D::id()
        );
    }

    WeightRule weights_;
};

template <class WeightRule>
CentersFromFaces1D(WeightRule) -> CentersFromFaces1D<WeightRule>;

}  // namespace fvgrid