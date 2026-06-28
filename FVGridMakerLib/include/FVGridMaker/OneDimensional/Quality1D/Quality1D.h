// ============================================================================
// Arquivo: Quality1D.h
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Declara métricas de qualidade para eixos unidimensionais.
// Autor: João Flávio Vieira de Vasconcellos
//
// SPDX-FileCopyrightText: 2026 João Flávio Vieira de Vasconcellos
// SPDX-License-Identifier: BSD-3-Clause
//
// Este arquivo faz parte do FVGridMaker.
//
// Licença: BSD 3-Clause.
// É permitido usar, copiar, modificar e redistribuir este arquivo, em código-fonte
// ou forma binária, com ou sem modificações, desde que sejam preservados os avisos
// de copyright, esta identificação de licença e as condições descritas no arquivo
// LICENSE.
//
// O nome do autor, de colaboradores ou de instituições associadas ao projeto não
// pode ser usado para endossar ou promover produtos derivados sem autorização
// prévia por escrito.
//
// Este software é fornecido sem garantias de qualquer natureza. Consulte o arquivo
// LICENSE, na raiz do repositório, para o texto completo da licença.
// ============================================================================
#pragma once

#include <concepts>
#include <string_view>

#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>

namespace fvgrid {

template <std::floating_point T>
struct BasicQualityMetrics1D final {
    using value_type = T;

    T min_cell_length{};
    T max_cell_length{};
    T cell_length_ratio{};
    T max_adjacent_cell_ratio{};
};

using QualityMetrics1D = BasicQualityMetrics1D<double>;
using QualityMetrics1DFloat = BasicQualityMetrics1D<float>;
using QualityMetrics1DLongDouble = BasicQualityMetrics1D<long double>;

class Quality1D final {
public:
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "Quality1D",
            "fvgrid.1d.quality.Quality1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    template <std::floating_point T>
    [[nodiscard]] static BasicQualityMetrics1D<T> metrics(
        const BasicAxis1D<T>& axis
    );

    template <std::floating_point T>
    [[nodiscard]] static T min_cell_length(const BasicAxis1D<T>& axis);

    template <std::floating_point T>
    [[nodiscard]] static T max_cell_length(const BasicAxis1D<T>& axis);

    template <std::floating_point T>
    [[nodiscard]] static T cell_length_ratio(const BasicAxis1D<T>& axis);

    template <std::floating_point T>
    [[nodiscard]] static T max_adjacent_cell_ratio(
        const BasicAxis1D<T>& axis
    );
};

template <std::floating_point T>
[[nodiscard]] BasicQualityMetrics1D<T> quality_metrics_1d(
    const BasicAxis1D<T>& axis
);

[[nodiscard]] QualityMetrics1D quality_metrics_1d(const Axis1D& axis);

template <std::floating_point T>
[[nodiscard]] T min_cell_length_1d(const BasicAxis1D<T>& axis);

[[nodiscard]] Real min_cell_length_1d(const Axis1D& axis);

template <std::floating_point T>
[[nodiscard]] T max_cell_length_1d(const BasicAxis1D<T>& axis);

[[nodiscard]] Real max_cell_length_1d(const Axis1D& axis);

template <std::floating_point T>
[[nodiscard]] T cell_length_ratio_1d(const BasicAxis1D<T>& axis);

[[nodiscard]] Real cell_length_ratio_1d(const Axis1D& axis);

template <std::floating_point T>
[[nodiscard]] T max_adjacent_cell_ratio_1d(const BasicAxis1D<T>& axis);

[[nodiscard]] Real max_adjacent_cell_ratio_1d(const Axis1D& axis);

}  // namespace fvgrid

#include <FVGridMaker/OneDimensional/Quality1D/Quality1D.tpp>
