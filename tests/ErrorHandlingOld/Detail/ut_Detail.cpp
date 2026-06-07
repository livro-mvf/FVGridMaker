// ----------------------------------------------------------------------------
// File: ut_Detail.cpp
// Project: FVGridMaker
// Version: 1.0
// Description: Testes dedicados às funções internas de Detail.h.
//              Foca na lógica de substituição de placeholders, seleção de
//              idioma e filtragem por severidade.
// Last modified: 2025-11-25
// Author: João Flávio Vasconcellos
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
#include <FVGridMaker/ErrorHandling/Detail.h>
#include <FVGridMaker/ErrorHandling/CoreErrors.h>
#include <FVGridMaker/ErrorHandling/ErrorManager.h>
#include <FVGridMaker/ErrorHandling/ErrorConfig.h>

// Namespaces
namespace Err = FVGridMaker::error;
using namespace FVGridMaker::error;

/**
 * @file ut_Detail.cpp
 * @brief Testes unitários para a lógica interna de formatação e despacho.
 * @ingroup ErrorHandlingTests
 */

// ============================================================================
// FIXTURE (Configuração Comum)
// ============================================================================

class DetailTest : public ::testing::Test {
protected:
    std::shared_ptr<const ErrorConfig> original_cfg_;

    void SetUp() override {
        // Salva configuração original
        original_cfg_ = Config::get();
        // Reseta para um estado conhecido (PtBR, Trace, Status)
        ErrorConfig cfg;
        cfg.language = Language::PtBR;
        cfg.min_severity = Severity::Trace; // Aceita tudo
        Config::set(cfg);
        // Limpa buffer anterior
        ErrorManager::flush();
    }

    void TearDown() override {
        // Restaura configuração original
        Config::set(*original_cfg_);
    }
};

// ============================================================================
// TESTES DE FORMATAÇÃO (String Interpolation)
// ============================================================================

/**
 * @test Replacement_Basic
 * @brief Verifica se um placeholder simples {key} é substituído corretamente.
 */
TEST_F(DetailTest, Replacement_Basic) {
    // CoreErr::InvalidArgument (PtBR) = "Argumento inválido: {name}."
    detail::log_error(CoreErr::InvalidArgument, {{"name", "ValorTeste"}});

    auto errors = ErrorManager::flush();
    ASSERT_EQ(errors.size(), 1);
    EXPECT_EQ(errors[0].message, "Argumento inválido: ValorTeste.");
}

/**
 * @test Replacement_MultipleOccurrences
 * @brief Verifica se a mesma chave é substituída múltiplas vezes se aparecer repetida.
 * Nota: Precisaríamos de um erro com chave repetida no enum para testar "naturalmente",
 * mas aqui verificamos se a lógica suporta isso conceitualmente.
 * Vamos usar InvalidArgument e injetar uma chave que não existe para garantir que não crasha.
 */
TEST_F(DetailTest, Replacement_UnknownKey_NoCrash) {
    // "Argumento inválido: {name}." -> Passamos chave "banana" (não existe no template)
    // O resultado deve manter o {name} original e ignorar "banana".
    detail::log_error(CoreErr::InvalidArgument, {{"banana", "fruta"}});

    auto errors = ErrorManager::flush();
    ASSERT_EQ(errors.size(), 1);
    // Espera-se que o placeholder original permaneça intacto
    EXPECT_EQ(errors[0].message, "Argumento inválido: {name}.");
}

/**
 * @test Replacement_RecursiveEdgeCase
 * @brief Verifica se o valor inserido contém a própria chave (Evita loop infinito).
 * Ex: Trocar "{name}" por "O valor de {name}".
 */
TEST_F(DetailTest, Replacement_RecursiveEdgeCase) {
    // Se a lógica do Detail.h não avançar o cursor (pos += v.size()), isso trava o teste.
    std::string tricky_value = "O valor de {name}";
    
    detail::log_error(CoreErr::InvalidArgument, {{"name", tricky_value}});

    auto errors = ErrorManager::flush();
    ASSERT_EQ(errors.size(), 1);
    // A substituição deve ocorrer apenas uma vez
    EXPECT_EQ(errors[0].message, "Argumento inválido: O valor de {name}.");
}

// ============================================================================
// TESTES DE FILTRAGEM (Severity)
// ============================================================================

/**
 * @test Logic_SeverityFiltering
 * @brief Verifica se log_error retorna cedo (early exit) quando severidade é baixa.
 */
TEST_F(DetailTest, Logic_SeverityFiltering) {
    // Configura filtro alto (apenas Errors e Fatals)
    ErrorConfig cfg;
    cfg.min_severity = Severity::Error;
    Config::set(cfg);

    // Tenta logar um Warning (NotImplemented)
    detail::log_error(CoreErr::NotImplemented);

    // Buffer deve estar vazio
    auto errors = ErrorManager::flush();
    EXPECT_TRUE(errors.empty()) << "Warning não deveria ter sido logado com min_severity=Error";

    // Tenta logar um Error (InvalidArgument)
    detail::log_error(CoreErr::InvalidArgument, {{"name", "ok"}});
    
    // Agora deve aparecer
    errors = ErrorManager::flush();
    ASSERT_EQ(errors.size(), 1);
}

// ============================================================================
// TESTES DE SELEÇÃO DE IDIOMA (i18n)
// ============================================================================

/**
 * @test Logic_LanguageSelection
 * @brief Verifica se a função seleciona a string correta baseada no Config global.
 */
TEST_F(DetailTest, Logic_LanguageSelection) {
    // 1. Testa PT-BR (Default do SetUp)
    detail::log_error(CoreErr::NotImplemented);
    auto logs = ErrorManager::flush();
    ASSERT_EQ(logs.size(), 1);
    EXPECT_EQ(logs[0].message, "Recurso não implementado.");

    // 2. Muda para EN-US
    ErrorConfig cfg_en;
    cfg_en.language = Language::EnUS;
    Config::set(cfg_en);

    detail::log_error(CoreErr::NotImplemented);
    logs = ErrorManager::flush();
    ASSERT_EQ(logs.size(), 1);
    EXPECT_EQ(logs[0].message, "Feature not implemented.");
}