// ============================================================================
// Arquivo: Custom1D.h
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Declara a construção de eixos unidimensionais a partir de coordenadas do usuário.
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
#include <type_traits>
#include <utility>

#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/ErrorHandling/BuiltInErrors.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Coordinates1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Domain1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/GridPatternConcept1D.h>

namespace fvgrid {

template <std::floating_point T>
class BasicCustom1D final {
public:
    using value_type = T;

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "Custom1D",
            "fvgrid.1d.distribution.Custom1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    template <class Pattern>
        requires GridPattern1DFor<std::remove_cvref_t<Pattern>, T>
    [[nodiscard]] static BasicAxis1D<T> make(
        BasicCoordinates1D<T> coordinates,
        Pattern&& pattern,
        BasicDomain1D<T> domain = BasicDomain1D<T>::none()
    ) {
        using PatternType = std::remove_cvref_t<Pattern>;
        using CoordinateTag = typename PatternType::primary_coordinates;

        require<errors::grid::InvalidCoordinateKind>(
            coordinates.template has_tag<CoordinateTag>(),
            BasicCustom1D::id()
        );

        const auto& pattern_ref = pattern;

        BasicAxisGeometry1D<T> geometry = pattern_ref.complete_geometry(
            std::move(coordinates).release_values(),
            domain
        );

        return BasicAxis1D<T>::from_geometry(std::move(geometry));
    }
};

using Custom1D = BasicCustom1D<double>;
using Custom1DFloat = BasicCustom1D<float>;
using Custom1DLongDouble = BasicCustom1D<long double>;

template <std::floating_point T, class Pattern>
    requires GridPattern1DFor<std::remove_cvref_t<Pattern>, T>
[[nodiscard]] BasicAxis1D<T> custom_axis_1d(
    BasicCoordinates1D<T> coordinates,
    Pattern&& pattern,
    BasicDomain1D<T> domain = BasicDomain1D<T>::none()
) {
    return BasicCustom1D<T>::make(
        std::move(coordinates),
        std::forward<Pattern>(pattern),
        domain
    );
}

template <class Pattern>
    requires GridPattern1DFor<std::remove_cvref_t<Pattern>, double>
[[nodiscard]] Axis1D custom_axis_1d(
    Coordinates1D coordinates,
    Pattern&& pattern,
    Domain1D domain = Domain1D::none()
) {
    return Custom1D::make(
        std::move(coordinates),
        std::forward<Pattern>(pattern),
        domain
    );
}

}  // namespace fvgrid
