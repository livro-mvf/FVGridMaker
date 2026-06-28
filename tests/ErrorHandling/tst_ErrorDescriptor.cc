// ============================================================================
// Arquivo: tst_ErrorDescriptor.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Testa o componente ErrorDescriptor no contexto de ErrorHandling.
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

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/ErrorDescriptor.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

    
TEST(ErrorDescriptor, StoresCodeMessageAndCategory) {
    constexpr ErrorDescriptor descriptor{
        .code = "FVGRID.TEST.ERROR",
        .message = "Test error message.",
        .category = "Test",
    };

    EXPECT_EQ(descriptor.code, std::string_view{"FVGRID.TEST.ERROR"});
    EXPECT_EQ(descriptor.message, std::string_view{"Test error message."});
    EXPECT_EQ(descriptor.category, std::string_view{"Test"});
}

TEST(ErrorDescriptor, IsFinalAndNotPolymorphic) {
    static_assert(std::is_final_v<ErrorDescriptor>);
    static_assert(!std::is_polymorphic_v<ErrorDescriptor>);

    SUCCEED();
}

TEST(ErrorDescriptor, IsLightweightValueType) {
    static_assert(std::is_copy_constructible_v<ErrorDescriptor>);
    static_assert(std::is_copy_assignable_v<ErrorDescriptor>);
    static_assert(std::is_move_constructible_v<ErrorDescriptor>);
    static_assert(std::is_move_assignable_v<ErrorDescriptor>);

    SUCCEED();
}

}  // namespace fvgrid
