// ----------------------------------------------------------------------------
// File: ErrorTraits.h
// Project: FVGridMaker
// Version: 1.3
// Description: Define o template base ErrorTraits e o concept ErrorEnum.
//              Estabelece o contrato (interface estática) que todos os enums
//              de erro devem seguir para se integrarem ao sistema.
// Last modified: 2025-11-25
// Author: FVGridMaker Team
// License: GNU GPL v3
// ----------------------------------------------------------------------------

#pragma once


// ----------------------------------------------------------------------------
// includes c++
// ----------------------------------------------------------------------------
#include <cstdint>

// ----------------------------------------------------------------------------
// includes FVGridMaker
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/Severity.h>

/**
 * @file ErrorTraits.h
 * @brief Traits e Concepts para extensão do sistema de erros.
 * @ingroup ErrorHandling
 */

FVGRIDMAKER_NAMESPACE_OPEN
ERROR_NAMESPACE_OPEN

/**
 * @brief Declaração primária do template de Traits.
 * Deve ser especializado para cada Enum de erro do usuário.
 *
 * @tparam T O tipo do enum (enum class).
 */
template <class T> struct ErrorTraits;

/**
 * @brief Combina ID de domínio e valor do erro em um inteiro de 32 bits.
 *
 * Layout:
 * [31 ... 16] Domain ID (16 bits)
 * [15 ...  0] Error Value (16 bits)
 *
 * @param domain Identificador único do domínio (ex: 0x0001 para Core).
 * @param value Valor numérico do erro dentro do enum.
 * @return std::uint32_t Código único global.
 */
constexpr std::uint32_t make_code(std::uint16_t domain,
                                  std::uint16_t value) noexcept {
    return (static_cast<std::uint32_t>(domain) << 16) |
           static_cast<std::uint32_t>(value);
}

/**
 * @concept ErrorEnum
 * @brief Verifica se um tipo T satisfaz os requisitos para ser um erro do sistema.
 *
 * Requisitos:
 * 1. T deve ser um enum.
 * 2. ErrorTraits<T> deve estar especializado e fornecer os métodos estáticos:
 * - domain_id(), domain_name()
 * - key(e), enUS(e), ptBR(e), default_severity(e)
 */
template <class T>
concept ErrorEnum = std::is_enum_v<T> &&
    requires(T e) {
        { ErrorTraits<T>::domain_id() } noexcept;
        { ErrorTraits<T>::domain_name() } noexcept;
        { ErrorTraits<T>::key(e) } noexcept;
        { ErrorTraits<T>::enUS(e) } noexcept;
        { ErrorTraits<T>::ptBR(e) } noexcept;
        { ErrorTraits<T>::default_severity(e) } noexcept;
    };

/**
 * @brief Helper para gerar o código de 32 bits a partir de um enum tipado.
 *
 * Utiliza SFINAE/Concepts para garantir que só seja chamado para enums válidos.
 *
 * @tparam E Um tipo que satisfaça o concept ErrorEnum.
 * @param err_enum O valor do erro.
 * @return std::uint32_t O código combinado (Domain | Value).
 */
template <ErrorEnum E>
constexpr std::uint32_t code(E err_enum) noexcept {
    // A cláusula 'requires' no template acima já garante a validade,
    // mas o static_assert ajuda a gerar mensagens de erro mais claras
    // caso o compilador não suporte conceitos plenamente ou para debug.
    static_assert(requires { ErrorTraits<E>::domain_id(); },
                  "ErrorTraits<E> não está especializado ou incompleto.");
                  
    return make_code(ErrorTraits<E>::domain_id(),
                     static_cast<std::uint16_t>(err_enum));
}

ERROR_NAMESPACE_CLOSE
FVGRIDMAKER_NAMESPACE_CLOSE