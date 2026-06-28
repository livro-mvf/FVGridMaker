// ============================================================================
// Arquivo: ErrorTraits.h
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Define conceitos e utilitários para descritores de erro tipados.
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


// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/ErrorDescriptor.h>

namespace fvgrid {

template <class Error>
concept ErrorDescriptorProvider =
    requires {
        { Error::code } -> std::convertible_to<std::string_view>;
        { Error::message } -> std::convertible_to<std::string_view>;
        { Error::category } -> std::convertible_to<std::string_view>;
    };

template <ErrorDescriptorProvider Error>
[[nodiscard]] constexpr ErrorDescriptor descriptor_of() noexcept {
    return ErrorDescriptor{
        .code = Error::code,
        .message = Error::message,
        .category = Error::category,
    };
}

}  // namespace fvgrid
