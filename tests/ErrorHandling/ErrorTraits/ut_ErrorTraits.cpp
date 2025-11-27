// ----------------------------------------------------------------------------
// File: ut_ErrorTraits.cpp
// Project: FVGridMaker
// Version: 1.0
// Description: Testes unitários para a infraestrutura de Traits e Concepts.
//              Utiliza um Mock Enum local para validar a geração de códigos
//              e a detecção de conformidade (Concepts) sem depender de
//              outros arquivos de erro.
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
#include <FVGridMaker/ErrorHandling/ErrorTraits.h>

// Namespaces
namespace Err = FVGridMaker::error;
using namespace FVGridMaker::error;

/**
 * @file ut_ErrorTraits.cpp
 * @brief Validação da lógica de metaprogramação (Traits/Concepts).
 * @ingroup ErrorHandlingTests
 */

// ============================================================================
// DEFINIÇÃO DE MOCK (Enum para Teste)
// ============================================================================

// 1. Enum válido
enum class MockErr : std::uint16_t {
    ErrorA = 1,
    ErrorB = 255
};

// 2. Especialização Válida
namespace FVGridMaker::error {
    template <> struct ErrorTraits<MockErr> {
        static constexpr std::uint16_t domain_id() noexcept { return 0xAAAA; } // ID Teste
        static constexpr std::string_view domain_name() noexcept { return "Mock"; }
        
        static constexpr Severity default_severity(MockErr) noexcept { return Severity::Error; }
        static constexpr std::string_view key(MockErr) noexcept { return "MOCK_KEY"; }
        static constexpr std::string_view enUS(MockErr) noexcept { return "Msg"; }
        static constexpr std::string_view ptBR(MockErr) noexcept { return "Msg"; }
    };
}

// 3. Enum Inválido (sem traits)
enum class BadErr { Fail };

// ============================================================================
// TESTES
// ============================================================================

/**
 * @test BitwiseLogic_MakeCode
 * @brief Verifica se a função make_code monta o inteiro de 32 bits corretamente.
 * Esperado: (Domain << 16) | Value
 */
TEST(ErrorTraitsTest, BitwiseLogic_MakeCode) {
    std::uint16_t domain = 0x0001;
    std::uint16_t value  = 0x0002;
    
    // 0x0001 << 16 = 0x00010000
    // | 0x0002     = 0x00010002
    std::uint32_t expected = 0x00010002;
    
    EXPECT_EQ(make_code(domain, value), expected);

    // Teste com valores maiores
    domain = 0xFFFF;
    value  = 0xFFFF;
    expected = 0xFFFFFFFF;
    EXPECT_EQ(make_code(domain, value), expected);
}

/**
 * @test CodeHelperFunction
 * @brief Verifica se a função helper 'code<E>(e)' usa os traits corretamente.
 */
TEST(ErrorTraitsTest, CodeHelperFunction) {
    // MockErr::ErrorA = 1, Domain = 0xAAAA
    // Esperado: 0xAAAA0001
    std::uint32_t expected_a = 0xAAAA0001;
    EXPECT_EQ(code(MockErr::ErrorA), expected_a);

    // MockErr::ErrorB = 255 (0x00FF), Domain = 0xAAAA
    // Esperado: 0xAAAA00FF
    std::uint32_t expected_b = 0xAAAA00FF;
    EXPECT_EQ(code(MockErr::ErrorB), expected_b);
}

/**
 * @test ConceptValidation
 * @brief Verifica em tempo de compilação (static_assert) se o concept funciona.
 * Este teste não tem asserções de runtime (EXPECT_*), pois falharia na compilação
 * se a lógica estivesse errada.
 */
TEST(ErrorTraitsTest, ConceptValidation) {
    // 1. MockErr deve satisfazer ErrorEnum
    static_assert(ErrorEnum<MockErr>, "MockErr deveria ser um ErrorEnum válido");

    // 2. BadErr (sem traits) NÃO deve satisfazer ErrorEnum
    static_assert(!ErrorEnum<BadErr>, "BadErr não tem traits, não deveria ser válido");

    // 3. Tipos primitivos NÃO devem satisfazer
    static_assert(!ErrorEnum<int>, "Int não é enum");
    static_assert(!ErrorEnum<float>, "Float não é enum");

    SUCCEED(); // Marca o teste como passado se compilou até aqui
}