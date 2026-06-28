// ============================================================================
// Arquivo: Quality2D.h
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Declara relatórios de qualidade para malhas estruturadas bidimensionais.
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
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

namespace fvgrid {

template <std::floating_point T>
struct BasicQualityReport2D final {
    using value_type = T;

    T min_cell_measure{};
    T max_cell_measure{};
    T cell_measure_ratio{};

    T min_computational_area{};
    T max_computational_area{};
    T computational_area_ratio{};

    T max_adjacent_cell_measure_ratio_first_direction{};
    T max_adjacent_cell_measure_ratio_second_direction{};

    T min_first_face_measure{};
    T max_first_face_measure{};
    T min_second_face_measure{};
    T max_second_face_measure{};
};

using QualityReport2D = BasicQualityReport2D<double>;
using QualityReport2DFloat = BasicQualityReport2D<float>;
using QualityReport2DLongDouble = BasicQualityReport2D<long double>;

class Quality2D final {
public:
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "TwoDimensional",
            "Quality2D",
            "fvgrid.2d.quality.Quality2D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    template <std::floating_point T>
    [[nodiscard]] static BasicQualityReport2D<T> evaluate(
        const BasicStructuredGrid2D<T>& grid
    );
};

}  // namespace fvgrid

#include <FVGridMaker/TwoDimensional/Quality2D/Quality2D.tpp>
