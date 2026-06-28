// ============================================================================
// Arquivo: tst_Types.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Testa o componente Types no contexto de Core.
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
#include <cstddef>
#include <cstdint>
#include <type_traits>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/Types.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

TEST(Types, RealIsDouble) {
    static_assert(std::is_same_v<Real, double>);
    SUCCEED();
}

TEST(Types, IndexIsSignedPointerSizedInteger) {
    static_assert(std::is_same_v<Index, std::ptrdiff_t>);
    static_assert(std::is_signed_v<Index>);
    SUCCEED();
}

TEST(Types, SizeIsUnsignedSizeType) {
    static_assert(std::is_same_v<Size, std::size_t>);
    static_assert(std::is_unsigned_v<Size>);
    SUCCEED();
}

TEST(Types, SignedIntegerAliasesMatchStdTypes) {
    static_assert(std::is_same_v<Int8, std::int8_t>);
    static_assert(std::is_same_v<Int16, std::int16_t>);
    static_assert(std::is_same_v<Int32, std::int32_t>);
    static_assert(std::is_same_v<Int64, std::int64_t>);
    SUCCEED();
}

TEST(Types, UnsignedIntegerAliasesMatchStdTypes) {
    static_assert(std::is_same_v<UInt8, std::uint8_t>);
    static_assert(std::is_same_v<UInt16, std::uint16_t>);
    static_assert(std::is_same_v<UInt32, std::uint32_t>);
    static_assert(std::is_same_v<UInt64, std::uint64_t>);
    SUCCEED();
}

}  // namespace fvgrid
