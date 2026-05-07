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
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <random>
#include <span>

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
 * Generates a non-uniform grid on [A, B] by perturbing a uniform reference
 * grid. The perturbation is controlled by a percentage of the reference
 * spacing.
 *
 * Notes:
 *  - Randomness is controlled by @p seed.
 *  - The parameter @p percentual is interpreted as a fraction, e.g. 0.20 means
 *    20% of the reference spacing.
 *  - If a generated point is too close to a neighbor, the whole attempt is
 *    rejected and a new one is generated.
 */
struct Random1D {
    using Real = core::Real;
    using Size = std::size_t;

    [[nodiscard]] static std::uint64_t variedSeed() {
        const auto now = std::chrono::high_resolution_clock::now()
                             .time_since_epoch()
                             .count();
        std::random_device rd;

        return (static_cast<std::uint64_t>(now) << 1u) ^
               static_cast<std::uint64_t>(rd());
    }

    /**
     * @brief Generate random face coordinates on [A, B].
     *
     * @param n       Number of physical cells.
     * @param a       Left domain boundary.
     * @param b       Right domain boundary.
     * @param xf      Span where face coordinates will be stored (size >= n+1).
     * @param seed        Seed for RNG (0 means use a default fixed seed).
     * @param percentual  Maximum perturbation as a fraction of uniform dx.
     */
    void makeFaces(Size n,
                   Real a,
                   Real b,
                   std::span<Real> xf,
                   std::uint64_t seed = 0,
                   Real percentual = Real(0.20)) const {
        assert(n > Size(0));
        assert(xf.size() >= n + Size(1));

        const Real length = b - a;
        assert(length > Real(0));

        const Real dx_ref = length / static_cast<Real>(n);
        const Real dx_min = minSpacing(dx_ref);
        std::mt19937_64 rng(effectiveSeed(seed));
        std::uniform_real_distribution<Real> dist(-percentual, percentual);

        do {
            Uniform1D{}.makeFaces(n, a, b, xf);

            if (n <= Size(1) || percentual <= Real(0)) {
                return;
            }

            for (Size i = 1; i < n; ++i) {
                xf[i] += dist(rng) * dx_ref;
            }

            xf[0] = a;
            xf[n] = b;
        } while (!hasMinimumSpacing(xf.first(n + Size(1)), dx_min));
    }

    void makeFaces(Size n,
                   Real a,
                   Real b,
                   std::span<Real> xf,
                   std::uint64_t seed,
                   Real percentual,
                   Real dx_min_fraction) const {
        assert(n > Size(0));
        assert(xf.size() >= n + Size(1));

        const Real length = b - a;
        assert(length > Real(0));

        const Real dx_ref = length / static_cast<Real>(n);
        const Real dx_min = dx_min_fraction * dx_ref;
        std::mt19937_64 rng(effectiveSeed(seed));
        std::uniform_real_distribution<Real> dist(-percentual, percentual);

        do {
            Uniform1D{}.makeFaces(n, a, b, xf);

            if (n <= Size(1) || percentual <= Real(0)) {
                return;
            }

            for (Size i = 1; i < n; ++i) {
                xf[i] += dist(rng) * dx_ref;
            }

            xf[0] = a;
            xf[n] = b;
        } while (!hasMinimumSpacing(xf.first(n + Size(1)), dx_min));
    }

    void makeCenters(Size n,
                     Real a,
                     Real b,
                     std::span<Real> xc,
                     std::uint64_t seed,
                     Real percentual,
                     Real dx_min_fraction) const {
        assert(n > Size(0));
        assert(xc.size() >= n);

        const Real length = b - a;
        assert(length > Real(0));

        const Real dx_ref = length / static_cast<Real>(n);
        const Real dx_min = dx_min_fraction * dx_ref;
        std::mt19937_64 rng(effectiveSeed(seed));
        std::uniform_real_distribution<Real> dist(-percentual, percentual);

        do {
            Uniform1D{}.makeCenters(n, a, b, xc);

            if (percentual <= Real(0)) {
                return;
            }

            for (Size i = 0; i < n; ++i) {
                xc[i] += dist(rng) * dx_ref;
            }
        } while (!hasMinimumSpacing(xc.first(n), dx_min) ||
                 !insideDomain(xc.first(n), a, b));
    }

    /**
     * @brief Generate cell-center coordinates for a random grid.
     *
     * @param n           Number of physical cells.
     * @param a           Left domain boundary.
     * @param b           Right domain boundary.
     * @param xc          Span where center coordinates will be stored (size >= n).
     * @param seed        Seed for RNG (0 means use a default fixed seed).
     * @param percentual  Maximum perturbation as a fraction of uniform dx.
     */
    void makeCenters(Size n,
                     Real a,
                     Real b,
                     std::span<Real> xc,
                     std::uint64_t seed = 0,
                     Real percentual = Real(0.20)) const {
        assert(n > Size(0));
        assert(xc.size() >= n);

        const Real length = b - a;
        assert(length > Real(0));

        const Real dx_ref = length / static_cast<Real>(n);
        const Real dx_min = minSpacing(dx_ref);
        std::mt19937_64 rng(effectiveSeed(seed));
        std::uniform_real_distribution<Real> dist(-percentual, percentual);

        do {
            Uniform1D{}.makeCenters(n, a, b, xc);

            if (percentual <= Real(0)) {
                return;
            }

            for (Size i = 0; i < n; ++i) {
                xc[i] += dist(rng) * dx_ref;
            }
        } while (!hasMinimumSpacing(xc.first(n), dx_min) ||
                 !insideDomain(xc.first(n), a, b));
    }

private:
    [[nodiscard]] static constexpr Real minSpacing(Real dx_ref) noexcept {
        return Real(0.01) * dx_ref;
    }

    [[nodiscard]] static bool hasMinimumSpacing(std::span<const Real> x,
                                                Real dx_min) noexcept {
        for (Size i = 1; i < x.size(); ++i) {
            if (!(x[i] - x[i - 1u] >= dx_min)) {
                return false;
            }
        }

        return true;
    }

    [[nodiscard]] static bool insideDomain(std::span<const Real> x,
                                           Real a,
                                           Real b) noexcept {
        for (Real xi : x) {
            if (!(xi > a && xi < b)) {
                return false;
            }
        }

        return true;
    }
    [[nodiscard]] static std::uint64_t effectiveSeed(std::uint64_t seed) noexcept {
        return (seed != 0u) ? seed : defaultSeed();
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
