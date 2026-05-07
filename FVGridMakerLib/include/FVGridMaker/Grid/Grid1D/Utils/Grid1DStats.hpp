// ----------------------------------------------------------------------------
// File: Grid1DStats.hpp
// Project: FVGridMaker
// Description: Estatisticas e metricas simples de qualidade para malhas 1D.
// ----------------------------------------------------------------------------

#pragma once

#include <FVGridMaker/Core/Common/namespace.hpp> // FVG_GRID1D_UTILS_OPEN/CLOSE.
#include <FVGridMaker/Core/Common/types.hpp> // FVGridMaker::core::Real.

#include <algorithm> // std::clamp, std::max, std::min, std::minmax_element.
#include <cmath> // std::abs, std::sqrt.
#include <cstddef> // std::size_t.
#include <limits> // std::numeric_limits.
#include <numeric> // std::accumulate.
#include <optional> // std::optional.
#include <span> // std::span.
#include <utility> // std::pair.
#include <vector> // std::vector.

/**
 * @file Grid1DStats.hpp
 * @brief Estatisticas de malhas 1D.
 *
 * A classe `Grid1DStats` nao gera malha e nao altera a malha recebida. Ela
 * apenas resume vetores geometricos ja calculados, como `deltasFaces()` e
 * `deltasCenters()`.
 *
 * Convenção usada neste arquivo:
 *
 * - `dface_x`: distancia entre faces consecutivas. Em uma malha 1D,
 *   corresponde a largura dos volumes.
 * - `dcentro_x`: distancia associada aos centros. Tem `N + 1` valores para
 *   `N` volumes, pois inclui as distancias das bordas.
 *
 * A classe oferece dois grupos de funcoes:
 *
 * - funcoes sobre `std::span<const Real>`, uteis para testar vetores isolados;
 * - funcoes sobre `GridLike`, isto e, qualquer objeto que exponha os metodos
 *   publicos de `Grid1D` usados aqui.
 *
 * Por padrao, as estatisticas baseadas em `grid.deltasFaces()` consideram todos
 * os volumes armazenados, incluindo volumes ficticios. Para analisar apenas a
 * malha fisica, use `basicPhysicalFaces(grid)`.
 */

FVG_GRID1D_UTILS_OPEN

struct Grid1DStats {
    using Real = ::FVGridMaker::core::Real;

    /**
     * @brief Estatisticas basicas de um conjunto de comprimentos.
     *
     * Campos:
     * - `count`: numero de valores analisados.
     * - `sum`: soma dos valores.
     * - `min` e `max`: menor e maior valor.
     * - `mean`: media aritmetica.
     * - `stddev`: desvio padrao populacional.
     * - `aspect`: razao `max / min`.
     * - `cv`: coeficiente de variacao `stddev / mean`.
     */
    struct Basic {
        std::size_t count{0};
        Real sum{0};
        Real min{0};
        Real max{0};
        Real mean{0};
        Real stddev{0};
        Real aspect{0};
        Real cv{0};
    };

    /**
     * @brief Histograma simples de comprimentos.
     *
     * `counts[k]` guarda quantos valores cairam no bin `k`.
     */
    struct Histogram {
        Real bin_min{0};
        Real bin_max{0};
        Real bin_width{0};
        std::vector<std::size_t> counts{};
    };

    /**
     * @brief Razoes entre comprimentos vizinhos.
     *
     * `ratios[i] = L[i + 1] / L[i]`.
     *
     * `worst_sym_ratio` considera tanto crescimento quanto reducao:
     * `max(r, 1/r)`.
     */
    struct AdjacentRatios {
        std::vector<Real> ratios{};
        Real max_ratio{1};
        Real worst_sym_ratio{1};
    };

    /**
     * @brief Medida simples de suavidade entre comprimentos vizinhos.
     *
     * Para cada par vizinho, usa:
     *
     * `abs(L[i+1] - L[i]) / min(L[i], L[i+1])`.
     */
    struct Smoothness {
        Real mean_grad{0};
        Real max_grad{0};
    };

    /**
     * @brief Compara as bordas com o interior da malha.
     */
    struct EdgeBalance {
        Real mean_interior{0};
        Real left_over_interior{0};
        Real right_over_interior{0};
    };

    /**
     * @brief Estatisticas basicas sobre um span.
     */
    [[nodiscard]] static Basic basic(std::span<const Real> values) {
        Basic out{};
        out.count = values.size();

        if (values.empty()) {
            return out;
        }

        const auto [min_it, max_it] =
            std::minmax_element(values.begin(), values.end());

        out.min = *min_it;
        out.max = *max_it;
        out.sum = std::accumulate(values.begin(), values.end(), Real(0));
        out.mean = out.sum / static_cast<Real>(out.count);

        Real variance_sum = 0;
        for (Real value : values) {
            const Real delta = value - out.mean;
            variance_sum += delta * delta;
        }

        out.stddev = std::sqrt(variance_sum / static_cast<Real>(out.count));
        out.aspect = out.min > Real(0)
            ? out.max / out.min
            : std::numeric_limits<Real>::infinity();
        out.cv = out.mean > Real(0) ? out.stddev / out.mean : Real(0);

        return out;
    }

