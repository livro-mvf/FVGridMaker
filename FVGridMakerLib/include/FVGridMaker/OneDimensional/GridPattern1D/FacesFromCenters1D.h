// ----------------------------------------------------------------------------
// File: FacesFromCenters1D.h
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Defines weighted face reconstruction from 1D centre coordinates.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

#include <algorithm>
#include <cmath>
#include <concepts>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

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

    template <std::floating_point T>
    [[nodiscard]] BasicAxisGeometry1D<T> complete_geometry(
        std::vector<T> centers,
        BasicDomain1D<T> domain
    ) const {
        validate_domain(domain);
        validate_centers(centers, domain);

        const Size cell_count = centers.size();

        std::vector<T> faces(cell_count + static_cast<Size>(1));
        faces.front() = domain.xmin();
        faces.back() = domain.xmax();

        for (Size p = 1; p < cell_count; ++p) {
            const T theta = static_cast<T>(weights_(p));
            validate_weight(theta);

            faces[p] =
                (T{1} - theta) * centers[p - static_cast<Size>(1)] +
                theta * centers[p];
        }

        return BasicAxisGeometry1D<T>{
            std::move(faces),
            std::move(centers),
            std::string{name()}
        };
    }

private:
    template <std::floating_point T>
    static void validate_domain(BasicDomain1D<T> domain) {
        require<errors::core::InvalidArgument>(
            domain.has_bounds(),
            FacesFromCenters1D::id()
        );

        require<errors::grid::InvalidLength>(
            domain.xmax() > domain.xmin(),
            FacesFromCenters1D::id()
        );
    }

    template <std::floating_point T>
    static void validate_centers(
        const std::vector<T>& centers,
        BasicDomain1D<T> domain
    ) {
        require<errors::grid::InvalidCenterCount>(
            !centers.empty(),
            FacesFromCenters1D::id()
        );

        const bool centers_strictly_increasing =
            std::ranges::adjacent_find(
                centers,
                [](T left, T right) {
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

    template <std::floating_point T>
    static void validate_weight(T theta) {
        require<errors::core::InvalidArgument>(
            std::isfinite(theta) && theta > T{0} && theta < T{1},
            FacesFromCenters1D::id()
        );
    }

    WeightRule weights_;
};

template <class WeightRule>
FacesFromCenters1D(WeightRule) -> FacesFromCenters1D<WeightRule>;

}  // namespace fvgrid
