// ============================================================================
// Arquivo: tst_ID.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Testa o componente ID no contexto de Core.
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
#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/ID.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

TEST(ID, DefaultIdentityIsUnknown) {
    constexpr ID id;

    EXPECT_EQ(id.module(), std::string_view{"Unknown"});
    EXPECT_EQ(id.class_name(), std::string_view{"Unknown"});
    EXPECT_EQ(id.class_id(), std::string_view{"fvgrid.unknown.Unknown"});
}

TEST(ID, StoresModuleClassNameAndClassId) {
    constexpr ID id{
        "OneDimensional",
        "Axis1D",
        "fvgrid.1d.axis.Axis1D"
    };

    EXPECT_EQ(id.module(), std::string_view{"OneDimensional"});
    EXPECT_EQ(id.class_name(), std::string_view{"Axis1D"});
    EXPECT_EQ(id.class_id(), std::string_view{"fvgrid.1d.axis.Axis1D"});
}

TEST(ID, IsFinalAndNotPolymorphic) {
    static_assert(std::is_final_v<ID>);
    static_assert(!std::is_polymorphic_v<ID>);

    SUCCEED();
}

TEST(ID, IsLightweightValueType) {
    static_assert(std::is_copy_constructible_v<ID>);
    static_assert(std::is_copy_assignable_v<ID>);
    static_assert(std::is_move_constructible_v<ID>);
    static_assert(std::is_move_assignable_v<ID>);
    static_assert(std::is_nothrow_copy_constructible_v<ID>);
    static_assert(std::is_nothrow_move_constructible_v<ID>);

    SUCCEED();
}

TEST(ID, IsCopyConstructible) {
    constexpr ID original{
        "Core",
        "ID",
        "fvgrid.core.ID"
    };

    constexpr ID copy{original};

    EXPECT_EQ(copy.module(), original.module());
    EXPECT_EQ(copy.class_name(), original.class_name());
    EXPECT_EQ(copy.class_id(), original.class_id());
}

TEST(ID, IsMoveConstructible) {
    ID original{
        "Core",
        "ID",
        "fvgrid.core.ID"
    };

    const ID moved{std::move(original)};

    EXPECT_EQ(moved.module(), std::string_view{"Core"});
    EXPECT_EQ(moved.class_name(), std::string_view{"ID"});
    EXPECT_EQ(moved.class_id(), std::string_view{"fvgrid.core.ID"});
}

}  // namespace fvgrid
