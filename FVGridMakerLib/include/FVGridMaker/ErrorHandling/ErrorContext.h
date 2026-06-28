// ============================================================================
// Arquivo: ErrorContext.h
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Define pares de contexto para enriquecer diagnósticos de erro.
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
#include <string>
#include <string_view>
#include <vector>

namespace fvgrid {

struct ErrorContext final {
    std::string key;
    std::string value;
};

using ErrorContextList = std::vector<ErrorContext>;

[[nodiscard]] inline ErrorContext make_error_context(
    std::string_view key,
    std::string_view value
) {
    return ErrorContext{
        .key = std::string{key},
        .value = std::string{value},
    };
}

}  // namespace fvgrid
