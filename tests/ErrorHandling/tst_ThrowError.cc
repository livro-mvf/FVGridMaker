// ============================================================================
// Arquivo: tst_ThrowError.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Testa o componente ThrowError no contexto de ErrorHandling.
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
#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/ErrorHandling/BuiltInErrors.h>
#include <FVGridMaker/ErrorHandling/FVGridException.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

namespace {

[[nodiscard]] constexpr ID test_id() noexcept {
    return ID{
        "ErrorHandling",
        "ThrowErrorTest",
        "fvgrid.test.ThrowErrorTest"
    };
}

struct UserDefinedError final {
    static constexpr std::string_view code =
        "USER.TEST.ERROR";

    static constexpr std::string_view message =
        "User-defined test error.";

    static constexpr std::string_view category =
        "User";
};

}  // namespace

TEST(ThrowError, RuntimeThrowErrorThrowsFVGridException) {
    EXPECT_THROW(
        throw_error(
            "FVGRID.TEST.THROW",
            "throw test message",
            "Test",
            test_id()
        ),
        FVGridException
    );
}

TEST(ThrowError, RuntimeThrowErrorStoresRecordFields) {
    try {
        throw_error(
            "FVGRID.TEST.RECORD",
            "record test message",
            "Test",
            test_id()
        );
    } catch (const FVGridException& exception) {
        EXPECT_EQ(exception.record().code, std::string_view{"FVGRID.TEST.RECORD"});
        EXPECT_EQ(exception.record().message, "record test message");
        EXPECT_EQ(exception.record().category, std::string_view{"Test"});
        EXPECT_TRUE(exception.record().context.empty());
        EXPECT_EQ(exception.record().source.module(), std::string_view{"ErrorHandling"});
        EXPECT_EQ(exception.record().source.class_name(), std::string_view{"ThrowErrorTest"});
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.test.ThrowErrorTest"}
        );
        return;
    }

    FAIL() << "throw_error did not throw FVGridException.";
}

TEST(ThrowError, RuntimeThrowErrorStoresContext) {
    try {
        throw_error(
            "FVGRID.TEST.CONTEXT",
            "context test message",
            "Test",
            test_id(),
            {
                make_error_context("nvol", "0"),
                make_error_context("expected", "> 0"),
            }
        );
    } catch (const FVGridException& exception) {
        ASSERT_EQ(exception.record().context.size(), 2U);
        EXPECT_EQ(exception.record().context[0].key, "nvol");
        EXPECT_EQ(exception.record().context[0].value, "0");
        EXPECT_EQ(exception.record().context[1].key, "expected");
        EXPECT_EQ(exception.record().context[1].value, "> 0");
        return;
    }

    FAIL() << "throw_error did not throw FVGridException.";
}

TEST(ThrowError, RuntimeThrowErrorStoresExplicitSourceLocation) {
    constexpr auto location = std::source_location::current();

    try {
        throw_error(
            "FVGRID.TEST.LOCATION",
            "location test message",
            "Test",
            test_id(),
            location
        );
    } catch (const FVGridException& exception) {
        EXPECT_EQ(exception.record().location.line(), location.line());
        EXPECT_STREQ(exception.record().location.file_name(), location.file_name());
        return;
    }

    FAIL() << "throw_error did not throw FVGridException.";
}


TEST(ThrowError, TypedThrowErrorStoresBuiltInDescriptor) {
    try {
        throw_error<errors::grid::InvalidNVol>(test_id());
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.GRID.INVALID_NVOL"}
        );
        EXPECT_EQ(
            exception.record().message,
            "The number of control volumes must be positive."
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Grid"});
        return;
    }

    FAIL() << "typed throw_error did not throw FVGridException.";
}

TEST(ThrowError, TypedThrowErrorStoresUserDefinedDescriptor) {
    try {
        throw_error<UserDefinedError>(test_id());
    } catch (const FVGridException& exception) {
        EXPECT_EQ(exception.record().code, std::string_view{"USER.TEST.ERROR"});
        EXPECT_EQ(exception.record().message, "User-defined test error.");
        EXPECT_EQ(exception.record().category, std::string_view{"User"});
        return;
    }

    FAIL() << "typed throw_error did not throw FVGridException.";
}

TEST(ThrowError, TypedThrowErrorStoresContext) {
    try {
        throw_error<errors::grid::InvalidNVol>(
            test_id(),
            {
                make_error_context("nvol", "0"),
            }
        );
    } catch (const FVGridException& exception) {
        ASSERT_EQ(exception.record().context.size(), 1U);
        EXPECT_EQ(exception.record().context[0].key, "nvol");
        EXPECT_EQ(exception.record().context[0].value, "0");
        return;
    }

    FAIL() << "typed throw_error did not throw FVGridException.";
}

TEST(Require, RuntimeRequireDoesNothingWhenConditionIsTrue) {
    EXPECT_NO_THROW(
        require(
            true,
            "FVGRID.TEST.REQUIRE",
            "require test message",
            "Test",
            test_id()
        )
    );
}

TEST(Require, RuntimeRequireThrowsWhenConditionIsFalse) {
    EXPECT_THROW(
        require(
            false,
            "FVGRID.TEST.REQUIRE",
            "require test message",
            "Test",
            test_id()
        ),
        FVGridException
    );
}

TEST(Require, RuntimeRequireStoresContextWhenConditionIsFalse) {
    try {
        require(
            false,
            "FVGRID.TEST.REQUIRE.CONTEXT",
            "require context message",
            "Test",
            test_id(),
            {
                make_error_context("condition", "false"),
            }
        );
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.TEST.REQUIRE.CONTEXT"}
        );
        ASSERT_EQ(exception.record().context.size(), 1U);
        EXPECT_EQ(exception.record().context[0].key, "condition");
        EXPECT_EQ(exception.record().context[0].value, "false");
        return;
    }

    FAIL() << "require did not throw FVGridException.";
}

TEST(Require, TypedRequireDoesNothingWhenConditionIsTrue) {
    EXPECT_NO_THROW(
        require<errors::grid::InvalidNVol>(
            true,
            test_id()
        )
    );
}

TEST(Require, TypedRequireThrowsWhenConditionIsFalse) {
    EXPECT_THROW(
        require<errors::grid::InvalidNVol>(
            false,
            test_id()
        ),
        FVGridException
    );
}

TEST(Require, TypedRequireStoresBuiltInDescriptorAndContext) {
    try {
        require<errors::grid::InvalidNVol>(
            false,
            test_id(),
            {
                make_error_context("nvol", "0"),
            }
        );
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.GRID.INVALID_NVOL"}
        );
        ASSERT_EQ(exception.record().context.size(), 1U);
        EXPECT_EQ(exception.record().context[0].key, "nvol");
        EXPECT_EQ(exception.record().context[0].value, "0");
        return;
    }

    FAIL() << "typed require did not throw FVGridException.";
}

}  // namespace fvgrid
