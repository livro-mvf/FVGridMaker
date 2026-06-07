// ----------------------------------------------------------------------------
// File: ut_IErrorLogger.cpp
// Project: FVGridMaker
// Version: 1.0
// Description: Testes unitários para a Interface IErrorLogger.
//              Valida o contrato da interface, destrutores virtuais e
//              implementações padrão através de uma classe Mock.
// Last modified: 2025-11-25
// Author: João Flávio Vasconcellos
// License: GNU GPL v3
// ----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <memory>
#include <vector>

// ----------------------------------------------------------------------------
// includes FVGridMaker
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/IErrorLogger.h>

// Namespaces
namespace Err = FVGridMaker::error;
using namespace FVGridMaker::error;

/**
 * @file ut_IErrorLogger.cpp
 * @brief Testes do contrato da interface IErrorLogger.
 * @ingroup ErrorHandlingTests
 */

// ============================================================================
// MOCK IMPLEMENTATION (Para testar a Interface Abstrata)
// ============================================================================

/**
 * @class MockLogger
 * @brief Implementação mínima para validar o polimorfismo.
 */
class MockLogger : public IErrorLogger {
public:
    int log_calls = 0;
    int flush_calls = 0;
    
    // Implementação obrigatória do método puro
    void log(const ErrorRecord& /*record*/) override {
        log_calls++;
    }

    // Sobrescrita opcional (para testar dispatch)
    std::vector<ErrorRecord> flush() override {
        flush_calls++;
        return {};
    }
};

/**
 * @class DefaultFlushLogger
 * @brief Implementação que NÃO sobrescreve flush() para testar o default.
 */
class DefaultFlushLogger : public IErrorLogger {
public:
    void log(const ErrorRecord&) override {}
};

// ============================================================================
// TESTES DE POLIMORFISMO
// ============================================================================

/**
 * @test VirtualDestructor
 * @brief Garante que deletar pelo ponteiro da base chama o destrutor correto.
 * (Detectado por ferramentas de análise de memória como Valgrind/ASan).
 */
TEST(IErrorLoggerTest, VirtualDestructor) {
    std::shared_ptr<IErrorLogger> logger = std::make_shared<MockLogger>();
    // Ao sair do escopo, o destrutor deve rodar sem vazamento.
    SUCCEED();
}

/**
 * @test VirtualDispatch_Log
 * @brief Verifica se a chamada na interface invoca a implementação derivada.
 */
TEST(IErrorLoggerTest, VirtualDispatch_Log) {
    auto mock = std::make_shared<MockLogger>();
    IErrorLogger* interface_ptr = mock.get();

    ErrorRecord rec;
    interface_ptr->log(rec);

    EXPECT_EQ(mock->log_calls, 1);
}

/**
 * @test VirtualDispatch_Flush
 * @brief Verifica se a chamada na interface invoca o flush derivado.
 */
TEST(IErrorLoggerTest, VirtualDispatch_Flush) {
    auto mock = std::make_shared<MockLogger>();
    IErrorLogger* interface_ptr = mock.get();

    interface_ptr->flush();

    EXPECT_EQ(mock->flush_calls, 1);
}

// ============================================================================
// TESTES DE COMPORTAMENTO PADRÃO
// ============================================================================

/**
 * @test DefaultFlushBehavior
 * @brief Verifica se a implementação padrão de flush() retorna vetor vazio.
 */
TEST(IErrorLoggerTest, DefaultFlushBehavior) {
    auto logger = std::make_shared<DefaultFlushLogger>();
    
    // O logger não implementou flush(), deve usar o default da base
    auto result = logger->flush();
    
    EXPECT_TRUE(result.empty());
    EXPECT_EQ(result.size(), 0);
}