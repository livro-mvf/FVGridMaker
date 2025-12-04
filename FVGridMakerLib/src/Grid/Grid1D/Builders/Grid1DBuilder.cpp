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

#include <FVGridMaker/Grid/Grid1D/API/Grid1D.h>
#include <FVGridMaker/Grid/Grid1D/Builders/Grid1DBuilder.hpp>
#include <FVGridMaker/Grid/Grid1D/Patterns/Distribution/ConceptsDistribution.hpp>
#include <FVGridMaker/Grid/Grid1D/Patterns/Distribution/Random1D.hpp>
#include <FVGridMaker/Grid/Grid1D/Patterns/Distribution/Uniform1D.hpp>

// ----------------------------------------------------------------------------
// includes C++ (alphabetical)
// ----------------------------------------------------------------------------
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <span>
#include <stdexcept>
#include <typeindex>
#include <vector>

FVGRIDMAKER_NAMESPACE_OPEN
GRID_NAMESPACE_OPEN
GRID1D_NAMESPACE_OPEN
BUILDERS_NAMESPACE_OPEN
DETAIL_NAMESPACE_OPEN

using core::Index;
using core::Real;

using api::Grid1D;
using patterns::distribution::Distribution1D;
using patterns::distribution::Random1D;
using patterns::distribution::Uniform1D;

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
 * Ghost cells (cfg.nGhost) and centering (cfg.centering) are currently not
 * used in this backend; they can be incorporated later via policies sem
 * quebrar a interface do builder.
 */
template <Distribution1D Dist>
static Grid1D build_with_distribution(const Grid1DBuilderConfig& cfg) {
    // -------------------------------------------------------------------------
    // 1. Extract basic configuration
    // -------------------------------------------------------------------------
    const Index n = cfg.n;
    const Real  a = cfg.a;
    const Real  b = cfg.b;

    if (n <= Index{0}) {
        throw std::invalid_argument("[Grid1DBuilder] n must be > 0.");
    }
    if (!(b > a)) {
        throw std::invalid_argument("[Grid1DBuilder] Domain must satisfy b > a.");
    }

    const std::size_t n_cells = static_cast<std::size_t>(n);
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

    std::span<Real> xf_span{xf.data(), xf.size()};
    std::span<Real> xc_span{xc.data(), xc.size()};

    using Size = typename Dist::Size;

    // Se Dist::Size for unsigned (size_t), o cast explícito evita warnings
    // de conversão de sinal sem alterar o comportamento.
    dist.makeFaces(static_cast<Size>(n), a, b, xf_span);
    dist.makeCenters(static_cast<Size>(n), a, b, xc_span);

    // -------------------------------------------------------------------------
    // 4. Compute face distances dF
    // -------------------------------------------------------------------------
    for (std::size_t i = 0; i < n_cells; ++i) {
        dF[i] = xf[i + 1] - xf[i];
    }

    // -------------------------------------------------------------------------
    // 5. Compute cell "spacings" dC (mesma convenção do código antigo)
    // -------------------------------------------------------------------------
    dC.front() = xc.front() - xf.front();
    if (n_cells >= 2) {
        for (std::size_t i = 1; i < n_cells; ++i) {
            dC[i] = xc[i] - xc[i - 1];
        }
    }
    dC.back() = xf.back() - xc.back();

    // -------------------------------------------------------------------------
    // 6. Build the Grid1D object
    //
    // Mantém a mesma chamada usada anteriormente:
    //   Grid1D{faces, centres, dF, dC}
    // Se a assinatura de Grid1D mudar no futuro (ex.: suportar ghosts
    // explicitamente), basta ajustar esta linha, sem alterar o builder.
    // -------------------------------------------------------------------------
    return Grid1D{std::move(xf), std::move(xc),
                  std::move(dF), std::move(dC)};
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

    // Se você preferir, isto pode ser substituído pelo sistema de erros
    // personalizado da biblioteca.
    throw std::invalid_argument(
        "[Grid1DBuilder] Unknown distribution type in build_grid1d().");
}

DETAIL_NAMESPACE_CLOSE
BUILDERS_NAMESPACE_CLOSE
GRID1D_NAMESPACE_CLOSE
GRID_NAMESPACE_CLOSE
FVGRIDMAKER_NAMESPACE_CLOSE
