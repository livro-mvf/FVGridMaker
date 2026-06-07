// ----------------------------------------------------------------------------
// File: ut_ErrorManager.cpp
// Project: FVGridMaker
// Version: 1.0
// Description: Testes para a fachada ErrorManager e o ThreadLocalBufferLogger.
//              Foca em concorrência (isolamento de buffer) e extensão (mocks).
// Last modified: 2025-11-25
// Author: João Flávio Vasconcellos
// License: GNU GPL v3
// ----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <thread>
#include <vector>
#include <algorithm>

// ----------------------------------------------------------------------------
// includes FVGridMaker
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/ErrorManager.h>
#include <FVGridMaker/ErrorHandling/ErrorConfig.h>

namespace Err = FVGridMaker::error;
using namespace FVGridMaker::error;

/**
 * @file ut_ErrorManager.cpp
 * @brief Testes da fachada ErrorManager e Logger Padrão.
 * @ingroup ErrorHandlingTests
 */

// ============================================================================
// FIXTURE
// ============================================================================
class ErrorManagerTest : public ::testing::Test {
protected:
    std::shared_ptr<const ErrorConfig> original_cfg_;

    void SetUp() override {
        original_cfg_ = Config::get();
        // Garante estado limpo e padrão
        Config::set(ErrorConfig{}); 
        ErrorManager::flush(); // Limpa thread atual
    }

    void TearDown() override {
        Config::set(*original_cfg_);
    }
};

// ============================================================================
// TESTES: ThreadLocalBufferLogger (Padrão)
// ============================================================================

/**
 * @test DefaultLogger_BasicFlow
 * @brief Verifica ciclo simples de Log -> Flush.
 */
TEST_F(ErrorManagerTest, DefaultLogger_BasicFlow) {
    ErrorRecord rec;
    rec.code = 10;
    rec.message = "Teste1";

    ErrorManager::log(rec);

    auto errors = ErrorManager::flush();
    ASSERT_EQ(errors.size(), 1);
    EXPECT_EQ(errors[0].message, "Teste1");

    // Flush deve limpar
    EXPECT_TRUE(ErrorManager::flush().empty());
}

/**
 * @test DefaultLogger_CapacityLimit
 * @brief Verifica se o logger respeita o limite de buffer configurado.
 */
TEST_F(ErrorManagerTest, DefaultLogger_CapacityLimit) {
    // Configura capacidade pequena
    ErrorConfig cfg;
    cfg.thread_buffer_cap = 2; 
    Config::set(cfg);

    // Tenta inserir 3 erros
    ErrorRecord r;
    r.message = "E";
    ErrorManager::log(r);
    ErrorManager::log(r);
    ErrorManager::log(r); // Deve ser descartado

    auto errors = ErrorManager::flush();
    EXPECT_EQ(errors.size(), 2) << "Deveria ter mantido apenas os 2 primeiros erros.";
}

/**
 * @test DefaultLogger_ThreadIsolation
 * @brief PROVA DE CONCEITO: Garante que erros de threads diferentes não se misturam.
 */
TEST_F(ErrorManagerTest, DefaultLogger_ThreadIsolation) {
    // Thread Principal: Loga erro A
    ErrorRecord recA; recA.message = "MainThread";
    ErrorManager::log(recA);

    // Thread Secundária: Loga erro B e verifica seu próprio buffer
    std::thread t([&]() {
        ErrorRecord recB; recB.message = "WorkerThread";
        ErrorManager::log(recB);

        // O flush dentro da thread deve retornar apenas "WorkerThread"
        auto local_errors = ErrorManager::flush();
        ASSERT_EQ(local_errors.size(), 1);
        EXPECT_EQ(local_errors[0].message, "WorkerThread");
    });

    t.join();

    // De volta à Thread Principal: O flush deve retornar apenas "MainThread"
    auto main_errors = ErrorManager::flush();
    ASSERT_EQ(main_errors.size(), 1);
    EXPECT_EQ(main_errors[0].message, "MainThread");
}

// ============================================================================
// TESTES: ErrorManager (Extensibilidade)
// ============================================================================

// Mock Logger Simples para teste
class MockLogger : public IErrorLogger {
public:
    std::vector<ErrorRecord> logs;
    void log(const ErrorRecord& r) override {
        logs.push_back(r);
    }
    std::vector<ErrorRecord> flush() override {
        std::vector<ErrorRecord> out = logs;
        logs.clear();
        return out;
    }
};

/**
 * @test Manager_CustomLoggerInjection
 * @brief Verifica se o ErrorManager redireciona para um logger customizado.
 */
TEST_F(ErrorManagerTest, Manager_CustomLoggerInjection) {
    auto mock = std::make_shared<MockLogger>();
    
    // Injeta o mock na configuração
    ErrorConfig cfg;
    cfg.logger = mock;
    Config::set(cfg);

    // Usa a fachada ErrorManager
    ErrorRecord rec;
    rec.message = "ViaMock";
    ErrorManager::log(rec);

    // Verifica se chegou no mock (bypassando o flush do manager para verificar o ponteiro direto)
    EXPECT_EQ(mock->logs.size(), 1);
    EXPECT_EQ(mock->logs[0].message, "ViaMock");

    // Verifica flush via manager
    auto flushed = ErrorManager::flush();
    EXPECT_EQ(flushed.size(), 1);
    EXPECT_TRUE(mock->logs.empty());
}