// ----------------------------------------------------------------------------
// File: ut_ErrorHandling.cpp
// Project: FVGridMaker
// Version: 2.0
// Description: Testes unitários completos do módulo ErrorHandling.
// License: GNU GPL v3
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// includes c++
// ----------------------------------------------------------------------------
#include <string>
#include <vector>

// ----------------------------------------------------------------------------
// includes gtest
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

// ----------------------------------------------------------------------------
// includes FVGridMaker
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/ErrorHandling.h>
#include <FVGridMaker/ErrorHandling/GridErrors.h>
#include <FVGridMaker/ErrorHandling/Status.h>

// Alias para facilitar
namespace Err = FVGridMaker::error;
using namespace FVGridMaker::error;

/**
 * @file ut_ErrorHandling.cpp
 * @brief Suíte de testes unitários para o subsistema de Tratamento de Erros.
 * @details Cobre configuração global, macros de logging, formatação i18n,
 * integração de novos domínios (Grid) e classes de retorno funcional.
 * @ingroup ErrorHandlingTests
 */

// ============================================================================
// SUÍTE: Infraestrutura e Configuração
// ============================================================================

/**
 * @test ConfigGlobal
 * @brief Verifica o comportamento Singleton e persistência da configuração.
 */
TEST(ErrorInfrastructure, ConfigGlobal) {
    auto original_cfg = Config::get();
    
    ErrorConfig new_cfg;
    new_cfg.language = Language::EnUS;
    new_cfg.policy = Policy::Status;
    new_cfg.min_severity = Severity::Debug;

    Config::set(new_cfg);

    auto current = Config::get();
    EXPECT_EQ(current->language, Language::EnUS);
    EXPECT_EQ(current->policy, Policy::Status);
    EXPECT_EQ(current->min_severity, Severity::Debug);
    EXPECT_NE(current->logger, nullptr);

    Config::set(*original_cfg);
}

/**
 * @test LogAndFlush
 * @brief Valida se o logger padrão armazena e limpa o buffer thread-local.
 */
TEST(ErrorInfrastructure, LogAndFlush) {
    auto original_cfg = Config::get();
    Config::set(ErrorConfig{}); 

    // Log direto via função interna
    detail::log_error(CoreErr::NotImplemented);

    auto errors = ErrorManager::flush();
    ASSERT_EQ(errors.size(), 1);
    EXPECT_EQ(errors[0].code, code(CoreErr::NotImplemented));
    EXPECT_EQ(errors[0].severity, Severity::Warning);

    // Garante que o flush limpou
    EXPECT_TRUE(ErrorManager::flush().empty());

    Config::set(*original_cfg);
}

// ============================================================================
// SUÍTE: Formatação e Localização (i18n)
// ============================================================================

/**
 * @test Localizacao
 * @brief Verifica a seleção correta de strings (PT-BR vs EN-US) baseada na config.
 */
TEST(ErrorFormatting, Localizacao) {
    auto original_cfg = Config::get();
    ErrorConfig cfg;

    // 1. Português
    cfg.language = Language::PtBR;
    Config::set(cfg);
    detail::log_error(FileErr::FileNotFound, {{"path", "f.txt"}});
    
    auto logs_br = ErrorManager::flush();
    ASSERT_EQ(logs_br.size(), 1);
    EXPECT_EQ(logs_br[0].message, "Arquivo não encontrado: f.txt.");

    // 2. Inglês
    cfg.language = Language::EnUS;
    Config::set(cfg);
    detail::log_error(FileErr::FileNotFound, {{"path", "f.txt"}});
    
    auto logs_us = ErrorManager::flush();
    ASSERT_EQ(logs_us.size(), 1);
    EXPECT_EQ(logs_us[0].message, "File not found: f.txt.");

    Config::set(*original_cfg);
}

// ============================================================================
// SUÍTE: Macros e Lógica de Fluxo
// ============================================================================

/**
 * @test PolicyStatus_NaoLanca
 * @brief Garante que a política 'Status' apenas loga e não lança exceções.
 */
TEST(ErrorMacros, PolicyStatus_NaoLanca) {
    auto original_cfg = Config::get();
    ErrorConfig cfg;
    cfg.policy = Policy::Status; 
    Config::set(cfg);

    EXPECT_NO_THROW({
        FVG_ERROR(CoreErr::InvalidArgument, {{"name", "x"}});
    });

    auto errors = ErrorManager::flush();
    ASSERT_EQ(errors.size(), 1);

    Config::set(*original_cfg);
}

