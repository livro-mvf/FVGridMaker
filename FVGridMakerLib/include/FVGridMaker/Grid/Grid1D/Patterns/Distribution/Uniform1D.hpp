// ----------------------------------------------------------------------------
// File: Uniform1D.hpp
// Project: FVGridMaker
// Version: 1.0
// Description: Uniform 1D grid distribution functor.
// License: GNU GPL v3
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
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <span>

/**
 * @file Uniform1D.hpp
 * @brief Uniform 1D distribution functor.
 * @ingroup Grid1D
 */

FVGRIDMAKER_NAMESPACE_OPEN
GRID_NAMESPACE_OPEN
GRID1D_NAMESPACE_OPEN
PATTERNS_NAMESPACE_OPEN
DISTRIBUTION_NAMESPACE_OPEN

/**
 * @brief Uniform 1D distribution.
 *
 * Generates a strictly uniform grid on [A, B] with N cells:
 *  - Faces: N + 1 positions, including A and B;
 *  - Centers: N positions at midpoints of each cell.
 */
struct Uniform1D {
    using Real = core::Real;
    using Size = std::size_t;

    /**
     * @brief Generate face coordinates for a uniform grid.
     *
     * @param n       Number of physical cells.
     * @param a       Left domain boundary.
     * @param b       Right domain boundary.
     * @param xf      Span where face coordinates will be stored (size >= n+1).
     * @param seed    Unused (kept for API compatibility).
     * @param dx_min  Unused (uniform grid has fixed spacing).
     */
    void makeFaces(Size n,
                   Real a,
                   Real b,
                   std::span<Real> xf,
                   std::uint64_t /*seed*/ = 0,
                   Real /*dx_min*/ = Real(0)) const noexcept {
        assert(n > Size(0));
        assert(xf.size() >= n + Size(1));

        const Real length = b - a;
        const Real dx = length / static_cast<Real>(n);

        for (Size i = 0; i <= n; ++i) {
            xf[i] = a + dx * static_cast<Real>(i);
        }
    }

    /**
     * @brief Generate cell-center coordinates for a uniform grid.
     *
     * @param n       Number of physical cells.
     * @param a       Left domain boundary.
     * @param b       Right domain boundary.
     * @param xc      Span where center coordinates will be stored (size >= n).
     * @param seed    Unused (kept for API compatibility).
     * @param dx_min  Unused (uniform grid has fixed spacing).
     */
    void makeCenters(Size n,
                     Real a,
                     Real b,
                     std::span<Real> xc,
                     std::uint64_t /*seed*/ = 0,
                     Real /*dx_min*/ = Real(0)) const noexcept {
        assert(n > Size(0));
        assert(xc.size() >= n);

        const Real length = b - a;
        const Real dx = length / static_cast<Real>(n);

        for (Size i = 0; i < n; ++i) {
            const Real x_left = a + dx * static_cast<Real>(i);
            xc[i] = x_left + dx * Real(0.5);
        }
    }
};

static_assert(
    Distribution1D<Uniform1D>,
    "Uniform1D must satisfy the Distribution1D concept.");

DISTRIBUTION_NAMESPACE_CLOSE
PATTERNS_NAMESPACE_CLOSE
GRID1D_NAMESPACE_CLOSE
GRID_NAMESPACE_CLOSE
FVGRIDMAKER_NAMESPACE_CLOSE
