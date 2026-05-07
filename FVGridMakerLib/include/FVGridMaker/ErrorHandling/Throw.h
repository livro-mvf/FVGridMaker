#pragma once

#include <FVGridMaker/ErrorHandling/Detail.h>
#include <FVGridMaker/ErrorHandling/ErrorManager.h>
#include <FVGridMaker/ErrorHandling/ErrorTraits.h>
#include <FVGridMaker/ErrorHandling/FVGException.h>

#include <initializer_list>
#include <string>
#include <string_view>
#include <utility>

/**
 * @file Throw.h
 * @brief Funções tipadas para criar, registrar e lançar erros da FVGridMaker.
 *
 * Este arquivo define o caminho preferencial para o código da biblioteca:
 * funções template tipadas em vez de macros de erro.
 */

FVGRIDMAKER_NAMESPACE_OPEN
ERROR_NAMESPACE_OPEN

using ErrorContext = std::initializer_list<std::pair<std::string_view, std::string>>;

template <ErrorEnum E>
inline void registrarErro(
    E err_enum,
    ErrorContext context = {})
{
    detail::log_error(err_enum, context);
}

template <ErrorEnum E>
[[nodiscard]] inline ErrorRecord make_error_record(
    E err_enum,
    ErrorContext context = {})
{
    registrarErro(err_enum, context);

    auto errors = ErrorManager::flush();
    if (!errors.empty()) {
        return std::move(errors.back());
    }

    const auto sev = ErrorTraits<E>::default_severity(err_enum);
    return ErrorRecord{
        .code = code(err_enum),
        .severity = sev,
        .message = "Erro grave lançado (verifique log/severidade)"
    };
}

template <ErrorEnum E>
[[noreturn]] inline void throw_error(
    E err_enum,
    ErrorContext context = {})
{
    throw FVGException{make_error_record(err_enum, context)};
}

template <ErrorEnum E>
[[noreturn]] inline void lancarErro(
    E err_enum,
    ErrorContext context = {})
{
    throw_error(err_enum, context);
}

template <ErrorEnum E>
inline void exigir(
    bool condition,
    E err_enum,
    ErrorContext context = {})
{
    if (!condition) {
        throw_error(err_enum, context);
    }
}

ERROR_NAMESPACE_CLOSE
FVGRIDMAKER_NAMESPACE_CLOSE
