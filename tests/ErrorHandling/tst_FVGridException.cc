// ============================================================================
// Arquivo: tst_FVGridException.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Testa o componente FVGridException no contexto de ErrorHandling.
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
#include <source_location>
#include <string>
#include <string_view>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/FVGridException.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

namespace {

[[nodiscard]] constexpr ID test_id() noexcept {
    return ID{
        "ErrorHandling",
        "FVGridExceptionTest",
        "fvgrid.test.FVGridExceptionTest"
    };
}

}  // namespace

TEST(FVGridException, StoresErrorRecord) {
    const ErrorRecord record{
        .code = "FVGRID.TEST.EXCEPTION",
        .message = "exception test message",
        .category = "Test",
        .context = {
            make_error_context("key", "value"),
        },
        .source = test_id(),
    };

    const FVGridException exception{record};

    EXPECT_EQ(exception.record().code, record.code);
    EXPECT_EQ(exception.record().message, record.message);
    EXPECT_EQ(exception.record().category, record.category);
    ASSERT_EQ(exception.record().context.size(), 1U);
    EXPECT_EQ(exception.record().context[0].key, "key");
    EXPECT_EQ(exception.record().context[0].value, "value");
    EXPECT_EQ(exception.record().source.module(), test_id().module());
    EXPECT_EQ(exception.record().source.class_name(), test_id().class_name());
    EXPECT_EQ(exception.record().source.class_id(), test_id().class_id());
}

TEST(FVGridException, WhatContainsStructuredInformation) {
    constexpr auto location = std::source_location::current();

    const FVGridException exception{
        ErrorRecord{
            .code = "FVGRID.TEST.FORMAT",
            .message = "format test message",
            .category = "Test",
            .context = {
                make_error_context("nvol", "0"),
                make_error_context("expected", "> 0"),
            },
            .source = test_id(),
            .location = location,
        }
    };

    const std::string message{exception.what()};

    
    EXPECT_NE(message.find("[FVGRID.TEST.FORMAT]"), std::string::npos);
    EXPECT_NE(message.find("format test message"), std::string::npos);
    EXPECT_NE(message.find("category: Test"), std::string::npos);
    EXPECT_NE(message.find("module: ErrorHandling"), std::string::npos);
    EXPECT_NE(message.find("class: FVGridExceptionTest"), std::string::npos);
    EXPECT_NE(
        message.find("class id: fvgrid.test.FVGridExceptionTest"),
        std::string::npos
    );
    EXPECT_NE(message.find("context: nvol=0, expected=> 0"), std::string::npos);
    EXPECT_NE(message.find(location.file_name()), std::string::npos);
}

TEST(FVGridException, WhatIsStableCString) {
    const FVGridException exception{
        ErrorRecord{
            .code = "FVGRID.TEST.CSTRING",
            .message = "cstring test",
            .category = "Test",
            .context = {},
            .source = test_id(),
        }
    };

    const char* first = exception.what();
    const char* second = exception.what();

    EXPECT_NE(first, nullptr);
    EXPECT_EQ(first, second);
}

}  // namespace fvgrid
