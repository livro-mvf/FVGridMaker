// ----------------------------------------------------------------------------
// File: ut_ErrorConfig.cpp
// Project: FVGridMaker
// Version: 1.0
// Description: Testes dedicados à classe de configuração e Singleton.
//              Foca na validação dos valores padrão e, crucialmente, na
//              segurança de thread (thread-safety) do acesso global.
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
#include <FVGridMaker/ErrorHandling/ErrorConfig.h>
#include <FVGridMaker/ErrorHandling/ErrorManager.h>  

// Namespaces
namespace Err = FVGridMaker::error;
using namespace FVGridMaker::error;

/**
 * @file ut_ErrorConfig.cpp
 * @brief Testes de unidade para a estrutura de configuração e Singleton.
 * @ingroup ErrorHandlingTests
 */

// ============================================================================
// TESTES DE VALORES PADRÃO
// ============================================================================

/**
 * @test DefaultValues
 * @brief Verifica se a configuração inicial (factory default) está correta.
 */
TEST(ErrorConfigTest, DefaultValues) {
    // Cria uma instância limpa
    ErrorConfig cfg;

    // Verifica padrões definidos no header
    EXPECT_EQ(cfg.language, Language::PtBR);
    EXPECT_EQ(cfg.policy, Policy::Throw);
    EXPECT_EQ(cfg.min_severity, Severity::Warning);
    EXPECT_EQ(cfg.thread_buffer_cap, 256);
    
    // O logger deve ser inicializado (não nulo) graças ao construtor em ErrorManager.h
    EXPECT_NE(cfg.logger, nullptr);
}

// ============================================================================
// TESTES DE SINGLETON (Lógica de Ponteiro)
// ============================================================================

/**
 * @test UpdateMechanism
 * @brief Verifica se Config::set atualiza corretamente o ponteiro global.
 */
TEST(ErrorConfigTest, UpdateMechanism) {
    // 1. Pega config original
    auto initial_ptr = Config::get();
    ASSERT_NE(initial_ptr, nullptr);

    // 2. Cria nova config
    ErrorConfig new_cfg;
    new_cfg.thread_buffer_cap = 1024; // Valor arbitrário diferente
    new_cfg.policy = Policy::Status;

    // 3. Aplica
    Config::set(new_cfg);

    // 4. Verifica se atualizou
    auto updated_ptr = Config::get();
    EXPECT_NE(initial_ptr, updated_ptr) << "O ponteiro deve mudar após um set()";
    EXPECT_EQ(updated_ptr->thread_buffer_cap, 1024);
    EXPECT_EQ(updated_ptr->policy, Policy::Status);

    // Restaura para o teste não afetar outros (embora unitários devam ser isolados)
    Config::set(*initial_ptr);
}

// ============================================================================
// TESTES DE CONCORRÊNCIA (Thread Safety)
// ============================================================================

/**
 * @test ThreadSafety_StressTest
 * @brief Teste de estresse para verificar Race Conditions.
 *
 * Cria várias threads: metade lendo (get) e metade escrevendo (set)
 * simultaneamente. Se o Mutex não estiver funcionando, isso causará
 * Double Free ou Segmentation Fault no shared_ptr.
 */
TEST(ErrorConfigTest, ThreadSafety_StressTest) {
    const int num_threads = 20;
    const int iterations_per_thread = 1000;
    std::vector<std::thread> threads;
    std::atomic<bool> run{true};

    // Configuração base para restaurar depois
    auto original = Config::get();

    for (int i = 0; i < num_threads; ++i) {
        if (i % 2 == 0) {
            // --- Threads Escritoras ---
            threads.emplace_back([&]() {
                for (int j = 0; j < iterations_per_thread; ++j) {
                    ErrorConfig cfg;
                    // Altera um valor aleatoriamente para garantir escritas diferentes
                    cfg.thread_buffer_cap = static_cast<std::size_t>(j); 
                    Config::set(cfg);
                }
            });
        } else {
            // --- Threads Leitoras ---
            threads.emplace_back([&]() {
                for (int j = 0; j < iterations_per_thread; ++j) {
                    auto current = Config::get();
                    // Apenas garante que o ponteiro é válido
                    if (!current) {
                        ADD_FAILURE() << "Config::get() retornou nullptr durante stress test";
                    }
                    // Simula uma leitura leve
                    volatile auto lang = current->language;
                    (void)lang;
                }
            });
        }
    }

    // Aguarda todas as threads terminarem
    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }

    // Se chegou aqui sem crash (SegFault), o Mutex funcionou.
    SUCCEED();

    // Restaura original
    Config::set(*original);
}