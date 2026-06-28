// ============================================================================
// Arquivo: Operations2D.h
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Declara operações geométricas sobre malhas estruturadas bidimensionais.
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
#include <utility>

#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/FaceCentered1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>
#include <FVGridMaker/OneDimensional/Operations1D/Operations1D.h>
#include <FVGridMaker/TwoDimensional/CoordinateSystem2D/CoordinateSystem2D.h>
#include <FVGridMaker/TwoDimensional/Operations2D/LogicalBox2D.h>
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

namespace fvgrid {

class Operations2D final {
public:
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "TwoDimensional",
            "Operations2D",
            "fvgrid.2d.operations.Operations2D"
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
    [[nodiscard]] static BasicLogicalBox2D<T> domain_box(
        const BasicStructuredGrid2D<T>& grid
    ) noexcept;

    template <std::floating_point T>
    [[nodiscard]] static BasicLogicalBox2D<T> intersection(
        const BasicStructuredGrid2D<T>& left,
        const BasicStructuredGrid2D<T>& right,
        T tolerance = T{}
    );

    template <std::floating_point T>
    [[nodiscard]] static BasicLogicalBox2D<T> require_box_intersection(
        const BasicStructuredGrid2D<T>& left,
        const BasicStructuredGrid2D<T>& right,
        T tolerance = T{}
    );

    template <std::floating_point T>
    [[nodiscard]] static BasicStructuredGrid2D<T> clip_to_logical_box(
        const BasicStructuredGrid2D<T>& grid,
        BasicLogicalBox2D<T> box,
        T tolerance = T{}
    );

    template <std::floating_point T, class Mapping>
        requires CoordinateMapping2DFor<Mapping, T>
    [[nodiscard]] static BasicStructuredGrid2D<T> clip_to_logical_box(
        const BasicStructuredGrid2D<T>& grid,
        BasicLogicalBox2D<T> box,
        const Mapping& mapping,
        T tolerance = T{}
    );

private:
    template <std::floating_point T>
    static void require_same_patterns(
        const BasicStructuredGrid2D<T>& left,
        const BasicStructuredGrid2D<T>& right
    );

    template <std::floating_point T>
    static void require_area_box(BasicLogicalBox2D<T> box);

    template <std::floating_point T>
    static void require_clippable_axis_patterns(
        const BasicStructuredGrid2D<T>& grid
    );
};

}  // namespace fvgrid

#include <FVGridMaker/TwoDimensional/Operations2D/Operations2D.tpp>
