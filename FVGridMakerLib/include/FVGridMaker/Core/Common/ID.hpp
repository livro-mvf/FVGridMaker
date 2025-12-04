// ----------------------------------------------------------------------------
// File: ID.hpp
// Project: FVGridMaker
// Version: 1.0
// Description: Classe base para identificação única de objetos (Entities).
//              Fornece um UID sequencial thread-safe e um nome legível para
//              fins de debug e logging.
// Last modified: 2025-11-25
// Author: FVGridMaker Team
// License: GNU GPL v3
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// includes FVMGridMaker
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/Common/namespace.hpp>


// ----------------------------------------------------------------------------
// includes da biblioteca padrão
// ----------------------------------------------------------------------------
#include <atomic>
#include <string>

/**
 * @file ID.hpp
 * @brief Classe base para identificação e rastreamento de objetos.
 * @ingroup Core
 */

FVGRIDMAKER_NAMESPACE_OPEN
CORE_NAMESPACE_OPEN
COMMON_NAMESPACE_OPEN

/**
 * @class ID
 * @brief Mixin que fornece identidade única (UID) e nome para objetos.
 *
 * Projetada para ser herdada por classes importantes (como `Grid`, `Field`, `Solver`)
 * que precisam ser rastreadas em logs ou gerenciadores.
 *
 * - **Thread-Safe:** O gerador de IDs usa `std::atomic`.
 * - **Não-Copiável:** A cópia é desabilitada para evitar duplicação de identidade
 * (duas entidades vivas com o mesmo ID). Se você copiar um objeto derivado,
 * ele deve gerar um *novo* ID para a cópia.
 * - **Movível:** A identidade pode ser transferida via `std::move`.
 */
class ID {
public:
    // ------------------------------------------------------------------------
    // Construtores e Destrutor
    // ------------------------------------------------------------------------

    /**
     * @brief Construtor padrão.
     * Gera um novo UID único e inicia com nome vazio.
     */
    ID() : uid_(next_uid()), name_() {}

    /**
     * @brief Construtor nomeado.
     * @param name Nome legível para o objeto (ex: "MalhaPrincipal").
     */
    explicit ID(std::string_view name) : uid_(next_uid()), name_(name) {}

    /**
     * @brief Destrutor virtual.
     * Essencial para classes base polimórficas.
     */
    virtual ~ID() = default;

    // ------------------------------------------------------------------------
    // Semântica de Cópia e Movimento
    // ------------------------------------------------------------------------

    // Desabilita cópia implícita (Identidade é única)
    ID(const ID&) = delete;
    ID& operator=(const ID&) = delete;

    /**
     * @brief Construtor de movimento.
     * Transfere a identidade (UID e Nome) do objeto `other` para este.
     * O objeto `other` fica com ID inválido.
     */
    ID(ID&& other) noexcept
        : uid_(other.uid_), name_(std::move(other.name_))
    {
        other.uid_ = INVALID_INDEX;
    }

    /**
     * @brief Atribuição de movimento.
     */
    ID& operator=(ID&& other) noexcept {
        if (this != &other) {
            uid_ = other.uid_;
            name_ = std::move(other.name_);
            other.uid_ = INVALID_INDEX;
        }
        return *this;
    }

    // ------------------------------------------------------------------------
    // Acesso
    // ------------------------------------------------------------------------

    /**
     * @brief Retorna o identificador único global da instância.
     * @return Index (inteiro 64-bit).
     */
    [[nodiscard]] Index uid() const noexcept { return uid_; }

    /**
     * @brief Retorna o nome legível do objeto.
     */
    [[nodiscard]] const std::string& name() const noexcept { return name_; }

    /**
     * @brief Verifica se o objeto possui uma identidade válida.
     * (Útil após um std::move).
     */
    [[nodiscard]] bool has_valid_id() const noexcept { return uid_ != INVALID_INDEX; }

    /**
     * @brief Define ou altera o nome do objeto.
     */
    void set_name(std::string_view name) { name_ = name; }

private:
    /**
     * @brief Gerador de IDs sequenciais thread-safe.
     * Utiliza uma variável estática atômica compartilhada por todas as instâncias.
     */
    static Index next_uid() {
        static std::atomic<Index> counter{0};
        return ++counter; // Começa em 1, 0 ou -1 podem ser reservados para erros
    }

    Index uid_;         ///< Identificador único.
    std::string name_;  ///< Tag legível.
};

COMMON_NAMESPACE_CLOSE
CORE_NAMESPACE_CLOSE
FVGRIDMAKER_NAMESPACE_CLOSE