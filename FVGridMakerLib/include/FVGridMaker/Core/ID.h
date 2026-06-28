// ============================================================================
// Arquivo: ID.h
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Declara uma identidade leve e imutável para componentes do FVGridMaker.
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
#include <string_view>

namespace fvgrid {

class ID final {
public:
    constexpr ID() noexcept = default;

    constexpr ID(
        std::string_view module,
        std::string_view class_name,
        std::string_view class_id
    ) noexcept
        : module_(module),
          class_name_(class_name),
          class_id_(class_id) {}

    [[nodiscard]] constexpr std::string_view module() const noexcept {
        return module_;
    }

    [[nodiscard]] constexpr std::string_view class_name() const noexcept {
        return class_name_;
    }

    [[nodiscard]] constexpr std::string_view class_id() const noexcept {
        return class_id_;
    }

private:
    std::string_view module_ = "Unknown";
    std::string_view class_name_ = "Unknown";
    std::string_view class_id_ = "fvgrid.unknown.Unknown";
};

}  // namespace fvgrid

