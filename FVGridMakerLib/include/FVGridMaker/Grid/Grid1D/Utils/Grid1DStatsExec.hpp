// ----------------------------------------------------------------------------
// File: Grid1DStatsExec.hpp
// Project: FVGridMaker
// Description: Estatisticas Grid1D com politica de execucao serial/paralela.
// ----------------------------------------------------------------------------

#pragma once

#include <FVGridMaker/Core/Common/namespace.hpp> // FVG_GRID1D_UTILS_OPEN/CLOSE.
#include <FVGridMaker/Core/Common/types.hpp> // FVGridMaker::core::Real.
#include <FVGridMaker/Grid/Grid1D/Utils/Grid1DStats.hpp> // Grid1DStats.

#include <algorithm> // std::max, std::min.
#include <cmath> // std::sqrt.
#include <cstddef> // std::size_t.
#ifdef FVG_HAVE_PSTL_EXEC
#include <execution> // std::execution::par.
#endif
#include <limits> // std::numeric_limits.
#include <numeric> // std::transform_reduce.
#include <span> // std::span.

/**
 * @file Grid1DStatsExec.hpp
 * @brief Versao com politica de execucao para estatisticas basicas de Grid1D.
 *
 * Este header e opcional. A fonte principal de metricas continua sendo
 * `Grid1DStats.hpp`. Aqui existe apenas uma camada fina para escolher entre:
 *
 * - `ExecPolicy::Serial`: sempre usa `Grid1DStats::basic(...)`;
 * - `ExecPolicy::Parallel`: usa Parallel STL quando o TU foi compilado com
 *   `FVG_HAVE_PSTL_EXEC`; caso contrario, cai para serial;
 * - `ExecPolicy::Auto`: usa paralelo quando disponivel e serial quando nao.
 *
 * Apenas as estatisticas basicas entram na via paralela. Metricas como
 * suavidade, histograma e razoes adjacentes continuam em `Grid1DStats`.
 */

FVG_GRID1D_UTILS_OPEN

enum class ExecPolicy {
    Auto,
    Serial,
    Parallel
};

struct Grid1DStatsExec {
    using Real = ::FVGridMaker::core::Real;
    using Basic = Grid1DStats::Basic;

    [[nodiscard]] static constexpr bool hasParallel() noexcept {
#ifdef FVG_HAVE_PSTL_EXEC
        return true;
#else
        return false;
#endif
    }

    [[nodiscard]] static Basic basic(std::span<const Real> values,
                                     ExecPolicy policy = ExecPolicy::Auto,
                                     bool* used_parallel = nullptr) {
        if (values.empty()) {
            if (used_parallel != nullptr) {
                *used_parallel = false;
            }
            return {};
        }

#ifdef FVG_HAVE_PSTL_EXEC
        const bool wants_parallel =
            policy == ExecPolicy::Parallel ||
            (policy == ExecPolicy::Auto && hasParallel());

        if (wants_parallel) {
            struct Acc {
                Real min{std::numeric_limits<Real>::infinity()};
                Real max{-std::numeric_limits<Real>::infinity()};
                long double sum{0};
                long double sum2{0};
                std::size_t count{0};
            };

            const auto make = [](Real value) {
                const auto v = static_cast<long double>(value);
                return Acc{value, value, v, v * v, std::size_t{1}};
            };

            const auto join = [](const Acc& left, const Acc& right) {
                Acc out{};
                out.min = std::min(left.min, right.min);
                out.max = std::max(left.max, right.max);
                out.sum = left.sum + right.sum;
                out.sum2 = left.sum2 + right.sum2;
                out.count = left.count + right.count;
                return out;
            };

            const Acc init{};
            const Acc acc = std::transform_reduce(std::execution::par,
                                                  values.begin(),
                                                  values.end(),
                                                  init,
                                                  join,
                                                  make);

            Basic out{};
            out.count = acc.count;
            out.sum = static_cast<Real>(acc.sum);
            out.min = acc.min;
            out.max = acc.max;
            out.mean = out.sum / static_cast<Real>(out.count);

            const long double mean_ld = static_cast<long double>(out.mean);
            const long double variance =
                acc.sum2 / static_cast<long double>(out.count) -
                mean_ld * mean_ld;

            out.stddev = variance > 0
                ? std::sqrt(static_cast<Real>(variance))
                : Real(0);
            out.aspect = out.min > Real(0)
                ? out.max / out.min
                : std::numeric_limits<Real>::infinity();
            out.cv = out.mean > Real(0) ? out.stddev / out.mean : Real(0);

            if (used_parallel != nullptr) {
                *used_parallel = true;
            }

            return out;
        }
#else
        (void)policy;
#endif

        if (used_parallel != nullptr) {
            *used_parallel = false;
        }
        return Grid1DStats::basic(values);
    }

    template <class GridLike>
    [[nodiscard]] static Basic basicFaces(const GridLike& grid,
                                          ExecPolicy policy = ExecPolicy::Auto,
                                          bool* used_parallel = nullptr) {
        return basic(grid.deltasFaces(), policy, used_parallel);
    }

    template <class GridLike>
    [[nodiscard]] static Basic basicPhysicalFaces(
        const GridLike& grid,
        ExecPolicy policy = ExecPolicy::Auto,
        bool* used_parallel = nullptr) {

        const auto dface = grid.deltasFaces();
        const auto first =
            static_cast<std::size_t>(grid.primeiroVolumeFisico());
        const auto count =
            static_cast<std::size_t>(grid.nVolumesFisicos());

        return basic(dface.subspan(first, count), policy, used_parallel);
    }
};

FVG_GRID1D_UTILS_CLOSE
