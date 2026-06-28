// ============================================================================
// Arquivo: tst_StrongTypes.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Testa o componente StrongTypes no contexto de Core.
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
#include <type_traits>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/StrongTypes.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

TEST(StrongReal, StoresRealValue) {
    const StrongReal<Real, LengthTag> value{1.25};

    EXPECT_DOUBLE_EQ(value.value(), 1.25);
}

TEST(StrongInteger, StoresIntegerValue) {
    const StrongInteger<Size, NVolTag> value{42};

    EXPECT_EQ(value.value(), static_cast<Size>(42));
}

TEST(StrongTypes, WrappersAreNotPolymorphic) {
    static_assert(!std::is_polymorphic_v<StrongReal<Real, LengthTag>>);
    static_assert(!std::is_polymorphic_v<StrongInteger<Size, NVolTag>>);

    SUCCEED();
}

TEST(StrongTypes, WrappersAreFinal) {
    static_assert(std::is_final_v<StrongReal<Real, LengthTag>>);
    static_assert(std::is_final_v<StrongInteger<Size, NVolTag>>);

    SUCCEED();
}

TEST(StrongTypes, NVolUsesSize) {
    const NVol nvol{100};

    static_assert(std::is_same_v<NVol::value_type, Size>);
    static_assert(std::is_same_v<NVol::tag_type, NVolTag>);
    EXPECT_EQ(nvol.value(), static_cast<Size>(100));
}

TEST(StrongTypes, LengthUsesReal) {
    const Length length{2.5};

    static_assert(std::is_same_v<Length::value_type, Real>);
    static_assert(std::is_same_v<Length::tag_type, LengthTag>);
    EXPECT_DOUBLE_EQ(length.value(), 2.5);
}

TEST(StrongTypes, XInitUsesReal) {
    const XInit xinit{-1.0};

    static_assert(std::is_same_v<XInit::value_type, Real>);
    static_assert(std::is_same_v<XInit::tag_type, XInitTag>);
    EXPECT_DOUBLE_EQ(xinit.value(), -1.0);
}

TEST(StrongTypes, XFinalUsesReal) {
    const XFinal xfinal{3.0};

    static_assert(std::is_same_v<XFinal::value_type, Real>);
    static_assert(std::is_same_v<XFinal::tag_type, XFinalTag>);
    EXPECT_DOUBLE_EQ(xfinal.value(), 3.0);
}

TEST(StrongTypes, MinSpacingUsesReal) {
    const MinSpacing min_spacing{0.05};

    static_assert(std::is_same_v<MinSpacing::value_type, Real>);
    static_assert(std::is_same_v<MinSpacing::tag_type, MinSpacingTag>);
    EXPECT_DOUBLE_EQ(min_spacing.value(), 0.05);
}

TEST(StrongTypes, BetaUsesReal) {
    const Beta beta{1.5};

    static_assert(std::is_same_v<Beta::value_type, Real>);
    static_assert(std::is_same_v<Beta::tag_type, BetaTag>);
    EXPECT_DOUBLE_EQ(beta.value(), 1.5);
}

TEST(StrongTypes, SeedUsesUInt64) {
    const Seed seed{123456};

    static_assert(std::is_same_v<Seed::value_type, UInt64>);
    static_assert(std::is_same_v<Seed::tag_type, SeedTag>);
    EXPECT_EQ(seed.value(), static_cast<UInt64>(123456));
}

TEST(StrongTypes, DifferentStrongTypesAreDifferentTypes) {
    static_assert(!std::is_same_v<NVol, Seed>);
    static_assert(!std::is_same_v<Length, XInit>);
    static_assert(!std::is_same_v<XInit, XFinal>);
    static_assert(!std::is_same_v<Length, MinSpacing>);
    static_assert(!std::is_same_v<Length, Beta>);
    static_assert(!std::is_same_v<Beta, MinSpacing>);
    static_assert(!std::is_same_v<Length, StrongReal<Real, XInitTag>>);
    static_assert(!std::is_same_v<NVol, StrongInteger<Size, SeedTag>>);

    SUCCEED();
}

}  // namespace fvgrid
