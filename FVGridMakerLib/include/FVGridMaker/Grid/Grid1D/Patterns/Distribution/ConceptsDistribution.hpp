// ----------------------------------------------------------------------------
// File: ConceptsDistribution.hpp
// Project: FVGridMaker
// Version: 1.0
// Description: C++20 concept for 1D grid distribution functors.
// License: GNU GPL v3
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// includes FVGridMaker (ordem alfabética por caminho)
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/Common/namespace.hpp>
#include <FVGridMaker/Core/Common/types.hpp>

// ----------------------------------------------------------------------------
// includes C++ 
// ----------------------------------------------------------------------------
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <span>
#include <type_traits>

/**
 * @file ConceptsDistribution.hpp
 * @brief Concept for 1D grid distributions (functor-based).
 * @ingroup Grid1D
 */

FVGRIDMAKER_NAMESPACE_OPEN
GRID_NAMESPACE_OPEN
GRID1D_NAMESPACE_OPEN
PATTERNS_NAMESPACE_OPEN
DISTRIBUTION_NAMESPACE_OPEN

using Real  = core::Real;
using Index = core::Index;

/**
 * @brief Concept for 1D grid distribution functors.
 *
 * A valid 1D distribution must:
 *  - be default-constructible;
 *  - implement makeFaces and makeCenters with the given signature;
 *  - operate on spans (SoA-friendly, no ownership of storage).
 */
template <typename D>
concept Distribution1D =
    std::is_default_constructible_v<D> &&
    requires(D d,
             Index n,
             Real a,
             Real b,
             std::span<Real> v,
             std::uint64_t seed,
             Real dx_min) {
        { d.makeFaces(n, a, b, v, seed, dx_min) } -> std::same_as<void>;
        { d.makeCenters(n, a, b, v, seed, dx_min) } -> std::same_as<void>;
    };

DISTRIBUTION_NAMESPACE_CLOSE
PATTERNS_NAMESPACE_CLOSE
GRID1D_NAMESPACE_CLOSE
GRID_NAMESPACE_CLOSE
FVGRIDMAKER_NAMESPACE_CLOSE
