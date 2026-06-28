// ============================================================================
// Arquivo: tst_ConstantWeight1D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Testa o componente ConstantWeight1D no contexto de GridPattern1D.
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
#include <limits>
#include <string_view>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/FVGridException.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/ConstantWeight1D.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {
namespace {

void expect_invalid_weight(Real value) {
    try {
        [[maybe_unused]] const ConstantWeight1D weight{value};
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.CORE.INVALID_ARGUMENT"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Core"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"ConstantWeight1D"}
        );
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.grid_pattern.ConstantWeight1D"}
        );
        return;
    }

    FAIL() << "ConstantWeight1D accepted an invalid weight.";
}

}  // namespace

TEST(ConstantWeight1D, MetadataIsStable) {
    EXPECT_EQ(
        ConstantWeight1D::class_name(),
        std::string_view{"ConstantWeight1D"}
    );
    EXPECT_EQ(
        ConstantWeight1D::class_id(),
        std::string_view{"fvgrid.1d.grid_pattern.ConstantWeight1D"}
    );
}

TEST(ConstantWeight1D, StoresWeightValue) {
    const ConstantWeight1D weight{0.25};

    EXPECT_DOUBLE_EQ(weight.value(), 0.25);
}

TEST(ConstantWeight1D, ReturnsSameWeightForAnyIndex) {
    const ConstantWeight1D weight{0.375};

    EXPECT_DOUBLE_EQ(weight(static_cast<Size>(0)), 0.375);
    EXPECT_DOUBLE_EQ(weight(static_cast<Size>(1)), 0.375);
    EXPECT_DOUBLE_EQ(weight(static_cast<Size>(100)), 0.375);
}

TEST(ConstantWeight1D, AcceptsOpenUnitInterval) {
    EXPECT_NO_THROW([[maybe_unused]] const ConstantWeight1D lower{1.0e-12});
    EXPECT_NO_THROW([[maybe_unused]] const ConstantWeight1D middle{0.5});
    EXPECT_NO_THROW([[maybe_unused]] const ConstantWeight1D upper{1.0 - 1.0e-12});
}

TEST(ConstantWeight1D, RejectsZero) {
    expect_invalid_weight(0.0);
}

TEST(ConstantWeight1D, RejectsOne) {
    expect_invalid_weight(1.0);
}

TEST(ConstantWeight1D, RejectsNegativeValue) {
    expect_invalid_weight(-0.25);
}

TEST(ConstantWeight1D, RejectsValueGreaterThanOne) {
    expect_invalid_weight(1.25);
}

TEST(ConstantWeight1D, RejectsInfinity) {
    expect_invalid_weight(std::numeric_limits<Real>::infinity());
}

TEST(ConstantWeight1D, RejectsNaN) {
    expect_invalid_weight(std::numeric_limits<Real>::quiet_NaN());
}

}  // namespace fvgrid