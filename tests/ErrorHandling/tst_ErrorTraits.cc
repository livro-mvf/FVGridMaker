// ============================================================================
// Arquivo: tst_ErrorTraits.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Testa o componente ErrorTraits no contexto de ErrorHandling.
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
#include <string_view>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/BuiltInErrors.h>
#include <FVGridMaker/ErrorHandling/ErrorTraits.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>


namespace fvgrid {

namespace {

struct ValidUserError final {
    static constexpr std::string_view code =
        "USER.TEST.ERROR";

    static constexpr std::string_view message =
        "User test error.";

    static constexpr std::string_view category =
        "User";
};

struct MissingCategoryError final {
    static constexpr std::string_view code =
        "USER.TEST.MISSING_CATEGORY";

    static constexpr std::string_view message =
        "Missing category.";
};

}  // namespace

TEST(ErrorTraits, RecognizesValidDescriptorProviders) {
    static_assert(ErrorDescriptorProvider<ValidUserError>);
    static_assert(ErrorDescriptorProvider<errors::grid::InvalidNVol>);
    static_assert(ErrorDescriptorProvider<errors::output::FileOpenFailed>);

    SUCCEED();
}

TEST(ErrorTraits, RejectsInvalidDescriptorProviders) {
    static_assert(!ErrorDescriptorProvider<MissingCategoryError>);
    static_assert(!ErrorDescriptorProvider<int>);

    SUCCEED();
}

TEST(ErrorTraits, DescriptorOfBuildsDescriptorFromProvider) {
    constexpr ErrorDescriptor descriptor = descriptor_of<ValidUserError>();

    EXPECT_EQ(descriptor.code, std::string_view{"USER.TEST.ERROR"});
    EXPECT_EQ(descriptor.message, std::string_view{"User test error."});
    EXPECT_EQ(descriptor.category, std::string_view{"User"});
}

TEST(ErrorTraits, DescriptorOfWorksForBuiltInErrors) {
    constexpr ErrorDescriptor descriptor =
        descriptor_of<errors::grid::InvalidNVol>();

    EXPECT_EQ(descriptor.code, std::string_view{"FVGRID.GRID.INVALID_NVOL"});
    EXPECT_EQ(
        descriptor.message,
        std::string_view{"The number of control volumes must be positive."}
    );
    EXPECT_EQ(descriptor.category, std::string_view{"Grid"});
}

}  // namespace fvgrid
