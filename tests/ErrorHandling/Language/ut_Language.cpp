// ----------------------------------------------------------------------------
// File: ut_Language.cpp
// Project: FVGridMaker
// Version: 1.0
// Description: Testes unitários para o enum Language.
//              Garante a existência dos enumeradores e propriedades de tipo.
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
#include <FVGridMaker/ErrorHandling/Language.h>

// Namespaces
namespace Err = FVGridMaker::error;
using namespace FVGridMaker::error;

/**
 * @file ut_Language.cpp
 * @brief Validação do Enum Language.
 * @ingroup ErrorHandlingTests
 */

// ============================================================================
// TESTES DE TIPO
// ============================================================================

/**
 * @test EnumProperties
 * @brief Verifica se Language é um enum class (escopado).
 */
TEST(LanguageTest, EnumProperties) {
    EXPECT_TRUE(std::is_enum_v<Language>);
    
    // Verifica se não é convertível implicitamente para int (característica de enum class)
    EXPECT_FALSE((std::is_convertible_v<Language, int>));
}

// ============================================================================
// TESTES DE VALORES
// ============================================================================

/**
 * @test EnumeratorsExist
 * @brief Garante que os idiomas suportados estão definidos.
 * Útil para detectar quebras de API se alguém remover um idioma.
 */
TEST(LanguageTest, EnumeratorsExist) {
    // Apenas verifica se compila e se os valores são distintos
    Language en = Language::EnUS;
    Language pt = Language::PtBR;

    EXPECT_NE(en, pt);
}

/**
 * @test SwitchCoverage
 * @brief Simula um switch-case para garantir que o compilador lida bem com os valores.
 */
TEST(LanguageTest, SwitchCoverage) {
    Language lang = Language::PtBR;
    bool visited = false;

    switch (lang) {
        case Language::PtBR:
            visited = true;
            break;
        case Language::EnUS:
            visited = false;
            break;
    }

    EXPECT_TRUE(visited);
}