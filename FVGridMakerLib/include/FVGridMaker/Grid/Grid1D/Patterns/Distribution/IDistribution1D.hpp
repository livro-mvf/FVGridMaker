// ----------------------------------------------------------------------------
/* File: IDistribution1D.hpp
 * Project: FVGridMaker
 * Version: 1.0
 * Description: Traits and helpers for 1D grid distributions.
 * License: GNU GPL v3
 */
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// includes FVGridMaker (ordem alfabética por caminho)
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/Common/namespace.hpp>
#include <FVGridMaker/Core/Common/types.hpp>
#include <FVGridMaker/Grid/Grid1D/Patterns/Distribution/ConceptsDistribution.hpp>

// ----------------------------------------------------------------------------
// includes C++ (ordem alfabética)
// ----------------------------------------------------------------------------
#include <cstddef>

/**
 * @file IDistribution1D.hpp
 * @brief Traits and helpers for 1D grid distributions.
 * @ingroup Grid1D
 */

FVGRIDMAKER_NAMESPACE_OPEN
GRID_NAMESPACE_OPEN
GRID1D_NAMESPACE_OPEN
PATTERNS_NAMESPACE_OPEN
DISTRIBUTION_NAMESPACE_OPEN

/**
 * @brief Compile-time traits for 1D distributions.
 */
template <typename D>
struct Distribution1DTraits {
    static constexpr bool is_valid = Distribution1D<D>;
    using distribution_type = D;
    using real_type = core::Real;
    using size_type = std::size_t;
};

/**
 * @brief Convenience variable template to check Distribution1D.
 */
template <typename D>
inline constexpr bool is_distribution_1d_v =
    Distribution1DTraits<D>::is_valid;

/**
 * @brief Helper alias for explicit implementation types.
 */
template <Distribution1D D>
using Distribution1DImpl = D;

DISTRIBUTION_NAMESPACE_CLOSE
PATTERNS_NAMESPACE_CLOSE
GRID1D_NAMESPACE_CLOSE
GRID_NAMESPACE_CLOSE
FVGRIDMAKER_NAMESPACE_CLOSE
