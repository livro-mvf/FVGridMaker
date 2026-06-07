// ----------------------------------------------------------------------------
// File: ut_ErrorRecord.cpp
// Project: FVGridMaker
// Version: 1.0
// Description: Testes unitários para a estrutura ErrorRecord.
//              Verifica a inicialização padrão (captura de contexto) e
//              a integridade dos dados.
// Last modified: 2025-11-25
// Author: João Flávio Vasconcellos
// License: GNU GPL v3
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// includes gtest
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

// ----------------------------------------------------------------------------
// includes FVGridMaker
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/ErrorRecord.h>

// Namespaces
namespace Err = FVGridMaker::error;
using namespace FVGridMaker::error;

/**
 * @file ut_ErrorRecord.cpp
 * @brief Validação da struct ErrorRecord e seus construtores implícitos.
 * @ingroup ErrorHandlingTests
 */

// ============================================================================
// TESTES DE INICIALIZAÇÃO
// ============================================================================

/**
 * @test DefaultConstruction_ContextCapture
 * @brief Verifica se timestamp e thread_id são capturados automaticamente.
 */
TEST(ErrorRecordTest, DefaultConstruction_ContextCapture) {
    // Instanciação padrão
    ErrorRecord rec;

    // 1. Verifica Defaults simples
    EXPECT_EQ(rec.code, 0);
    EXPECT_EQ(rec.severity, Severity::Error); // Default definido no header
    EXPECT_TRUE(rec.message.empty());

    // 2. Verifica Captura de Thread
    EXPECT_EQ(rec.tid, std::this_thread::get_id());

    // 3. Verifica Captura de Tempo (Sanity Check)
    // O timestamp do registro não deve ser muito diferente do "agora"
    auto now = std::chrono::system_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - rec.ts).count();
    
    // Aceita uma diferença pequena (ex: < 10ms) para execução do teste
    EXPECT_GE(diff, 0);
    EXPECT_LT(diff, 100) << "O timestamp capturado é muito antigo ou inválido.";
}

/**
 * @test AggregateInitialization
 * @brief Verifica a inicialização via Designadores (C++20) ou Braced Init.
 */
TEST(ErrorRecordTest, AggregateInitialization) {
    // Simula a criação feita pelo Detail.h
    ErrorRecord rec{
        .code = 0x00010005,
        .severity = Severity::Fatal,
        .message = "Erro Crítico"
        // .ts e .tid omitidos -> devem usar defaults
    };

    EXPECT_EQ(rec.code, 0x00010005);
    EXPECT_EQ(rec.severity, Severity::Fatal);
    EXPECT_EQ(rec.message, "Erro Crítico");
    
    // Defaults ainda devem funcionar
    EXPECT_EQ(rec.tid, std::this_thread::get_id());
}

// ============================================================================
// TESTES DE CONCORRÊNCIA (Contexto)
// ============================================================================

/**
 * @test ThreadContextAccuracy
 * @brief Garante que ErrorRecord captura o ID da thread correta (não a main).
 */
TEST(ErrorRecordTest, ThreadContextAccuracy) {
    std::thread::id main_id = std::this_thread::get_id();
    std::thread::id worker_id;
    ErrorRecord worker_rec;

    // Lança thread secundária
    std::thread t([&]() {
        worker_id = std::this_thread::get_id();
        // Cria o registro DENTRO da thread
        worker_rec = ErrorRecord{}; 
        worker_rec.message = "From Worker";
    });
    t.join();

    // Verificações
    EXPECT_NE(main_id, worker_id);
    EXPECT_EQ(worker_rec.tid, worker_id) 
        << "O registro deveria ter o ID da thread worker, não da main.";
    EXPECT_EQ(worker_rec.message, "From Worker");
}