// ----------------------------------------------------------------------------
// File: .h
// Author: FVGridMaker Team
// Version: 1.0  
// Description: Umbrella header para o módulo de Erros.
// License: GNU GPL v3
// ----------------------------------------------------------------------------

#pragma once

/**
 * @defgroup error Error Handling
 * @brief O sistema de tratamento de erros, log e exceções.
 *
 * Este módulo fornece uma maneira extensível de definir códigos de erro,
 * gerar mensagens localizadas (Português/Inglês), logar erros e
 * opcionalmente lançar exceções.
 */

// ----------------------------------------------------------------------------
// includes FVGridMaker
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/Language.h>
#include <FVGridMaker/ErrorHandling/ErrorTraits.h>
#include <FVGridMaker/ErrorHandling/IErrorLogger.h>    
#include <FVGridMaker/ErrorHandling/ErrorManager.h>
#include <FVGridMaker/ErrorHandling/Macros.h>          
#include <FVGridMaker/ErrorHandling/FileErrors.h>     