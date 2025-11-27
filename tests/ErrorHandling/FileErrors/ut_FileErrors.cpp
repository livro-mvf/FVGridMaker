// ----------------------------------------------------------------------------
// File: ut_FileErrors.cpp
// Project: FVGridMaker
// Version: 1.0
// Description: Testes dedicados para validar o conteúdo de FileErrors.h.
//              Verifica integridade das strings, mapeamento de IDs e Severidade.
// License: GNU GPL v3
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// includes gtest
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>


// Includes do alvo do teste
#include <FVGridMaker/ErrorHandling/FileErrors.h>
// #include <FVGridMaker/ErrorHandling/ErrorTraits.h>

// Namespaces
namespace Err = FVGridMaker::error;
using namespace FVGridMaker::error;

/**
 * @file ut_FileErrors.cpp
 * @brief Validação de integridade do domínio FileErrors.
 * @ingroup ErrorHandlingTests
 */

// ============================================================================
// TESTES DE INTEGRIDADE DE DADOS
// ============================================================================

/**
 * @test DomainTraits
 * @brief Verifica se os Traits do domínio estão configurados corretamente (ID 0x0002).
 */
TEST(FileErrorsTest, DomainTraits) {
    EXPECT_EQ(ErrorTraits<FileErr>::domain_id(), 0x0002);
    EXPECT_EQ(ErrorTraits<FileErr>::domain_name(), "File");
    EXPECT_TRUE(std::is_enum_v<FileErr>);
}

/**
 * @test AllErrorsHaveMessages
 * @brief Itera por TODOS os erros do enum para garantir que nenhum foi esquecido.
 */
TEST(FileErrorsTest, AllErrorsHaveMessages) {
    auto start = static_cast<std::uint16_t>(FileErr::_Min);
    auto end   = static_cast<std::uint16_t>(FileErr::_Max);

    for (auto i = start; i <= end; ++i) {
        auto e = static_cast<FileErr>(i);

        // Chave deve existir
        std::string_view key = ErrorTraits<FileErr>::key(e);
        EXPECT_FALSE(key.empty()) << "Erro ID " << i << " sem chave definida.";

        // Mensagens devem existir
        EXPECT_FALSE(ErrorTraits<FileErr>::ptBR(e).empty()) << key << " sem PT-BR.";
        EXPECT_FALSE(ErrorTraits<FileErr>::enUS(e).empty()) << key << " sem EN-US.";
    }
}

// ============================================================================
// TESTES DE VALORES ESPECÍFICOS
// ============================================================================

TEST(FileErrorsTest, SpecificValues_FileNotFound) {
    constexpr auto e = FileErr::FileNotFound;
    
    EXPECT_EQ(ErrorTraits<FileErr>::default_severity(e), Severity::Error);
    EXPECT_EQ(ErrorTraits<FileErr>::key(e), "FILE_NOT_FOUND");
    
    std::string pt = std::string(ErrorTraits<FileErr>::ptBR(e));
    // Verifica placeholder
    EXPECT_NE(pt.find("{path}"), std::string::npos);
    EXPECT_NE(pt.find("não encontrado"), std::string::npos);
}

TEST(FileErrorsTest, SpecificValues_AccessDenied) {
    constexpr auto e = FileErr::AccessDenied;
    
    EXPECT_EQ(ErrorTraits<FileErr>::default_severity(e), Severity::Error);
    EXPECT_EQ(ErrorTraits<FileErr>::key(e), "FILE_ACCESS_DENIED");
}

// ============================================================================
// TESTES DE LIMITES (Bounds)
// ============================================================================

TEST(FileErrorsTest, InvalidEnumValues) {
    // Valor fora do range
    auto invalid_err = static_cast<FileErr>(9999);

    // Deve ser seguro (Trace e strings vazias)
    EXPECT_EQ(ErrorTraits<FileErr>::default_severity(invalid_err), Severity::Trace);
    EXPECT_TRUE(ErrorTraits<FileErr>::key(invalid_err).empty());
    EXPECT_TRUE(ErrorTraits<FileErr>::ptBR(invalid_err).empty());
}