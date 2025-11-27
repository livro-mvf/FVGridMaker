// ----------------------------------------------------------------------------
// File: ut_CoreErrors.cpp
// Project: FVGridMaker
// Version: 1.0
// Description: Testes dedicados para validar o conteúdo de CoreErrors.h.
//              Verifica integridade das strings, mapeamento de IDs e Severidade.
// License: GNU GPL v3
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// includes gtest
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

// #include <string_view>
// #include <type_traits>

// Includes do alvo do teste
#include <FVGridMaker/ErrorHandling/CoreErrors.h>
#include <FVGridMaker/ErrorHandling/ErrorTraits.h>

// Namespaces
namespace Err = FVGridMaker::error;
using namespace FVGridMaker::error;

/**
 * @file ut_CoreErrors.cpp
 * @brief Validação de integridade do domínio CoreErrors.
 */

// ============================================================================
// TESTES DE INTEGRIDADE DE DADOS
// ============================================================================

/**
 * @test DomainTraits
 * @brief Verifica se os Traits do domínio estão configurados corretamente (ID 0x0001).
 */
TEST(CoreErrorsTest, DomainTraits) {
    // Verifica IDs e Nomes
    EXPECT_EQ(ErrorTraits<CoreErr>::domain_id(), 0x0001);
    EXPECT_EQ(ErrorTraits<CoreErr>::domain_name(), "Core");
    
    // Verifica conceito de Enum
    EXPECT_TRUE(std::is_enum_v<CoreErr>);
}

/**
 * @test AllErrorsHaveMessages
 * @brief Itera por TODOS os erros do enum para garantir que nenhum foi esquecido.
 * Isso protege contra adicionar um erro no Enum e esquecer o switch-case.
 */
TEST(CoreErrorsTest, AllErrorsHaveMessages) {
    // Itera do _Min ao _Max
    auto start = static_cast<std::uint16_t>(CoreErr::_Min);
    auto end   = static_cast<std::uint16_t>(CoreErr::_Max);

    for (auto i = start; i <= end; ++i) {
        auto e = static_cast<CoreErr>(i);

        // 1. Verifica se existe uma chave definida
        std::string_view key = ErrorTraits<CoreErr>::key(e);
        EXPECT_FALSE(key.empty()) 
            << "Erro com ID " << i << " nao tem chave (KEY) definida.";

        // 2. Verifica mensagem em PT-BR
        std::string_view pt = ErrorTraits<CoreErr>::ptBR(e);
        EXPECT_FALSE(pt.empty()) 
            << "Erro " << key << " nao tem mensagem PT-BR.";

        // 3. Verifica mensagem em EN-US
        std::string_view en = ErrorTraits<CoreErr>::enUS(e);
        EXPECT_FALSE(en.empty()) 
            << "Erro " << key << " nao tem mensagem EN-US.";
    }
}

// ============================================================================
// TESTES DE VALORES ESPECÍFICOS (Spot Checks)
// ============================================================================

/**
 * @test SpecificValues_InvalidArgument
 * @brief Valida dados específicos do erro InvalidArgument.
 */
TEST(CoreErrorsTest, SpecificValues_InvalidArgument) {
    constexpr auto e = CoreErr::InvalidArgument;
    
    EXPECT_EQ(ErrorTraits<CoreErr>::default_severity(e), Severity::Error);
    EXPECT_EQ(ErrorTraits<CoreErr>::key(e), "CORE_INVALID_ARGUMENT");
    
    // Verifica conteúdo da string (parcialmente para permitir mudanças pequenas)
    std::string pt = std::string(ErrorTraits<CoreErr>::ptBR(e));
    EXPECT_NE(pt.find("Argumento inválido"), std::string::npos);
    EXPECT_NE(pt.find("{name}"), std::string::npos); // Deve ter o placeholder
}

/**
 * @test SpecificValues_AssertFailed
 * @brief Valida se AssertFailed é Fatal (crítico para a macro FVG_ASSERT).
 */
TEST(CoreErrorsTest, SpecificValues_AssertFailed) {
    constexpr auto e = CoreErr::AssertFailed;
    
    // Assert Failed PRECISA ser Fatal, senão a macro FVG_ASSERT não aborta/lança
    EXPECT_EQ(ErrorTraits<CoreErr>::default_severity(e), Severity::Fatal);
    EXPECT_EQ(ErrorTraits<CoreErr>::key(e), "CORE_ASSERT_FAILED");
}

/**
 * @test SpecificValues_NotImplemented
 * @brief Valida se NotImplemented é Warning (não deve abortar o programa).
 */
TEST(CoreErrorsTest, SpecificValues_NotImplemented) {
    constexpr auto e = CoreErr::NotImplemented;
    
    EXPECT_EQ(ErrorTraits<CoreErr>::default_severity(e), Severity::Warning);
    EXPECT_EQ(ErrorTraits<CoreErr>::key(e), "CORE_NOT_IMPLEMENTED");
}

// ============================================================================
// TESTES DE LIMITES (Bounds)
// ============================================================================

/**
 * @test InvalidEnumValues
 * @brief Verifica o comportamento ao passar um inteiro inválido castado para CoreErr.
 * O sistema deve retornar valores seguros (vazios/Trace) e não crashar.
 */
TEST(CoreErrorsTest, InvalidEnumValues) {
    // Um valor fora do range definido em _Min/_Max
    auto invalid_err = static_cast<CoreErr>(9999);

    // Deve retornar severidade Trace (padrão seguro definido no switch default)
    EXPECT_EQ(ErrorTraits<CoreErr>::default_severity(invalid_err), Severity::Trace);
    
    // As mensagens devem ser vazias
    EXPECT_TRUE(ErrorTraits<CoreErr>::ptBR(invalid_err).empty());
    EXPECT_TRUE(ErrorTraits<CoreErr>::enUS(invalid_err).empty());
    EXPECT_TRUE(ErrorTraits<CoreErr>::key(invalid_err).empty());
}