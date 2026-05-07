// ----------------------------------------------------------------------------
// File: Grid1DBuilder.cpp
// Project: FVGridMaker
// Version: 3.0
// Description:
//   Backend implementation for Grid1DBuilder.
//
//   - Uses 1D distribution functors (Uniform1D, Random1D, ...)
//   - Generates faces and centres on [a, b]
//   - Computes face and cell sizes (dF, dC) in a data-oriented layout
//   - No registry, no std::any; selection by std::type_index.
// License: GNU GPL v3
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// includes FVMGridMaker (ordem alfabética por caminho)
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/Common/namespace.hpp>
#include <FVGridMaker/Core/Common/types.hpp>
#include <FVGridMaker/ErrorHandling/GridErrors.h>
#include <FVGridMaker/ErrorHandling/Throw.h>

// #include <FVGridMaker/Grid/Grid1D/Grid1D.h>
#include <FVGridMaker/Grid/Grid1D/Builders/Grid1DBuilder.hpp>
#include <FVGridMaker/Grid/Grid1D/Patterns/Distribution/ConceptsDistribution.hpp>
#include <FVGridMaker/Grid/Grid1D/Patterns/Distribution/Random1D.hpp>
#include <FVGridMaker/Grid/Grid1D/Patterns/Distribution/Uniform1D.hpp>

// ----------------------------------------------------------------------------
// includes C++ (alphabetical)
// ----------------------------------------------------------------------------
#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <typeindex>
#include <type_traits>
#include <vector>

FVGRIDMAKER_NAMESPACE_OPEN
GRID_NAMESPACE_OPEN
GRID1D_NAMESPACE_OPEN
BUILDERS_NAMESPACE_OPEN
DETAIL_NAMESPACE_OPEN

using core::Index;
using core::Real;

//
using patterns::distribution::Distribution1D;
using patterns::distribution::Random1D;
using patterns::distribution::Uniform1D;

namespace {

void validate_strictly_increasing(std::span<const Real> values,
                                  ::FVGridMaker::error::GridErr err) {
    for (std::size_t i = 1; i < values.size(); ++i) {
        if (!(values[i] > values[i - 1u])) {
            ::FVGridMaker::error::lancarErro(
                err, {{"i", std::to_string(i)}});
        }
    }
}

void validate_positive_physical_length(std::span<const Real> xf_physical) {
    if (!(xf_physical.back() - xf_physical.front() > Real(0))) {
        ::FVGridMaker::error::lancarErro(
            ::FVGridMaker::error::GridErr::InvalidDomain,
            {{"A", std::to_string(xf_physical.front())},
             {"B", std::to_string(xf_physical.back())}});
    }
}

void validate_fictitious_count(Index n_ficticios, Index n_physical) {
    if (n_ficticios < Index{0} || n_ficticios > n_physical) {
        ::FVGridMaker::error::lancarErro(
            ::FVGridMaker::error::GridErr::InvalidFictitiousVolumes,
            {{"N", std::to_string(n_ficticios)}});
    }
}

Real max_random_percentual(Index n) {
    if (n <= Index{1}) {
        return Real(0);
    }

    return Real(1);
}

void validate_random_percentual(Real percentual, Index n) {
    const Real max_percentual = max_random_percentual(n);

    if (percentual < Real(0) || percentual > max_percentual) {
        ::FVGridMaker::error::lancarErro(
            ::FVGridMaker::error::GridErr::OptionsOutOfRange,
            {{"w_lo", std::to_string(percentual)},
             {"w_hi", std::to_string(max_percentual)}});
    }
}

void extend_faces_by_reflection(std::span<const Real> xf_physical,
                                std::span<Real> xf,
                                std::size_t n_fict) {
    const std::size_t n_physical = xf_physical.size() - 1u;

    for (std::size_t i = 0; i <= n_physical; ++i) {
        xf[n_fict + i] = xf_physical[i];
    }

    Real x_left = xf_physical.front();
    for (std::size_t k = 0; k < n_fict; ++k) {
        const Real dx = xf_physical[k + 1u] - xf_physical[k];
        x_left -= dx;
        xf[n_fict - 1u - k] = x_left;
    }

    Real x_right = xf_physical.back();
    for (std::size_t k = 0; k < n_fict; ++k) {
        const Real dx =
            xf_physical[n_physical - k] - xf_physical[n_physical - 1u - k];
        x_right += dx;
        xf[n_fict + n_physical + 1u + k] = x_right;
    }
}

void extend_centers_by_reflection(std::span<const Real> xc_physical,
                                  std::span<Real> xc,
                                  Real a,
                                  Real b,
                                  std::size_t n_fict) {
    const std::size_t n_physical = xc_physical.size();

    for (std::size_t i = 0; i < n_physical; ++i) {
        xc[n_fict + i] = xc_physical[i];
    }

    for (std::size_t k = 0; k < n_fict; ++k) {
        xc[n_fict - 1u - k] = Real(2) * a - xc_physical[k];
        xc[n_fict + n_physical + k] =
            Real(2) * b - xc_physical[n_physical - 1u - k];
    }
}

void compute_centers_from_faces(std::span<const Real> xf,
                                std::span<Real> xc) {
    for (std::size_t i = 0; i < xc.size(); ++i) {
        xc[i] = Real(0.5) * (xf[i] + xf[i + 1u]);
    }
}

void compute_faces_from_centers(std::span<const Real> xc,
                                std::span<Real> xf,
                                Real a,
                                Real b,
                                std::size_t n_fict,
                                std::size_t n_physical) {
    const std::size_t n_cells = xc.size();

    for (std::size_t i = 1; i < n_cells; ++i) {
        xf[i] = Real(0.5) * (xc[i - 1u] + xc[i]);
    }

    if (n_fict == 0u) {
        xf.front() = a;
        xf.back() = b;
    } else {
        xf.front() = Real(2) * xc.front() - xf[1];
        xf.back() = Real(2) * xc.back() - xf[n_cells - 1u];
        xf[n_fict] = a;
        xf[n_fict + n_physical] = b;
    }
}

void compute_metrics(std::span<const Real> xf,
                     std::span<const Real> xc,
                     std::span<Real> dF,
                     std::span<Real> dC) {
    for (std::size_t i = 0; i < dF.size(); ++i) {
        dF[i] = xf[i + 1u] - xf[i];
    }

    dC.front() = xc.front() - xf.front();
    for (std::size_t i = 1; i < xc.size(); ++i) {
        dC[i] = xc[i] - xc[i - 1u];
    }
    dC.back() = xf.back() - xc.back();
}

} // namespace

