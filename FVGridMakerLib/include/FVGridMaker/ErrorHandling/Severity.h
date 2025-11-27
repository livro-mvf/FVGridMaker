// ----------------------------------------------------------------------------
// File: Severity.h
// Project: FVGridMaker
// Version: 1.1
// Description: Define a enumeração `Severity` que estabelece os níveis de
//              gravidade para logs e erros. A ordem dos valores é crítica
//              para a lógica de filtragem (Trace < ... < Fatal).
// Last modified: 2025-11-25
// Author: FVGridMaker Team
// License: GNU GPL v3
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// includes FVGridMaker
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/Common/namespace.hpp>

/**
 * @file Severity.h
 * @brief Níveis de prioridade/gravidade para o sistema de log.
 * @ingroup ErrorHandling
 */

FVGRIDMAKER_NAMESPACE_OPEN
ERROR_NAMESPACE_OPEN

/**
 * @enum Severity
 * @brief Nível de severidade de um evento de log.
 *
 * A ordem numérica é importante: valores menores indicam menor gravidade
 * (maior verbosidade) e valores maiores indicam maior gravidade.
 * Isto permite comparações como `if (current >= min_severity)`.
 */
enum class Severity {
    /** @brief Detalhes de fluxo, variáveis temporárias e rastreamento fino. */
    Trace = 0,

    /** @brief Informações úteis para diagnóstico durante o desenvolvimento. */
    Debug = 1,

    /** @brief Eventos informativos gerais sobre o estado da aplicação. */
    Info = 2,

    /** @brief Condições anormais que não impedem a execução, mas requerem atenção. */
    Warning = 3,

    /** @brief Falhas que impedem uma função ou operação de concluir com sucesso. */
    Error = 4,

    /** @brief Erros críticos que tornam a aplicação instável ou exigem aborto. */
    Fatal = 5
};

ERROR_NAMESPACE_CLOSE
FVGRIDMAKER_NAMESPACE_CLOSE

