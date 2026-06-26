// ----------------------------------------------------------------------------
// File: GridPatternConcept1D.h
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Defines the concept for one-dimensional grid pattern objects.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <concepts>
#include <string_view>
#include <utility>
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/AxisGeometry1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/CoordinateTags1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Domain1D.h>

namespace fvgrid {

template <class Pattern>
concept GridPattern1D =
    requires(
        const Pattern& pattern,
        std::vector<Real> coordinates,
        Domain1D domain
    ) {
        typename Pattern::primary_coordinates;

        requires PrimaryCoordinateTag1D<typename Pattern::primary_coordinates>;

        {
            pattern.name()
        } -> std::convertible_to<std::string_view>;

        {
            pattern.complete_geometry(std::move(coordinates), domain)
        } -> std::same_as<AxisGeometry1D>;
    };

}  // namespace fvgrid