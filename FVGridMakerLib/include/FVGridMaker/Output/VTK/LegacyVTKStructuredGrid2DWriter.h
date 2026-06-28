// ============================================================================
// Arquivo: LegacyVTKStructuredGrid2DWriter.h
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Declara escritor VTK legacy para malhas estruturadas mapeadas.
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
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

namespace fvgrid {

class LegacyVTKStructuredGrid2DWriter final {
public:
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "Output",
            "LegacyVTKStructuredGrid2DWriter",
            "fvgrid.output.vtk.LegacyVTKStructuredGrid2DWriter"
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
        const BasicStructuredGrid2D<T>& grid,
        const std::filesystem::path& filepath
    );

private:
    template <std::floating_point T>
    static void write_geometry(
        std::ostream& output,
        const BasicStructuredGrid2D<T>& grid
    );

    template <std::floating_point T>
    static void write_cell_measure_data(
        std::ostream& output,
        const BasicStructuredGrid2D<T>& grid
    );
};

}  // namespace fvgrid

#include <FVGridMaker/Output/VTK/LegacyVTKStructuredGrid2DWriter.tpp>
