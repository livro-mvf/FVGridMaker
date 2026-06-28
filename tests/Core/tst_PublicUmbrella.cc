// ============================================================================
// Arquivo: tst_PublicUmbrella.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Testa o componente PublicUmbrella no contexto de Core.
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
#include <FVGridMaker/FVGridMaker.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

TEST(PublicUmbrella, ExposesMainOneAndTwoDimensionalAPI) {
    static_assert(std::is_class_v<Axis1D>);
    static_assert(std::is_class_v<Uniform1D>);
    static_assert(std::is_class_v<StructuredGrid2D>);
    static_assert(CoordinateMapping2D<CartesianCoordinates2D>);

    EXPECT_NE(version(), nullptr);
    EXPECT_STREQ(version(), version_string().data());
    EXPECT_EQ(version_string(), std::string_view{"0.2.0"});
}

}  // namespace fvgrid