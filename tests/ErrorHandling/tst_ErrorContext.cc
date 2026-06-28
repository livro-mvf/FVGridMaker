// ============================================================================
// Arquivo: tst_ErrorContext.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Testa o componente ErrorContext no contexto de ErrorHandling.
// Autor: João Flávio Vieira de Vasconcellos
//
// SPDX-FileCopyrightText: 2026 João Flávio Vieira de Vasconcellos
// SPDX-License-Identifier: BSD-3-Clause
//
// Este arquivo faz parte do FVGridMaker.
//
// Licença: BSD 3-Clause.
// É permitido usar, copiar, modificar e redistribuir este arquivo, em código-fonte
// ou forma binária, com ou sem modificações, desde que sejam preservados os avisos
// de copyright, esta identificação de licença e as condições descritas no arquivo
// LICENSE.
//
// O nome do autor, de colaboradores ou de instituições associadas ao projeto não
// pode ser usado para endossar ou promover produtos derivados sem autorização
// prévia por escrito.
//
// Este software é fornecido sem garantias de qualquer natureza. Consulte o arquivo
// LICENSE, na raiz do repositório, para o texto completo da licença.
// ============================================================================
// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <string>
#include <string_view>
#include <type_traits>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/ErrorContext.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>


namespace fvgrid {

TEST(ErrorContext, StoresKeyAndValue) {
    const ErrorContext context{
        .key = "nvol",
        .value = "0",
    };

    EXPECT_EQ(context.key, std::string{"nvol"});
    EXPECT_EQ(context.value, std::string{"0"});
}

TEST(ErrorContext, MakeErrorContextCopiesStringViews) {
    const std::string key{"length"};
    const std::string value{"-1.0"};

    const ErrorContext context = make_error_context(key, value);

    EXPECT_EQ(context.key, std::string{"length"});
    EXPECT_EQ(context.value, std::string{"-1.0"});
}

TEST(ErrorContext, ErrorContextListIsVectorOfErrorContext) {
    static_assert(std::is_same_v<ErrorContextList, std::vector<ErrorContext>>);

    ErrorContextList context;
    context.push_back(make_error_context("beta", "1.0"));
    context.push_back(make_error_context("expected", "> 1"));

    ASSERT_EQ(context.size(), 2U);
    EXPECT_EQ(context[0].key, std::string{"beta"});
    EXPECT_EQ(context[0].value, std::string{"1.0"});
    EXPECT_EQ(context[1].key, std::string{"expected"});
    EXPECT_EQ(context[1].value, std::string{"> 1"});
}

}  // namespace fvgrid
