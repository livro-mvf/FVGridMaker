// ============================================================================
// Arquivo: tst_Version.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Testa o componente Version no contexto de Core.
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
#include <FVGridMaker/Core/Version.h>
#include <FVGridMaker/FVGridMaker.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

TEST(Version, VersionNumbersAreAvailable) {
    EXPECT_EQ(version_major(), 0);
    EXPECT_EQ(version_minor(), 2);
    EXPECT_EQ(version_patch(), 0);
}

TEST(Version, VersionStringIsAvailable) {
    EXPECT_EQ(version_string(), std::string_view{"0.2.0"});
}

TEST(Version, FullVersionStringIsAvailable) {
    EXPECT_FALSE(full_version_string().empty());
    EXPECT_TRUE(full_version_string().starts_with(version_string()));
}

TEST(Version, RootVersionFunctionMatchesVersionString) {
    EXPECT_STREQ(version(), version_string().data());
}

TEST(Version, VersionConstantsMatchAccessors) {
    EXPECT_EQ(kVersionMajor, version_major());
    EXPECT_EQ(kVersionMinor, version_minor());
    EXPECT_EQ(kVersionPatch, version_patch());
    EXPECT_EQ(kVersionString, version_string());
    EXPECT_EQ(kFullVersionString, full_version_string());
}

TEST(Version, GitMetadataAccessorsAreAvailable) {
    EXPECT_FALSE(git_hash().empty());
    EXPECT_FALSE(git_branch().empty());
}

TEST(Version, BuildMetadataAccessorsAreAvailable) {
    EXPECT_FALSE(build_timestamp().empty());
    EXPECT_FALSE(build_date().empty());
}

TEST(Version, BuildClassificationAccessorsAreCallable) {
    [[maybe_unused]] const bool development_build = is_development_build();
    [[maybe_unused]] const bool tagged_release = is_tagged_release();

    SUCCEED();
}

}  // namespace fvgrid
