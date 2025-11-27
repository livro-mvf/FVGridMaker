// ----------------------------------------------------------------------------
// File: ut_Severity.cpp
// Project: FVGridMaker
// Version: 1.0
// Description: Testes unitários para o enum Severity.
//              Verifica a hierarquia lógica (Order Relation) e valores base,
//              garantindo que a lógica de filtragem de logs funcionará.
// Last modified: 2025-11-25
// Author: João Flávio Vasconcellos
// License: GNU GPL v3
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// includes gtest
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>
// #include <type_traits>

// ----------------------------------------------------------------------------
// includes FVGridMaker
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/Severity.h>

// Namespaces
namespace Err = FVGridMaker::error;
using namespace FVGridMaker::error;

/**
 * @file ut_Severity.cpp
 * @brief Validação da ordem e propriedades do enum Severity.
 * @ingroup ErrorHandlingTests
 */

// ============================================================================
// TESTES DE HIERARQUIA (Order Relation)
// ============================================================================

/**
 * @test LogicalHierarchy
 * @brief Verifica se a ordem Trace < Debug < Info < ... < Fatal é respeitada.
 * Essa ordem é vital para o filtro: `if (msg_sev >= min_sev) log()`.
 */
TEST(SeverityTest, LogicalHierarchy) {
    EXPECT_LT(Severity::Trace,   Severity::Debug);
    EXPECT_LT(Severity::Debug,   Severity::Info);
    EXPECT_LT(Severity::Info,    Severity::Warning);
    EXPECT_LT(Severity::Warning, Severity::Error);
    EXPECT_LT(Severity::Error,   Severity::Fatal);
}

// ============================================================================
// TESTES DE VALORES E TIPOS
// ============================================================================

/**
 * @test UnderlyingValues
 * @brief Garante que os valores inteiros subjacentes são estáveis.
 * Importante se esses valores forem serializados ou passados para C APIs.
 */
TEST(SeverityTest, UnderlyingValues) {
    EXPECT_EQ(static_cast<int>(Severity::Trace),   0);
    EXPECT_EQ(static_cast<int>(Severity::Debug),   1);
    EXPECT_EQ(static_cast<int>(Severity::Info),    2);
    EXPECT_EQ(static_cast<int>(Severity::Warning), 3);
    EXPECT_EQ(static_cast<int>(Severity::Error),   4);
    EXPECT_EQ(static_cast<int>(Severity::Fatal),   5);
}

/**
 * @test TypeProperties
 * @brief Verifica propriedades do enum class.
 */
TEST(SeverityTest, TypeProperties) {
    EXPECT_TRUE(std::is_enum_v<Severity>);
    // Garante que não converte implicitamente para int
    EXPECT_FALSE((std::is_convertible_v<Severity, int>));
}