// ----------------------------------------------------------------------------
// File: Random1D.hpp
// Project: FVGridMaker
// Version: 1.0
// Description: Random 1D grid distribution functor.
// License: GNU GPL v3
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// includes FVGridMaker (ordem alfabética por caminho)
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/Common/namespace.hpp>
#include <FVGridMaker/Core/Common/types.hpp>
#include <FVGridMaker/Grid/Grid1D/Patterns/Distribution/ConceptsDistribution.hpp>
#include <FVGridMaker/Grid/Grid1D/Patterns/Distribution/Uniform1D.hpp>

// ----------------------------------------------------------------------------
// includes C++ (ordem alfabética)
// ----------------------------------------------------------------------------
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <numeric>
#include <random>
#include <span>
#include <vector>

/**
 * @file Random1D.hpp
 * @brief Random 1D distribution functor.
 * @ingroup Grid1D
 */

FVGRIDMAKER_NAMESPACE_OPEN
GRID_NAMESPACE_OPEN
GRID1D_NAMESPACE_OPEN
PATTERNS_NAMESPACE_OPEN
DISTRIBUTION_NAMESPACE_OPEN

/**
 * @brief Random 1D distribution.
 *
 * Generates a non-uniform grid on [A, B] with N cells using random weights.
 * The domain is fully covered and the ordering of faces is strictly increasing.
 *
 * Notes:
 *  - Randomness is controlled by @p seed. For reproducible grids, always pass
 *    an explicit non-zero seed.
 *  - The parameter @p dx_min is reserved for future refinements and is not
 *    enforced strictly in this implementation.
 */
struct Random1D {
    using Real = core::Real;
    using Size = std::size_t;

    /**
     * @brief Generate random face coordinates on [A, B].
     *
     * @param n       Number of physical cells.
     * @param a       Left domain boundary.
     * @param b       Right domain boundary.
     * @param xf      Span where face coordinates will be stored (size >= n+1).
     * @param seed    Seed for RNG (0 means use a default fixed seed).
     * @param dx_min  Reserved for future use (not enforced strictly yet).
     */
    void makeFaces(Size n,
                   Real a,
                   Real b,
                   std::span<Real> xf,
                   std::uint64_t seed = 0,
                   Real /*dx_min*/ = Real(0)) const {
        assert(n > Size(0));
        assert(xf.size() >= n + Size(1));

        const Real length = b - a;
        assert(length > Real(0));

        const auto weights = generateWeights(n, seed);
        const Real sum_w =
            std::accumulate(weights.begin(), weights.end(), Real(0));

        // Fallback: uniform grid if RNG degenerates.
        if (sum_w <= Real(0)) {
            Uniform1D{}.makeFaces(n, a, b, xf);
            return;
        }

        xf[0] = a;
        Real x = a;

        // Use all but the last weight to avoid accumulating rounding error,
        // and force the last face to be exactly at b.
        for (Size i = 0; i + 1 < n; ++i) {
            const Real frac = weights[i] / sum_w;
            const Real dx = length * frac;
            x += dx;
            xf[i + 1] = x;
        }

        xf[n] = b;
    }

    /**
     * @brief Generate random cell-center coordinates on [A, B].
     *
     * The same RNG sequence used in makeFaces is re-applied here so that
     * centers are consistent with the underlying random pattern.
     *
     * @param n       Number of physical cells.
     * @param a       Left domain boundary.
     * @param b       Right domain boundary.
     * @param xc      Span where center coordinates will be stored (size >= n).
     * @param seed    Seed for RNG (0 means use a default fixed seed).
     * @param dx_min  Reserved for future use (not enforced strictly yet).
     */
    void makeCenters(Size n,
                     Real a,
                     Real b,
                     std::span<Real> xc,
                     std::uint64_t seed = 0,
                     Real /*dx_min*/ = Real(0)) const {
        assert(n > Size(0));
        assert(xc.size() >= n);

        const Real length = b - a;
        assert(length > Real(0));

        const auto weights = generateWeights(n, seed);
        const Real sum_w =
            std::accumulate(weights.begin(), weights.end(), Real(0));

        if (sum_w <= Real(0)) {
            Uniform1D{}.makeCenters(n, a, b, xc);
            return;
        }

        Real x_left = a;
        Real x_right = a;

        for (Size i = 0; i + 1 < n; ++i) {
            const Real frac = weights[i] / sum_w;
            const Real dx = length * frac;
            x_right = x_left + dx;
            xc[i] = (x_left + x_right) * Real(0.5);
            x_left = x_right;
        }

        // Last cell closes exactly at b.
        x_right = b;
        xc[n - 1] = (x_left + x_right) * Real(0.5);
    }

private:
    [[nodiscard]] static std::vector<Real> generateWeights(
        Size n, std::uint64_t seed) {
        const std::uint64_t effective_seed =
            (seed != 0u) ? seed : defaultSeed();

        std::mt19937_64 rng(effective_seed);
        std::uniform_real_distribution<Real> dist(Real(0.0), Real(1.0));

        std::vector<Real> w(n);
        for (auto &wi : w) {
            wi = dist(rng);
        }
        return w;
    }

    [[nodiscard]] static constexpr std::uint64_t defaultSeed() noexcept {
        // Fixed default seed for deterministic behaviour when seed == 0.
        return 0x9E3779B97F4A7C15ull;
    }
};

static_assert(
    Distribution1D<Random1D>,
    "Random1D must satisfy the Distribution1D concept.");

DISTRIBUTION_NAMESPACE_CLOSE
PATTERNS_NAMESPACE_CLOSE
GRID1D_NAMESPACE_CLOSE
GRID_NAMESPACE_CLOSE
FVGRIDMAKER_NAMESPACE_CLOSE
