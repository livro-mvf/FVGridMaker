// ----------------------------------------------------------------------------
// File: FacesFromCenters1D.h
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Defines weighted face reconstruction from 1D centre coordinates.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <algorithm>
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
class FacesFromCenters1D final {
public:
    using primary_coordinates = CenterCoordinates1D;

    explicit FacesFromCenters1D(WeightRule weights)
        : weights_{std::move(weights)} {}

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "FacesFromCenters1D",
            "fvgrid.1d.grid_pattern.FacesFromCenters1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view name() noexcept {
        return "FacesFromCenters1D";
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] static constexpr std::string_view primary_coordinates_name()
        noexcept {
        return "centers";
    }

    [[nodiscard]] static constexpr std::string_view secondary_coordinates()
        noexcept {
        return "faces";
    }

    [[nodiscard]] AxisGeometry1D complete_geometry(
        std::vector<Real> centers,
        Domain1D domain
    ) const {
        validate_domain(domain);
        validate_centers(centers, domain);

        const Size cell_count = centers.size();

        std::vector<Real> faces(cell_count + static_cast<Size>(1));
        faces.front() = domain.xmin();
        faces.back() = domain.xmax();

        for (Size p = 1; p < cell_count; ++p) {
            const Real theta = weights_(p);
            validate_weight(theta);

            faces[p] =
                (static_cast<Real>(1.0) - theta) *
                centers[p - static_cast<Size>(1)] +
                theta * centers[p];
        }

        return AxisGeometry1D{
            std::move(faces),
            std::move(centers),
            std::string{name()}
        };
    }

private:
    static void validate_domain(Domain1D domain) {
        require<errors::core::InvalidArgument>(
            domain.has_bounds(),
            FacesFromCenters1D::id()
        );

        require<errors::grid::InvalidLength>(
            domain.xmax() > domain.xmin(),
            FacesFromCenters1D::id()
        );
    }

    static void validate_centers(
        const std::vector<Real>& centers,
        Domain1D domain
    ) {
        require<errors::grid::InvalidCenterCount>(
            !centers.empty(),
            FacesFromCenters1D::id()
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
            FacesFromCenters1D::id()
        );

        require<errors::core::OutOfRange>(
            centers.front() > domain.xmin(),
            FacesFromCenters1D::id()
        );

        require<errors::core::OutOfRange>(
            centers.back() < domain.xmax(),
            FacesFromCenters1D::id()
        );
    }

    static void validate_weight(Real theta) {
        require<errors::core::InvalidArgument>(
            std::isfinite(theta) &&
            theta > static_cast<Real>(0.0) &&
            theta < static_cast<Real>(1.0),
            FacesFromCenters1D::id()
        );
    }

    WeightRule weights_;
};

template <class WeightRule>
FacesFromCenters1D(WeightRule) -> FacesFromCenters1D<WeightRule>;

}  // namespace fvgrid