// -----------------------------------------------------------------------------
// Helper: builds a Grid1D for a given distribution functor
// -----------------------------------------------------------------------------

/**
 * @brief Build a Grid1D using a specific 1D distribution functor.
 *
 * This function assumes that Grid1DBuilderConfig provides at least:
 *   - Index n;        // number of physical cells
 *   - Real  a;        // left domain boundary
 *   - Real  b;        // right domain boundary
 *
 * Centering defines the primary coordinate generated by the distribution:
 *   - CellCentered: distribution generates physical faces; centers are derived.
 *   - FaceCentered: distribution generates physical centers; faces are derived.
 *
 * Fictitious volumes are mirrored from physical volumes after the physical mesh
 * has been generated.
 */
template <Distribution1D Dist>
static Grid1D build_with_distribution(const Grid1DBuilderConfig& cfg) {
    // -------------------------------------------------------------------------
    // 1. Extract basic configuration
    // -------------------------------------------------------------------------
    const Index n = cfg.n;
    const Real  a = cfg.a;
    const Real  b = cfg.b;
    const Index n_ficticios = cfg.nGhost;

    if (n <= Index{0}) {
        ::FVGridMaker::error::lancarErro(
            ::FVGridMaker::error::GridErr::InvalidN,
            {{"N", std::to_string(n)}});
    }
    if (!(b > a)) {
        ::FVGridMaker::error::lancarErro(
            ::FVGridMaker::error::GridErr::InvalidDomain,
            {{"A", std::to_string(a)}, {"B", std::to_string(b)}});
    }
    if (n_ficticios < Index{0}) {
        ::FVGridMaker::error::lancarErro(
            ::FVGridMaker::error::GridErr::InvalidFictitiousVolumes,
            {{"N", std::to_string(n_ficticios)}});
    }
    validate_fictitious_count(n_ficticios, n);

    if constexpr (std::is_same_v<Dist, Random1D>) {
        validate_random_percentual(cfg.randomPercentual, n);
    }

    const std::size_t n_physical = static_cast<std::size_t>(n);
    const std::size_t n_fict = static_cast<std::size_t>(n_ficticios);
    const std::size_t n_cells = n_physical + 2u * n_fict;
    const std::size_t n_faces = n_cells + 1u;

    // -------------------------------------------------------------------------
    // 2. Allocate SoA arrays: faces, centres, face sizes, cell "spacings"
    // -------------------------------------------------------------------------
    std::vector<Real> xf(n_faces);       // faces: x_{1/2}, ..., x_{N+1/2}
    std::vector<Real> xc(n_cells);       // centres: x_1, ..., x_N
    std::vector<Real> dF(n_cells);       // Δx_F: face-to-face distances (N)
    std::vector<Real> dC(n_cells + 1u);  // Δx_C: (N+1) entries:
                                         //   [0]     : x_1   - x_{1/2}
                                         //   [1..N-1]: x_i   - x_{i-1}
                                         //   [N]     : x_{N+1/2} - x_N

    // -------------------------------------------------------------------------
    // 3. Run the distribution functor on the physical domain [a, b]
    // -------------------------------------------------------------------------
    Dist dist{};
    using Size = typename Dist::Size;
    const std::uint64_t seed =
        cfg.randomSeedMode == RandomSeedMode::Varied
            ? Random1D::variedSeed()
            : cfg.randomSeed;

    switch (cfg.centering) {
    case CenteringTag::CellCentered: {
        std::vector<Real> xf_physical(n_physical + 1u);
        dist.makeFaces(static_cast<Size>(n),
                       a,
                       b,
                       std::span<Real>{xf_physical.data(), xf_physical.size()},
                       seed,
                       cfg.randomPercentual);
        validate_strictly_increasing(
            std::span<const Real>{xf_physical.data(), xf_physical.size()},
            ::FVGridMaker::error::GridErr::NonIncreasingFaces);
        validate_positive_physical_length(
            std::span<const Real>{xf_physical.data(), xf_physical.size()});

        extend_faces_by_reflection(
            std::span<const Real>{xf_physical.data(), xf_physical.size()},
            std::span<Real>{xf.data(), xf.size()},
            n_fict);
        compute_centers_from_faces(
            std::span<const Real>{xf.data(), xf.size()},
            std::span<Real>{xc.data(), xc.size()});
        break;
    }

    case CenteringTag::FaceCentered: {
        std::vector<Real> xc_physical(n_physical);
        dist.makeCenters(static_cast<Size>(n),
                         a,
                         b,
                         std::span<Real>{xc_physical.data(),
                                         xc_physical.size()},
                         seed,
                         cfg.randomPercentual);
        validate_strictly_increasing(
            std::span<const Real>{xc_physical.data(), xc_physical.size()},
            ::FVGridMaker::error::GridErr::NonIncreasingCenters);

        extend_centers_by_reflection(
            std::span<const Real>{xc_physical.data(), xc_physical.size()},
            std::span<Real>{xc.data(), xc.size()},
            a,
            b,
            n_fict);
        compute_faces_from_centers(
            std::span<const Real>{xc.data(), xc.size()},
            std::span<Real>{xf.data(), xf.size()},
            a,
            b,
            n_fict,
            n_physical);
        validate_positive_physical_length(
            std::span<const Real>{xf.data() + n_fict, n_physical + 1u});
        break;
    }

    default:
        ::FVGridMaker::error::lancarErro(
            ::FVGridMaker::error::GridErr::InvalidCentering,
            {{"center", std::string{::FVGridMaker::grid::to_string(
                            cfg.centering)}}});
    }

    // -------------------------------------------------------------------------
    // 4. Validate complete geometry and compute metrics
    // -------------------------------------------------------------------------
    validate_strictly_increasing(
        std::span<const Real>{xf.data(), xf.size()},
        ::FVGridMaker::error::GridErr::NonIncreasingFaces);
    validate_strictly_increasing(
        std::span<const Real>{xc.data(), xc.size()},
        ::FVGridMaker::error::GridErr::NonIncreasingCenters);
    compute_metrics(
        std::span<const Real>{xf.data(), xf.size()},
        std::span<const Real>{xc.data(), xc.size()},
        std::span<Real>{dF.data(), dF.size()},
        std::span<Real>{dC.data(), dC.size()});

    // -------------------------------------------------------------------------
    // 5. Build the Grid1D object
    // -------------------------------------------------------------------------
    return Grid1D{std::move(xf), std::move(xc),
                  std::move(dF), std::move(dC),
                  n, n_ficticios, cfg.centering};
}

