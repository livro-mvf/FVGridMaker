// ============================================================================
// Arquivo: tst_CoordinateTags1D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Testa o componente CoordinateTags1D no contexto de GridPattern1D.
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
#include <concepts>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/OneDimensional/GridPattern1D/CoordinateTags1D.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

TEST(CoordinateTags1D, DefinesFaceCoordinateTag) {
    EXPECT_TRUE((std::same_as<FaceCoordinates1D, FaceCoordinates1D>));
    EXPECT_FALSE((std::same_as<FaceCoordinates1D, CenterCoordinates1D>));
}

TEST(CoordinateTags1D, DefinesCenterCoordinateTag) {
    EXPECT_TRUE((std::same_as<CenterCoordinates1D, CenterCoordinates1D>));
    EXPECT_FALSE((std::same_as<CenterCoordinates1D, FaceCoordinates1D>));
}

TEST(CoordinateTags1D, AcceptsOnlyKnownPrimaryCoordinateTags) {
    EXPECT_TRUE((PrimaryCoordinateTag1D<FaceCoordinates1D>));
    EXPECT_TRUE((PrimaryCoordinateTag1D<CenterCoordinates1D>));
    EXPECT_FALSE((PrimaryCoordinateTag1D<int>));
}

}  // namespace fvgrid