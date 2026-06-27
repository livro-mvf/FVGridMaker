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
#include <FVGridMaker/OneDimensional/GridPattern1D/AxisGeometry1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/CoordinateTags1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Domain1D.h>

namespace fvgrid {

template <class Pattern, class T>
concept GridPattern1DFor =
    std::floating_point<T> &&
    requires(
        const Pattern& pattern,
        std::vector<T> coordinates,
        BasicDomain1D<T> domain
    ) {
        typename Pattern::primary_coordinates;

        requires PrimaryCoordinateTag1D<typename Pattern::primary_coordinates>;

        { Pattern::name() } -> std::convertible_to<std::string_view>;

        {
            pattern.complete_geometry(std::move(coordinates), domain)
        } -> std::same_as<BasicAxisGeometry1D<T>>;
    };

template <class Pattern>
concept GridPattern1D = GridPattern1DFor<Pattern, double>;

}  // namespace fvgrid
