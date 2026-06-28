// ============================================================================
// Arquivo: Operations1D.h
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Declara operações geométricas sobre eixos unidimensionais validados.
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
#include <span>
#include <string_view>
#include <vector>

#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/OneDimensional/Operations1D/AxisInterval1D.h>

namespace fvgrid {

class Operations1D final {
public:
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "Operations1D",
            "fvgrid.1d.operations.Operations1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    template <std::floating_point T>
    static void validate_tolerance(T tolerance);

    template <std::floating_point T>
    [[nodiscard]] static bool same_pattern(
        const BasicAxis1D<T>& left,
        const BasicAxis1D<T>& right
    ) noexcept;

    template <std::floating_point T>
    static void require_same_pattern(
        const BasicAxis1D<T>& left,
        const BasicAxis1D<T>& right
    );

    template <std::floating_point T>
    [[nodiscard]] static BasicAxisInterval1D<T> domain_interval(
        const BasicAxis1D<T>& axis
    ) noexcept;

    template <std::floating_point T>
    [[nodiscard]] static BasicAxisInterval1D<T> intersection(
        const BasicAxis1D<T>& left,
        const BasicAxis1D<T>& right,
        T tolerance = T{}
    );

    template <std::floating_point T>
    [[nodiscard]] static BasicAxisInterval1D<T> require_interval_intersection(
        const BasicAxis1D<T>& left,
        const BasicAxis1D<T>& right,
        T tolerance = T{}
    );

    template <std::floating_point T>
    [[nodiscard]] static std::vector<T> unique_sorted_coordinates(
        std::span<const T> coordinates,
        T tolerance = T{}
    );

    template <std::floating_point T>
    [[nodiscard]] static BasicAxis1D<T> clip_faces_to_interval(
        const BasicAxis1D<T>& axis,
        BasicAxisInterval1D<T> interval,
        T tolerance = T{}
    );
};

}  // namespace fvgrid

#include <FVGridMaker/OneDimensional/Operations1D/Operations1D.tpp>