// -----------------------------------------------------------------------------
// Public backend: dispatch by distribution type
// -----------------------------------------------------------------------------

/**
 * @brief Backend for Grid1DBuilder::build().
 *
 * @param cfg               Aggregated builder configuration.
 * @param distribution_type std::type_index of the selected distribution functor.
 * @param centering_type    std::type_index of centering policy (currently
 *                          unused here; centering is encoded via distributions
 *                          or can be handled by a future policy layer).
 *
 * This function replaces the old registry-based implementation:
 *   - No singletons
 *   - No std::any
 *   - Selection by typeid(Dist) + concepts (Distribution1D)
 */
Grid1D build_grid1d(const Grid1DBuilderConfig& cfg,
                    const std::type_index& distribution_type,
                    const std::type_index& /*centering_type*/) {
    // NOTE:
    // Para adicionar novas distribuições, basta:
    //   1) Incluir o header correspondente;
    //   2) Acrescentar mais um "if" abaixo.
    //
    // Exemplo:
    //   if (distribution_type == typeid(LogStretch1D)) {
    //       return build_with_distribution<LogStretch1D>(cfg);
    //   }

    if (distribution_type == typeid(Uniform1D)) {
        return build_with_distribution<Uniform1D>(cfg);
    }

    if (distribution_type == typeid(Random1D)) {
        return build_with_distribution<Random1D>(cfg);
    }

    ::FVGridMaker::error::lancarErro(
        ::FVGridMaker::error::GridErr::InvalidDistribution,
        {{"dist", distribution_type.name()}});
}

DETAIL_NAMESPACE_CLOSE
BUILDERS_NAMESPACE_CLOSE
GRID1D_NAMESPACE_CLOSE
GRID_NAMESPACE_CLOSE
FVGRIDMAKER_NAMESPACE_CLOSE
