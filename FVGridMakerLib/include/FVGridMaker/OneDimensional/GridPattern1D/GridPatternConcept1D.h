// ============================================================================
// Arquivo: GridPatternConcept1D.h
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Define conceitos que validam padrões de reconstrução de malhas 1D.
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

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <concepts>
#include <string_view>
#include <utility>
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/OneDimensional/GridPattern1D/AxisGeometry1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/CoordinateTags1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Domain1D.h>

namespace fvgrid {

template <class Pattern, class T>
concept GridPattern1DFor =
    std::floating_point<T> &&
    requires(
        const Pattern& pattern,
        std::vector<T> coordinates,
        BasicDomain1D<T> domain
    ) {
        typename Pattern::primary_coordinates;

        requires PrimaryCoordinateTag1D<typename Pattern::primary_coordinates>;

        { Pattern::name() } -> std::convertible_to<std::string_view>;

        {
            pattern.complete_geometry(std::move(coordinates), domain)
        } -> std::same_as<BasicAxisGeometry1D<T>>;
    };

template <class Pattern>
concept GridPattern1D = GridPattern1DFor<Pattern, double>;

}  // namespace fvgrid