    /**
     * @brief Estatisticas sobre todas as larguras `dface_x` armazenadas.
     *
     * Inclui volumes ficticios, se existirem.
     */
    template <class GridLike>
    [[nodiscard]] static Basic basicFaces(const GridLike& grid) {
        return basic(grid.deltasFaces());
    }

    /**
     * @brief Estatisticas sobre todos os `dcentro_x` armazenados.
     *
     * Inclui contribuicoes associadas aos volumes ficticios, se existirem.
     */
    template <class GridLike>
    [[nodiscard]] static Basic basicCenters(const GridLike& grid) {
        return basic(grid.deltasCenters());
    }

    /**
     * @brief Estatisticas das larguras dos volumes fisicos.
     *
     * Esta e a funcao recomendada quando o usuario quer avaliar somente a
     * qualidade da malha fisica, ignorando volumes ficticios.
     */
    template <class GridLike>
    [[nodiscard]] static Basic basicPhysicalFaces(const GridLike& grid) {
        const auto dface = grid.deltasFaces();
        const auto first =
            static_cast<std::size_t>(grid.primeiroVolumeFisico());
        const auto count =
            static_cast<std::size_t>(grid.nVolumesFisicos());

        return basic(dface.subspan(first, count));
    }

    /**
     * @brief Estatisticas de `dcentro_x` fisico.
     *
     * Diferente de `dface_x`, os valores fisicos de `dcentro_x` nao sao sempre
     * uma subfaixa direta de `deltasCenters()` quando ha volumes ficticios,
     * porque o primeiro e o ultimo valor usam as faces fisicas do dominio.
     */
    template <class GridLike>
    [[nodiscard]] static Basic basicPhysicalCenters(const GridLike& grid) {
        std::vector<Real> values;
        const auto n = grid.nVolumesFisicos();
        values.reserve(static_cast<std::size_t>(n + 1));

        for (decltype(n) i = 0; i <= n; ++i) {
            values.push_back(grid.deltaCentroFisico(i));
        }

        return basic(std::span<const Real>{values.data(), values.size()});
    }

    /**
     * @brief Uniformidade relativa no intervalo [0, 1].
     *
     * Valor proximo de 1 indica comprimentos quase iguais. Valor proximo de 0
     * indica grande dispersao em relacao a media.
     */
    [[nodiscard]] static Real relativeUniformity(std::span<const Real> values) {
        if (values.empty()) {
            return Real(0);
        }

        const Basic stats = basic(values);
        if (stats.mean == Real(0)) {
            return Real(0);
        }

        Real deviation_sum = 0;
        for (Real value : values) {
            deviation_sum += std::abs(value - stats.mean);
        }

        const Real uniformity =
            Real(1) -
            deviation_sum / (static_cast<Real>(values.size()) * stats.mean);

        return std::clamp(uniformity, Real(0), Real(1));
    }

    template <class GridLike>
    [[nodiscard]] static Real relativeUniformityFaces(const GridLike& grid) {
        return relativeUniformity(grid.deltasFaces());
    }

    template <class GridLike>
    [[nodiscard]] static Real relativeUniformityPhysicalFaces(
        const GridLike& grid) {

        const auto dface = grid.deltasFaces();
        const auto first =
            static_cast<std::size_t>(grid.primeiroVolumeFisico());
        const auto count =
            static_cast<std::size_t>(grid.nVolumesFisicos());

        return relativeUniformity(dface.subspan(first, count));
    }

    /**
     * @brief Informa se todos os valores sao estritamente positivos.
     */
    [[nodiscard]] static bool allPositive(std::span<const Real> values) {
        return std::all_of(values.begin(), values.end(), [](Real value) {
            return value > Real(0);
        });
    }

    template <class GridLike>
    [[nodiscard]] static bool physicalFacesArePositive(const GridLike& grid) {
        const auto dface = grid.deltasFaces();
        const auto first =
            static_cast<std::size_t>(grid.primeiroVolumeFisico());
        const auto count =
            static_cast<std::size_t>(grid.nVolumesFisicos());

        return allPositive(dface.subspan(first, count));
    }

    /**
     * @brief Informa se os valores sao quase uniformes.
     *
     * `relative_tolerance` compara cada valor com a media:
     * `abs(value - mean) <= relative_tolerance * mean`.
     */
    [[nodiscard]] static bool isAlmostUniform(std::span<const Real> values,
                                              Real relative_tolerance) {
        if (values.empty()) {
            return true;
        }

        const Basic stats = basic(values);
        if (stats.mean == Real(0)) {
            return false;
        }

        const Real tolerance = relative_tolerance * stats.mean;
        return std::all_of(values.begin(), values.end(), [&](Real value) {
            return std::abs(value - stats.mean) <= tolerance;
        });
    }

