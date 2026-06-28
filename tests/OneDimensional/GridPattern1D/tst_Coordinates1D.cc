// ============================================================================
// Arquivo: tst_Coordinates1D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Testa o componente Coordinates1D no contexto de GridPattern1D.
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
#include <utility>
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/OneDimensional/GridPattern1D/Coordinates1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/CoordinateTags1D.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

TEST(Coordinates1D, StoresFaceCoordinates) {
    Coordinates1D coordinates = Coordinates1D::faces(
        std::vector<Real>{0.0, 0.2, 0.7, 1.0}
    );

    EXPECT_TRUE(coordinates.has_tag<FaceCoordinates1D>());
    EXPECT_FALSE(coordinates.has_tag<CenterCoordinates1D>());

    ASSERT_EQ(coordinates.values().size(), static_cast<Size>(4));
    EXPECT_DOUBLE_EQ(coordinates.values()[0], 0.0);
    EXPECT_DOUBLE_EQ(coordinates.values()[1], 0.2);
    EXPECT_DOUBLE_EQ(coordinates.values()[2], 0.7);
    EXPECT_DOUBLE_EQ(coordinates.values()[3], 1.0);
}

TEST(Coordinates1D, StoresCentreCoordinates) {
    Coordinates1D coordinates = Coordinates1D::centers(
        std::vector<Real>{0.1, 0.45, 0.85}
    );

    EXPECT_TRUE(coordinates.has_tag<CenterCoordinates1D>());
    EXPECT_FALSE(coordinates.has_tag<FaceCoordinates1D>());

    ASSERT_EQ(coordinates.values().size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(coordinates.values()[0], 0.1);
    EXPECT_DOUBLE_EQ(coordinates.values()[1], 0.45);
    EXPECT_DOUBLE_EQ(coordinates.values()[2], 0.85);
}

TEST(Coordinates1D, ReleasesStoredValues) {
    Coordinates1D coordinates = Coordinates1D::faces(
        std::vector<Real>{0.0, 0.5, 1.0}
    );

    std::vector<Real> values = std::move(coordinates).release_values();

    ASSERT_EQ(values.size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(values[0], 0.0);
    EXPECT_DOUBLE_EQ(values[1], 0.5);
    EXPECT_DOUBLE_EQ(values[2], 1.0);
}

}  // namespace fvgrid