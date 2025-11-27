// ----------------------------------------------------------------------------
// File: CoreErrors.h
// Project: FVGridMaker
// Version: 1.6
// Description: Define a enumeração `CoreErr` para erros fundamentais (validação,
//              memória, geometria) e especializa o template `ErrorTraits`.
//              Integra os códigos de erro do núcleo ao sistema central de
//              exceções, fornecendo metadados de severidade e mensagens
//              localizadas (i18n).
// Last modified: 2025-11-25
// Author: João Flávio Vasconcellos
// License: GNU GPL v3
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// includes FVGridMaker
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/ErrorTraits.h>


/**
 * @file CoreErrors.h
 * @brief Definições de erros fundamentais do módulo Core.
 *
 * Este arquivo contém a enumeração de erros básicos que podem ocorrer
 * nas funcionalidades centrais da biblioteca, bem como a especialização
 * da estrutura `ErrorTraits` para integrar esses erros ao sistema
 * genérico de tratamento de exceções e logs.
 *
 * @author João Flávio Vasconcellos 
 * @version 1.6
 * @ingroup ErrorHandling
 */

FVGRIDMAKER_NAMESPACE_OPEN
ERROR_NAMESPACE_OPEN

/**
 * @enum CoreErr
 * @brief Enumeração fortemente tipada para códigos de erro do Core.
 *
 * Esta enumeração define os códigos de erro retornados ou lançados
 * pelas classes base e utilitários do sistema.
 *
 * @note O Domain ID associado a estes erros é **0x0001**.
 */
enum class CoreErr : std::uint16_t {
    /**
     * @brief Argumento inválido passado para uma função.
     * @details Ocorre quando um parâmetro não satisfaz as pré-condições (ex: nulo, negativo).
     */
    InvalidArgument = 1,

    /**
     * @brief Índice fora dos limites permitidos.
     * @details Geralmente associado a acessos a vetores ou malhas fora do range [0, N-1].
     */
    OutOfRange = 2,

    /**
     * @brief Funcionalidade ainda não implementada.
     * @details Usado em métodos virtuais ou placeholders de desenvolvimento futuro.
     */
    NotImplemented = 3,

    /**
     * @brief Falha crítica de asserção interna.
     * @details Indica um estado lógico impossível, geralmente bugs internos da biblioteca.
     */
    AssertFailed = 4,

    /**
     * @brief Geometria inconsistente detectada.
     * @details Ex: Volumes negativos, faces com áreas nulas ou topologia inválida.
     */
    InconsistentGeometry = 5,

    /// @brief Limite inferior para iteração (metadado).
    _Min = InvalidArgument,
    /// @brief Limite superior para iteração (metadado).
    _Max = InconsistentGeometry
};

DETAIL_NAMESPACE_OPEN

    using sv = std::string_view;

    /**
     * @struct CoreErrorInfo
     * @brief Estrutura auxiliar contendo metadados estáticos de um erro.
     * @internal Esta estrutura é um detalhe de implementação.
     */
    struct CoreErrorInfo {
        sv key;             ///< Chave única textual do erro (ex: "CORE_INVALID_ARGUMENT").
        Severity severity;  ///< Nível de severidade padrão.
        sv enUS;            ///< Mensagem padrão em Inglês (formato fmt/std::format).
        sv ptBR;            ///< Mensagem padrão em Português (formato fmt/std::format).
    };

    /**
     * @brief Mapeia um código `CoreErr` para seus metadados.
     *
     * Função `constexpr` que atua como uma tabela de lookup em tempo de compilação.
     *
     * @param e O código de erro do tipo `CoreErr`.
     * @return CoreErrorInfo Estrutura preenchida com os dados do erro.
     */
    constexpr CoreErrorInfo get_core_error_info_data(CoreErr e) {
        switch (e) {
            case CoreErr::InvalidArgument:
                return {sv{"CORE_INVALID_ARGUMENT"}, Severity::Error, sv{"Invalid argument: {name}."}, sv{"Argumento inválido: {name}."}};
            case CoreErr::OutOfRange:
                return {sv{"CORE_OUT_OF_RANGE"}, Severity::Error, sv{"Index out of range: {index}."}, sv{"Índice fora do intervalo: {index}."}};
            case CoreErr::NotImplemented:
                return {sv{"CORE_NOT_IMPLEMENTED"}, Severity::Warning, sv{"Feature not implemented."}, sv{"Recurso não implementado."}};
            case CoreErr::AssertFailed:
                return {sv{"CORE_ASSERT_FAILED"}, Severity::Fatal, sv{"Assertion failed."}, sv{"Falha de asserção."}};
            case CoreErr::InconsistentGeometry:
                 return {sv{"CORE_INCONSISTENT_GEOMETRY"}, Severity::Error, sv{"Geometric inconsistency detected: {details}."}, sv{"Inconsistência geométrica detectada: {details}."}};
            default:
                 return {sv{}, Severity::Trace, sv{}, sv{}};
        }
    }

    /**
     * @brief Wrapper seguro para obter informações do erro.
     *
     * Verifica se o erro está dentro dos limites `_Min` e `_Max` antes de buscar.
     *
     * @param e O código de erro.
     * @return CoreErrorInfo Dados do erro ou estrutura vazia se inválido.
     */
    constexpr CoreErrorInfo get_core_info(CoreErr e) noexcept {
         if (static_cast<uint16_t>(e) >= static_cast<uint16_t>(CoreErr::_Min) &&
             static_cast<uint16_t>(e) <= static_cast<uint16_t>(CoreErr::_Max))
         {
              return get_core_error_info_data(e);
         }
         return {sv{}, Severity::Trace, sv{}, sv{}};
    }

DETAIL_NAMESPACE_CLOSE

/**
 * @brief Especialização de ErrorTraits para o domínio `CoreErr`.
 *
 * Esta especialização permite que o sistema genérico de erros (ErrorRegistry/Exceptions)
 * entenda como extrair mensagens, chaves e severidades do enum `CoreErr` sem acoplamento direto.
 *
 * **Exemplo de uso genérico:**
 * @code{.cpp}
 * auto msg = ErrorTraits<CoreErr>::ptBR(CoreErr::InvalidArgument);
 * // Retorna "Argumento inválido: {name}."
 * @endcode
 */
template <> struct ErrorTraits<CoreErr> {
    /**
     * @brief ID único do domínio de erro (0x0001 para Core).
     * @return std::uint16_t Identificador hexadecimal.
     */
    static constexpr std::uint16_t domain_id() noexcept { return 0x0001; }

    /**
     * @brief Nome legível do domínio.
     * @return std::string_view "Core".
     */
    static constexpr std::string_view domain_name() noexcept { return detail::sv{"Core"}; }

    /**
     * @brief Obtém a severidade padrão para o erro.
     */
    static constexpr Severity default_severity(CoreErr e) noexcept { return detail::get_core_info(e).severity; }

    /**
     * @brief Obtém a chave única (string ID) do erro.
     */
    static constexpr std::string_view key(CoreErr e) noexcept { return detail::get_core_info(e).key; }

    /**
     * @brief Obtém o template de mensagem em Inglês (US).
     */
    static constexpr std::string_view enUS(CoreErr e) noexcept { return detail::get_core_info(e).enUS; }

    /**
     * @brief Obtém o template de mensagem em Português (BR).
     */
    static constexpr std::string_view ptBR(CoreErr e) noexcept { return detail::get_core_info(e).ptBR; }
};

ERROR_NAMESPACE_CLOSE
FVGRIDMAKER_NAMESPACE_CLOSE