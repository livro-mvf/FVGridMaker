// ============================================================================
// Arquivo: Axis1DCSVWriter.h
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Declara escritor CSV para inspeção tabular de eixos unidimensionais.
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
#include <filesystem>
#include <iosfwd>
#include <string_view>

#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>

namespace fvgrid {

class Axis1DCSVWriter final {
public:
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "Output",
            "Axis1DCSVWriter",
            "fvgrid.output.csv.Axis1DCSVWriter"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    template <std::floating_point T>
    static void write(
        const BasicAxis1D<T>& axis,
        std::ostream& output
    );

    template <std::floating_point T>
    static void write(
        const BasicAxis1D<T>& axis,
        const std::filesystem::path& filepath
    );
};

}  // namespace fvgrid

#include <FVGridMaker/Output/CSV/Axis1DCSVWriter.tpp>