    /**
     * @brief Histograma de comprimentos.
     */
    [[nodiscard]] static Histogram histogram(
        std::span<const Real> values,
        std::size_t bins,
        std::optional<std::pair<Real, Real>> range = std::nullopt) {

        Histogram out{};
        if (values.empty() || bins == 0u) {
            return out;
        }

        Real lo = range ? range->first
                        : *std::min_element(values.begin(), values.end());
        Real hi = range ? range->second
                        : *std::max_element(values.begin(), values.end());

        if (!(hi > lo)) {
            hi = lo + Real(1);
        }

        out.bin_min = lo;
        out.bin_max = hi;
        out.bin_width = (hi - lo) / static_cast<Real>(bins);
        out.counts.assign(bins, std::size_t{0});

        for (Real value : values) {
            if (value <= lo) {
                ++out.counts.front();
                continue;
            }

            if (value >= hi) {
                ++out.counts.back();
                continue;
            }

            const Real normalized = (value - lo) / (hi - lo);
            auto bin =
                static_cast<std::size_t>(normalized * static_cast<Real>(bins));

            if (bin >= bins) {
                bin = bins - 1u;
            }

            ++out.counts[bin];
        }

        return out;
    }

    /**
     * @brief Razoes entre comprimentos vizinhos.
     */
    [[nodiscard]] static AdjacentRatios adjacentRatios(
        std::span<const Real> values) {

        AdjacentRatios out{};
        if (values.size() < 2u) {
            return out;
        }

        out.ratios.resize(values.size() - 1u);

        for (std::size_t i = 0; i + 1u < values.size(); ++i) {
            const Real left = values[i];
            const Real right = values[i + 1u];
            const Real ratio = left > Real(0)
                ? right / left
                : std::numeric_limits<Real>::infinity();

            out.ratios[i] = ratio;
            out.max_ratio = std::max(out.max_ratio, ratio);

            const Real symmetric = ratio > Real(0)
                ? std::max(ratio, Real(1) / ratio)
                : std::numeric_limits<Real>::infinity();

            out.worst_sym_ratio =
                std::max(out.worst_sym_ratio, symmetric);
        }

        return out;
    }

    template <class GridLike>
    [[nodiscard]] static AdjacentRatios adjacentRatiosPhysicalFaces(
        const GridLike& grid) {

        const auto dface = grid.deltasFaces();
        const auto first =
            static_cast<std::size_t>(grid.primeiroVolumeFisico());
        const auto count =
            static_cast<std::size_t>(grid.nVolumesFisicos());

        return adjacentRatios(dface.subspan(first, count));
    }

    /**
     * @brief Suavidade entre comprimentos vizinhos.
     */
    [[nodiscard]] static Smoothness smoothness(std::span<const Real> values) {
        Smoothness out{};
        if (values.size() < 2u) {
            return out;
        }

        Real sum = 0;
        for (std::size_t i = 0; i + 1u < values.size(); ++i) {
            const Real left = values[i];
            const Real right = values[i + 1u];
            const Real denominator = std::min(left, right);
            const Real grad = denominator > Real(0)
                ? std::abs(right - left) / denominator
                : Real(0);

            sum += grad;
            out.max_grad = std::max(out.max_grad, grad);
        }

        out.mean_grad = sum / static_cast<Real>(values.size() - 1u);
        return out;
    }

    template <class GridLike>
    [[nodiscard]] static Smoothness smoothnessPhysicalFaces(
        const GridLike& grid) {

        const auto dface = grid.deltasFaces();
        const auto first =
            static_cast<std::size_t>(grid.primeiroVolumeFisico());
        const auto count =
            static_cast<std::size_t>(grid.nVolumesFisicos());

        return smoothness(dface.subspan(first, count));
    }

    /**
     * @brief Compara primeira/ultima largura com a media do interior.
     */
    [[nodiscard]] static EdgeBalance edgeBalance(std::span<const Real> values) {
        EdgeBalance out{};
        if (values.size() <= 2u) {
            return out;
        }

        Real interior_sum = 0;
        for (std::size_t i = 1; i + 1u < values.size(); ++i) {
            interior_sum += values[i];
        }

        out.mean_interior =
            interior_sum / static_cast<Real>(values.size() - 2u);

        if (out.mean_interior > Real(0)) {
            out.left_over_interior = values.front() / out.mean_interior;
            out.right_over_interior = values.back() / out.mean_interior;
        }

        return out;
    }

    template <class GridLike>
    [[nodiscard]] static EdgeBalance edgeBalancePhysicalFaces(
        const GridLike& grid) {

        const auto dface = grid.deltasFaces();
        const auto first =
            static_cast<std::size_t>(grid.primeiroVolumeFisico());
        const auto count =
            static_cast<std::size_t>(grid.nVolumesFisicos());

        return edgeBalance(dface.subspan(first, count));
    }
};

FVG_GRID1D_UTILS_CLOSE
