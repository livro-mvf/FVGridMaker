// ----------------------------------------------------------------------------
// File: ut_GridErrors.cpp
// Project: FVGridMaker
// Version: 1.0
// Description: Testes dedicados para validar o conteúdo de GridErrors.h.
//              Verifica integridade das strings, mapeamento de IDs e Severidade.
// License: GNU GPL v3
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// includes gtest
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>


// ----------------------------------------------------------------------------
// includes FVGridMaker
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/GridErrors.h>

// Namespaces
namespace Err = FVGridMaker::error;
using namespace FVGridMaker::error;

/**
 * @file ut_GridErrors.cpp
 * @brief Validação de integridade do domínio GridErrors.
 * @ingroup ErrorHandlingTests
 */

// ============================================================================
// TESTES DE INTEGRIDADE DE DADOS
// ============================================================================

/**
 * @test DomainTraits
 * @brief Verifica se os Traits do domínio estão configurados corretamente (ID 0x0003).
 */
TEST(GridErrorsTest, DomainTraits) {
    EXPECT_EQ(ErrorTraits<GridErr>::domain_id(), 0x0003);
    EXPECT_EQ(ErrorTraits<GridErr>::domain_name(), "Grid");
    EXPECT_TRUE(std::is_enum_v<GridErr>);
}

/**
 * @test AllErrorsHaveMessages
 * @brief Itera por TODOS os erros do enum para garantir que nenhum foi esquecido.
 */
TEST(GridErrorsTest, AllErrorsHaveMessages) {
    auto start = static_cast<std::uint16_t>(GridErr::_Min);
    auto end   = static_cast<std::uint16_t>(GridErr::_Max);

    for (auto i = start; i <= end; ++i) {
        auto e = static_cast<GridErr>(i);

        // Chave
        std::string_view key = ErrorTraits<GridErr>::key(e);
        EXPECT_FALSE(key.empty()) << "Erro ID " << i << " sem chave definida.";

        // Mensagens
        EXPECT_FALSE(ErrorTraits<GridErr>::ptBR(e).empty()) << key << " sem PT-BR.";
        EXPECT_FALSE(ErrorTraits<GridErr>::enUS(e).empty()) << key << " sem EN-US.";
    }
}

// ============================================================================
// TESTES DE VALORES ESPECÍFICOS
// ============================================================================

TEST(GridErrorsTest, SpecificValues_InvalidN) {
    constexpr auto e = GridErr::InvalidN;
    
    EXPECT_EQ(ErrorTraits<GridErr>::default_severity(e), Severity::Error);
    EXPECT_EQ(ErrorTraits<GridErr>::key(e), "GRID_INVALID_N");
    
    std::string pt = std::string(ErrorTraits<GridErr>::ptBR(e));
    EXPECT_NE(pt.find("{N}"), std::string::npos) << "Placeholder {N} faltando.";
}

TEST(GridErrorsTest, SpecificValues_ExecPolicyUnsupported) {
    constexpr auto e = GridErr::ExecPolicyUnsupported;
    
    // Este erro deve ser Warning, não Error
    EXPECT_EQ(ErrorTraits<GridErr>::default_severity(e), Severity::Warning);
    EXPECT_EQ(ErrorTraits<GridErr>::key(e), "GRID_EXEC_POLICY_UNSUPPORTED");
}

TEST(GridErrorsTest, SpecificValues_InvalidDomain) {
    constexpr auto e = GridErr::InvalidDomain;
    
    std::string en = std::string(ErrorTraits<GridErr>::enUS(e));
    EXPECT_NE(en.find("{A}"), std::string::npos);
    EXPECT_NE(en.find("{B}"), std::string::npos);
}

// ============================================================================
// TESTES DE LIMITES (Bounds)
// ============================================================================

TEST(GridErrorsTest, InvalidEnumValues) {
    auto invalid_err = static_cast<GridErr>(9999);

    EXPECT_EQ(ErrorTraits<GridErr>::default_severity(invalid_err), Severity::Trace);
    EXPECT_TRUE(ErrorTraits<GridErr>::key(invalid_err).empty());
}