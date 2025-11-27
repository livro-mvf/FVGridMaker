// ----------------------------------------------------------------------------
// File: Language.h
// Project: FVGridMaker
// Version: 1.0
// Description: Define a enumeração `Language` para seleção de idioma em
//              mensagens de erro localizadas (i18n).
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
 * @file Language.h
 * @brief Definição dos idiomas suportados pelo sistema.
 * @ingroup ErrorHandling
 */

FVGRIDMAKER_NAMESPACE_OPEN
ERROR_NAMESPACE_OPEN

/**
 * @enum Language
 * @brief Idiomas disponíveis para internacionalização (i18n).
 *
 * Utilizado na configuração global (`ErrorConfig`) para determinar qual
 * versão da string de erro será renderizada pelo `ErrorTraits`.
 */
enum class Language {
    /** @brief Inglês Americano (Padrão internacional). */
    EnUS,

    /** @brief Português Brasileiro (Idioma nativo do projeto). */
    PtBR
};

ERROR_NAMESPACE_CLOSE
FVGRIDMAKER_NAMESPACE_CLOSE