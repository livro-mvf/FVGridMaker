// ============================================================================
// Arquivo: tst_ErrorRecord.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Testa o componente ErrorRecord no contexto de ErrorHandling.
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
#include <string_view>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/ErrorRecord.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

    
TEST(ErrorRecord, StoresCodeMessageCategoryContextAndSource) {
    const ErrorRecord record{
        .code = "FVGRID.TEST.ERROR",
        .message = "test error message",
        .category = "Test",
        .context = {
            make_error_context("nvol", "0"),
            make_error_context("expected", "> 0"),
        },
        .source = ID{
            "ErrorHandling",
            "ErrorRecordTest",
            "fvgrid.test.ErrorRecordTest"
        },
    };

    EXPECT_EQ(record.code, std::string_view{"FVGRID.TEST.ERROR"});
    EXPECT_EQ(record.message, "test error message");
    EXPECT_EQ(record.category, std::string_view{"Test"});

    ASSERT_EQ(record.context.size(), 2U);
    EXPECT_EQ(record.context[0].key, "nvol");
    EXPECT_EQ(record.context[0].value, "0");
    EXPECT_EQ(record.context[1].key, "expected");
    EXPECT_EQ(record.context[1].value, "> 0");

    EXPECT_EQ(record.source.module(), std::string_view{"ErrorHandling"});
    EXPECT_EQ(record.source.class_name(), std::string_view{"ErrorRecordTest"});
    EXPECT_EQ(
        record.source.class_id(),
        std::string_view{"fvgrid.test.ErrorRecordTest"}
    );
}

TEST(ErrorRecord, StoresExplicitSourceLocation) {
    constexpr auto location = std::source_location::current();

    const ErrorRecord record{
        .code = "FVGRID.TEST.LOCATION",
        .message = "location test",
        .category = "Test",
        .context = {},
        .source = ID{
            "ErrorHandling",
            "ErrorRecordTest",
            "fvgrid.test.ErrorRecordTest"
        },
        .location = location,
    };

    EXPECT_EQ(record.location.line(), location.line());
    EXPECT_STREQ(record.location.file_name(), location.file_name());
}

}  // namespace fvgrid
