// ----------------------------------------------------------------------------
// File: Detail.h
// Project: FVGridMaker
// Version: 1.0
// Description: Helpers internos para renderização de mensagens e despacho
//              de logs. Realiza a substituição de placeholders e verificação
//              de severidade antes de invocar o ErrorManager.
// Last modified: 2025-11-25
// Author: João Flávio Vasconcellos
// License: GNU GPL v3
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// includes FVGridMaker
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/ErrorManager.h>
#include <FVGridMaker/ErrorHandling/ErrorTraits.h>

/**
 * @file Detail.h
 * @brief Implementação de funções auxiliares internas do sistema de erros.
 *
 * @warning Este arquivo contém detalhes de implementação e não deve ser
 * incluído ou utilizado diretamente pelo usuário final da biblioteca.
 * O uso deve ser feito através das macros `FVG_ERROR` e `FVG_ASSERT`.
 *
 * @ingroup ErrorHandling
 */

FVGRIDMAKER_NAMESPACE_OPEN
ERROR_NAMESPACE_OPEN
DETAIL_NAMESPACE_OPEN

/**
 * @brief Função interna para processar e logar um erro.
 *
 * Esta função atua como o "motor" por trás das macros de erro. Ela realiza
 * a filtragem por severidade, a seleção do idioma correto, a interpolação
 * de strings (substituição de `{key}` por `value`) e o envio para o gerenciador.
 *
 * @tparam E O tipo do enum de erro (deve satisfazer o concept `ErrorEnum` em C++20).
 * @param err_enum O código de erro específico (valor do enum).
 * @param kv Lista de pares {chave, valor} para substituição na mensagem formatada.
 *
 * @note O processamento é abortado imediatamente se a severidade do erro for
 * inferior à `min_severity` configurada globalmente, evitando custos de formatação de string.
 */
template <ErrorEnum E>
inline void log_error(
    E err_enum,
    std::initializer_list<std::pair<std::string_view, std::string>> kv = {})
{
    // 1. Obtém a configuração global (Thread-safe)
    auto cfg = Config::get();
    if (!cfg) return; // Segurança em caso de destruição estática

    // 2. Verifica Filtro de Severidade (Early Exit)
    const Severity sev = ErrorTraits<E>::default_severity(err_enum);
    if (sev < cfg->min_severity) {
        return; // Abaixo do limiar, ignora o log
    }

    // 3. Seleção de Idioma (i18n)
    const Language lang = cfg->language;
    const std::string_view tmpl =
        (lang == Language::PtBR)
            ? ErrorTraits<E>::ptBR(err_enum)
            : ErrorTraits<E>::enUS(err_enum);

    // 4. Formatação da Mensagem (Substituição de Placeholders)
    std::string out(tmpl);
    for (const auto& [k, v] : kv) {
        // Monta o token, ex: "{name}"
        std::string token = "{";
        token.append(k);
        token += "}";

        // Substitui todas as ocorrências do token pelo valor
        std::string::size_type pos = 0;
        while ((pos = out.find(token, pos)) != std::string::npos) {
            out.replace(pos, token.size(), v);
            pos += v.size(); // Avança o cursor para evitar loops infinitos se v contiver o token
        }
    }

    // 5. Despacho para o ErrorManager
    ErrorManager::log(ErrorRecord{
        .code = code(err_enum),
        .severity = sev,
        .message = std::move(out)
        // timestamp e thread_id são preenchidos automaticamente no construtor/braced init
    });
}

DETAIL_NAMESPACE_CLOSE
ERROR_NAMESPACE_CLOSE
FVGRIDMAKER_NAMESPACE_CLOSE