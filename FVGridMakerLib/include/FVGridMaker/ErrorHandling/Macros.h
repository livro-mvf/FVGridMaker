// ============================================================================
// File: Macros.h
// Author: FVGridMaker Team
// Version: 1.0 (Criado e Adaptado)
// Description: Macros de conveniência (FVG_ERROR, FVG_ASSERT).
// License: GNU GPL v3
// ============================================================================

#pragma once

// ----------------------------------------------------------------------------
// includes FVGridMaker
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/Detail.h>       // Para detail::log_error
#include <FVGridMaker/ErrorHandling/FVGException.h>// Para FVGException
#include <FVGridMaker/ErrorHandling/CoreErrors.h>   // Para CoreErr::AssertFailed
// #include <FVGridMaker/ErrorHandling/ErrorConfig.h>  // Para Policy::Throw
// #include <FVGridMaker/ErrorHandling/ErrorManager.h> // Para ErrorManager::flush
/**
 * @file Macros.h
 * @brief Macros de conveniência FVG_ERROR, FVG_ASSERT.
 * @ingroup ErrorHandling
 */

/**
 * @brief Macro principal para registrar e/ou lançar um erro (RNF08).
 *
 * Loga o erro se a severidade for >= min_severity.
 * Lança FVGException se policy == Throw E a severidade do erro for >= Error.
 */
#define FVG_ERROR(ERR_CODE, ...)                                           \
    do {                                                                    \
        /* 1. Loga o erro (formata a mensagem, respeita min_severity) */    \
        FVGridMaker::error::detail::log_error(ERR_CODE, ##__VA_ARGS__);    \
                                                                            \
        /* 2. Verifica a política global */                                 \
        auto fvg_err_cfg_ = FVGridMaker::error::Config::get();            \
        if (fvg_err_cfg_ && fvg_err_cfg_->policy == FVGridMaker::error::Policy::Throw) { \
            /* --- CORREÇÃO AQUI --- */                                     \
            /* 3. Verifica a severidade DESTE erro específico */            \
            using ErrType_ = decltype(ERR_CODE);                            \
            const FVGridMaker::error::Severity fvg_err_sev_ =             \
                FVGridMaker::error::ErrorTraits<ErrType_>::default_severity(ERR_CODE); \
            /* 4. Lança APENAS se for Error ou Fatal */                     \
            if (fvg_err_sev_ >= FVGridMaker::error::Severity::Error) {     \
            /* --- FIM DA CORREÇÃO --- */                                   \
                auto fvg_err_errors_ = FVGridMaker::error::ErrorManager::flush(); \
                if (!fvg_err_errors_.empty()) {                            \
                    /* Lança com o registro completo do log */              \
                    throw FVGridMaker::error::FVGException(std::move(fvg_err_errors_.back())); \
                } else {                                                    \
                    /* Fallback: Lança com info mínima se o log falhou */   \
                    throw FVGridMaker::error::FVGException(               \
                       {FVGridMaker::error::code(ERR_CODE),              \
                        fvg_err_sev_,                                     \
                        "Erro grave lançado (verifique log/severidade)"}); \
                }                                                           \
            }                                                               \
        }                                                                   \
    } while (0)


/**
 * @brief Macro para asserções internas. Lança um erro Fatal se a condição for falsa.
 */
#define FVG_ASSERT(condition, ...)                                         \
    do {                                                                    \
        if (!(condition)) {                                                 \
            /* Chama FVG_ERROR com AssertFailed (que é Fatal e vai lançar se policy=Throw) */ \
            FVG_ERROR(FVGridMaker::error::CoreErr::AssertFailed, ##__VA_ARGS__); \
        }                                                                   \
    } while (0)


    