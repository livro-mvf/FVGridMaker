// ----------------------------------------------------------------------------
// File: Status.h
// Project: FVGridMaker
// Version: 1.3
// Description: Define as classes `Status` (sucesso/falha) e `StatusOr<T>`
//              (Either Monad). Permite o tratamento de erros funcional sem
//              o uso de exceções.
// Last modified: 2025-11-25
// Author: FVGridMaker Team
// License: GNU GPL v3
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// includes c++
// ----------------------------------------------------------------------------
#include <optional>

// ----------------------------------------------------------------------------
// includes FVGridMaker
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/ErrorRecord.h>

/**
 * @file Status.h
 * @brief Primitivas para tratamento de erros sem exceções.
 * @ingroup ErrorHandling
 */

FVGRIDMAKER_NAMESPACE_OPEN
ERROR_NAMESPACE_OPEN

/**
 * @class Status
 * @brief Encapsula o resultado de uma operação (Sucesso ou Erro).
 *
 * É leve e carrega um `ErrorRecord` apenas em caso de falha.
 * O estado padrão (construtor default) representa Sucesso (OK).
 */
class Status {
public:
    // --- Construtores ---

    /** @brief Constrói um status de Sucesso (OK). */
    Status() : ok_(true), record_() {}

    /** @brief Constrói um status de Falha a partir de um registro de erro. */
    explicit Status(ErrorRecord record) : ok_(false), record_(std::move(record)) {}

    // --- Métodos de Acesso ---

    /**
     * @brief Verifica se a operação foi bem-sucedida.
     * @return true se OK, false se Erro.
     */
    [[nodiscard]] bool ok() const { return ok_; }

    /** @brief Retorna o código do erro (0 se OK). */
    std::uint32_t code() const { return record_.code; }

    /** @brief Retorna a mensagem descritiva do erro (vazia se OK). */
    const std::string& message() const { return record_.message; }

    /** @brief Retorna o registro completo do erro. */
    const ErrorRecord& record() const { return record_; }

    // --- Helpers Estáticos ---

    /** @brief Factory method para criar um Status de sucesso explicitamente. */
    static Status OK() { return Status(); }

private:
    bool ok_;            ///< Flag de sucesso.
    ErrorRecord record_; ///< Detalhes do erro (se ok_ == false).
};


// --- CLASSE 'StatusOr' ---

/**
 * @class StatusOr
 * @brief Container que armazena um valor T (Sucesso) OU um Status (Erro).
 *
 * Similar ao `std::expected` (C++23) ou `absl::StatusOr`. Permite que funções
 * retornem valores válidos ou erros de forma segura e tipada.
 *
 * @tparam T O tipo do valor retornado em caso de sucesso.
 */
template <typename T>
class StatusOr {
public:
    // --- Construtores de Sucesso (Implicitamente convertíveis de T) ---

    /** @brief Constrói a partir de um valor (Move semantics). */
    StatusOr(T&& value) : value_(std::move(value)) {}

    /** @brief Constrói a partir de um valor (Copy semantics). */
    StatusOr(const T& value) : value_(value) {}

    // --- Construtor de Falha (Implicitamente convertível de Status) ---

    /**
     * @brief Constrói a partir de um Status de erro.
     * @param status O status de erro. (Se passar Status::OK(), o comportamento é indefinido ao acessar value).
     */
    StatusOr(Status status) : status_(std::move(status)) {}

    // --- Métodos de Verificação ---

    /** @brief Verifica se contém um valor válido. */
    [[nodiscard]] bool ok() const { return status_.ok(); }

    /** @brief Retorna o Status associado (OK se tiver valor, Erro caso contrário). */
    const Status& status() const { return status_; }

    // --- Acesso ao Valor ---

    /**
     * @brief Acessa o valor armazenado (Const).
     * @throws std::bad_optional_access Se não houver valor (erro).
     */
    const T& value() const { return value_.value(); }

    /**
     * @brief Acessa o valor armazenado (Modificável).
     * @throws std::bad_optional_access Se não houver valor (erro).
     */
    T& value() { return value_.value(); }

private:
    std::optional<T> value_;       ///< O valor (presente apenas se sucesso).
    Status status_{Status::OK()};  ///< O status (padrão OK se value_ for preenchido).
};

ERROR_NAMESPACE_CLOSE
FVGRIDMAKER_NAMESPACE_CLOSE