/**
 * @test PolicyThrow_LancaEmErro
 * @brief Garante que a política 'Throw' lança exceção para erros graves.
 */
TEST(ErrorMacros, PolicyThrow_LancaEmErro) {
    auto original_cfg = Config::get();
    ErrorConfig cfg;
    cfg.policy = Policy::Throw;
    Config::set(cfg);

    EXPECT_THROW({
        // FileNotFound (Error) >= Warning -> Lança
        FVG_ERROR(FileErr::FileNotFound, {{"path", "fail.txt"}});
    }, FVGException);

    // Buffer deve estar vazio (exceção consome o erro)
    EXPECT_TRUE(ErrorManager::flush().empty());

    Config::set(*original_cfg);
}

/**
 * @test PolicyThrow_RespeitaSeveridadeBaixa
 * @brief Verifica se warnings são apenas logados (não lançados) mesmo em modo Throw.
 */
TEST(ErrorMacros, PolicyThrow_RespeitaSeveridadeBaixa) {
    auto original_cfg = Config::get();
    ErrorConfig cfg;
    cfg.policy = Policy::Throw; 
    cfg.min_severity = Severity::Info; // Loga tudo
    Config::set(cfg);

    // NotImplemented é Warning. Warning < Error. Não deve lançar.
    EXPECT_NO_THROW({
        FVG_ERROR(CoreErr::NotImplemented);
    });

    auto errors = ErrorManager::flush();
    ASSERT_EQ(errors.size(), 1);
    EXPECT_EQ(errors[0].severity, Severity::Warning);

    Config::set(*original_cfg);
}

/**
 * @test Assert
 * @brief Verifica se a macro FVG_ASSERT dispara erro Fatal e exceção.
 */
TEST(ErrorMacros, Assert) {
    auto original_cfg = Config::get();
    ErrorConfig cfg; 
    cfg.policy = Policy::Throw; 
    Config::set(cfg);

    // Sucesso
    EXPECT_NO_THROW(FVG_ASSERT(true));
    
    // Falha
    try {
        FVG_ASSERT(false, {{"val", "0"}});
        FAIL() << "FVG_ASSERT deve lançar exceção na falha.";
    } catch (const FVGException& e) {
        EXPECT_EQ(e.code(), code(CoreErr::AssertFailed));
        EXPECT_EQ(e.severity(), Severity::Fatal);
    }

    Config::set(*original_cfg);
}

// ============================================================================
// SUÍTE: Domínios de Erro (Traits)
// ============================================================================

/**
 * @test GridErrorsIntegration
 * @brief Valida a integração e traits do domínio de erro GridErr.
 */
TEST(ErrorDomains, GridErrorsIntegration) {
    // Valida Traits do novo domínio Grid
    EXPECT_EQ(ErrorTraits<GridErr>::domain_name(), "Grid");
    EXPECT_EQ(ErrorTraits<GridErr>::domain_id(), 0x0003);

    // Valida lookup de mensagem
    auto msg = ErrorTraits<GridErr>::ptBR(GridErr::InvalidN);
    EXPECT_FALSE(msg.empty());
    EXPECT_EQ(ErrorTraits<GridErr>::default_severity(GridErr::InvalidN), Severity::Error);
}

// ============================================================================
// SUÍTE: Padrão Funcional (Status / StatusOr)
// ============================================================================

/**
 * @test StatusBasic
 * @brief Verifica o comportamento básico da classe Status (OK vs Error).
 */
TEST(FunctionalError, StatusBasic) {
    // OK
    Status st_ok = Status::OK();
    EXPECT_TRUE(st_ok.ok());
    
    // Erro
    ErrorRecord rec;
    rec.code = 500;
    rec.message = "Msg";
    Status st_fail(rec);
    EXPECT_FALSE(st_fail.ok());
    EXPECT_EQ(st_fail.message(), "Msg");
}

/**
 * @test StatusOrBasic
 * @brief Verifica o comportamento da classe StatusOr (Valor vs Status de Erro).
 */
TEST(FunctionalError, StatusOrBasic) {
    // Sucesso
    StatusOr<int> val(42);
    ASSERT_TRUE(val.ok());
    EXPECT_EQ(val.value(), 42);

    // Falha
    ErrorRecord rec;
    rec.message = "Falha";
    StatusOr<int> fail = Status(rec);
    
    EXPECT_FALSE(fail.ok());
    EXPECT_EQ(fail.status().message(), "Falha");
}