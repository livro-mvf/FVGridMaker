// ----------------------------------------------------------------------------
// File: IErrorLogger.h
// Project: FVGridMaker
// Version: 1.1
// Description: Define a interface abstrata (ABC) para implementação de
//              loggers de erro (ex: Console, File, Database, Memory).
// Last modified: 2025-11-25
// Author: FVGridMaker Team
// License: GNU GPL v3
// ----------------------------------------------------------------------------

#pragma once


// ----------------------------------------------------------------------------
// includes FVGridMaker
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/ErrorRecord.h>

/**
 * @file IErrorLogger.h
 * @brief Interface base para Loggers de Erro.
 * @ingroup ErrorHandling
 */

FVGRIDMAKER_NAMESPACE_OPEN
ERROR_NAMESPACE_OPEN

/**
 * @class IErrorLogger
 * @brief Interface Abstrata (Protocolo) para loggers de erro.
 *
 * Qualquer classe que pretenda receber e armazenar erros do sistema deve
 * herdar desta interface. Isso permite a Injeção de Dependência no `Config`.
 */
class IErrorLogger {
public:
    /**
     * @brief Destrutor virtual padrão.
     * Essencial para garantir a destruição correta de objetos polimórficos.
     */
    virtual ~IErrorLogger() = default;

    /**
     * @brief Registra um evento de erro.
     * @param record O registro contendo mensagem, severidade e metadados.
     * @note Este método deve ser thread-safe se o logger for compartilhado.
     */
    virtual void log(const ErrorRecord& record) = 0;

    /**
     * @brief Descarrega e retorna os erros armazenados no buffer.
     *
     * Loggers que não utilizam buffer (ex: escrita direta em arquivo ou stdout)
     * podem manter a implementação padrão que retorna um vetor vazio.
     *
     * @return std::vector<ErrorRecord> Lista de erros removidos do buffer.
     */
    virtual std::vector<ErrorRecord> flush() { return {}; }
};

ERROR_NAMESPACE_CLOSE
FVGRIDMAKER_NAMESPACE_CLOSE