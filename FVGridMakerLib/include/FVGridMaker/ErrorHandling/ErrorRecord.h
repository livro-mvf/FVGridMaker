// ----------------------------------------------------------------------------
// File: ErrorRecord.h
// Project: FVGridMaker
// Version: 1.0
// Description: Define a estrutura de dados (DTO) que representa um erro
//              capturado. Contém metadados contextuais (timestamp, thread ID)
//              além dos dados do erro em si.
// Last modified: 2025-11-25
// Author: FVGridMaker Team
// License: GNU GPL v3
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// includes C++
// ----------------------------------------------------------------------------
#include <thread>

// ----------------------------------------------------------------------------
// includes FVGridMaker
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/Severity.h>

/**
 * @file ErrorRecord.h
 * @brief Estrutura de dados para registro de erros.
 * @ingroup ErrorHandling
 */

FVGRIDMAKER_NAMESPACE_OPEN
ERROR_NAMESPACE_OPEN

/**
 * @struct ErrorRecord
 * @brief Guarda toda informação contextual sobre um evento de erro.
 *
 * Esta estrutura é projetada para ser um "Plain Old Data" (POD) ou Agregado,
 * facilitando a movimentação entre buffers e loggers.
 *
 * Ao ser instanciada sem parâmetros para `ts` e `tid`, ela captura
 * automaticamente o momento e a thread de criação.
 */
struct ErrorRecord {
    /**
     * @brief Código de erro único composto.
     * Formato: (DomainID << 16) | ErrorValue.
     */
    std::uint32_t code{0};

    /**
     * @brief Nível de severidade do erro.
     * Padrão: Error (para garantir visibilidade se não especificado).
     */
    Severity severity{Severity::Error};

    /**
     * @brief Mensagem descritiva final.
     * Já processada (i18n) e formatada (placeholders substituídos).
     */
    std::string message;

    /**
     * @brief Carimbo de tempo da ocorrência.
     * Inicializado automaticamente com `std::chrono::system_clock::now()`.
     */
    std::chrono::system_clock::time_point ts{std::chrono::system_clock::now()};

    /**
     * @brief Identificador da thread onde o erro ocorreu.
     * Inicializado automaticamente com `std::this_thread::get_id()`.
     */
    std::thread::id tid{std::this_thread::get_id()};
};

ERROR_NAMESPACE_CLOSE
FVGRIDMAKER_NAMESPACE_CLOSE