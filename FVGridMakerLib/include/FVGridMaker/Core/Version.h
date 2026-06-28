// ============================================================================
// Arquivo: Version.h
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Declara funções e constantes de versão pública do FVGridMaker.
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
#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <string_view>

// ----------------------------------------------------------------------------
// FVGridMaker generated includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/version.hpp>

namespace fvgrid {

inline constexpr int kVersionMajor =
    static_cast<int>(build_info::VersionInfo::kMajor);

inline constexpr int kVersionMinor =
    static_cast<int>(build_info::VersionInfo::kMinor);

inline constexpr int kVersionPatch =
    static_cast<int>(build_info::VersionInfo::kPatch);

inline constexpr std::string_view kVersionString =
    build_info::VersionInfo::kSemanticVersion;

inline constexpr std::string_view kFullVersionString =
    build_info::VersionInfo::kFullVersion;

inline constexpr std::string_view kGitHash =
    build_info::VersionInfo::kGitHash;

inline constexpr std::string_view kGitBranch =
    build_info::VersionInfo::kGitBranch;

inline constexpr std::string_view kBuildTimestamp =
    build_info::VersionInfo::kBuildTimestamp;

inline constexpr std::string_view kBuildDate =
    build_info::VersionInfo::kBuildDate;

[[nodiscard]] constexpr int version_major() noexcept {
    return kVersionMajor;
}

[[nodiscard]] constexpr int version_minor() noexcept {
    return kVersionMinor;
}

[[nodiscard]] constexpr int version_patch() noexcept {
    return kVersionPatch;
}

[[nodiscard]] constexpr std::string_view version_string() noexcept {
    return kVersionString;
}

[[nodiscard]] constexpr std::string_view full_version_string() noexcept {
    return kFullVersionString;
}

[[nodiscard]] constexpr std::string_view git_hash() noexcept {
    return kGitHash;
}

[[nodiscard]] constexpr std::string_view git_branch() noexcept {
    return kGitBranch;
}

[[nodiscard]] constexpr std::string_view build_timestamp() noexcept {
    return kBuildTimestamp;
}

[[nodiscard]] constexpr std::string_view build_date() noexcept {
    return kBuildDate;
}

[[nodiscard]] constexpr bool is_development_build() noexcept {
    return build_info::VersionInfo::is_development_build();
}

[[nodiscard]] constexpr bool is_tagged_release() noexcept {
    return build_info::VersionInfo::is_tagged_release();
}

}  // namespace fvgrid

