// ============================================================================
// Arquivo: tst_Axis1DInvariants.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Testa o componente Axis1DInvariants no contexto de Axis1D.
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
#include <limits>
#include <type_traits>
#include <vector>

#include <gtest/gtest.h>

#include <FVGridMaker/ErrorHandling/FVGridException.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>

namespace fvgrid {

TEST(Axis1DInvariants, CannotBeDefaultConstructedInvalid) {
    static_assert(!std::is_default_constructible_v<Axis1D>);
    SUCCEED();
}

TEST(Axis1DInvariants, RejectsNaNCoordinate) {
    EXPECT_THROW(
        Axis1D(std::vector<Real>{0.0,std::numeric_limits<Real>::quiet_NaN(),1.0}),
        FVGridException);
}

TEST(Axis1DInvariants, RejectsInfiniteCoordinate) {
    EXPECT_THROW(
        Axis1D(std::vector<Real>{0.0,std::numeric_limits<Real>::infinity()}),
        FVGridException);
}

TEST(Axis1DInvariants, RejectsNonFiniteExplicitCenter) {
    EXPECT_THROW(
        Axis1D(std::vector<Real>{0.0,1.0},
               std::vector<Real>{std::numeric_limits<Real>::infinity()},
               "Custom"),
        FVGridException);
}

}  // namespace fvgrid

