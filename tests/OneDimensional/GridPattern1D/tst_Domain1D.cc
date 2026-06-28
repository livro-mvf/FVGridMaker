// ============================================================================
// Arquivo: tst_Domain1D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Testa o componente Domain1D no contexto de GridPattern1D.
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

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/FVGridException.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Domain1D.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

namespace {

void expect_invalid_argument_from_unbounded_domain(
    const FVGridException& exception
) {
    EXPECT_EQ(
        exception.record().code,
        std::string_view{"FVGRID.CORE.INVALID_ARGUMENT"}
    );
    EXPECT_EQ(exception.record().category, std::string_view{"Core"});
    EXPECT_EQ(
        exception.record().source.class_name(),
        std::string_view{"Domain1D"}
    );
    EXPECT_EQ(
        exception.record().source.class_id(),
        std::string_view{"fvgrid.1d.pattern.Domain1D"}
    );
}

}  // namespace

TEST(Domain1D, NoneHasNoBounds) {
    const Domain1D domain = Domain1D::none();

    EXPECT_FALSE(domain.has_bounds());

    try {
        [[maybe_unused]] const Real xmin = domain.xmin();
    } catch (const FVGridException& exception) {
        expect_invalid_argument_from_unbounded_domain(exception);
        return;
    }

    FAIL() << "Domain1D::none() did not reject xmin().";
}

TEST(Domain1D, NoneRejectsUpperBoundAccess) {
    const Domain1D domain = Domain1D::none();

    EXPECT_FALSE(domain.has_bounds());

    try {
        [[maybe_unused]] const Real xmax = domain.xmax();
    } catch (const FVGridException& exception) {
        expect_invalid_argument_from_unbounded_domain(exception);
        return;
    }

    FAIL() << "Domain1D::none() did not reject xmax().";
}

TEST(Domain1D, NoneRejectsLengthAccess) {
    const Domain1D domain = Domain1D::none();

    EXPECT_FALSE(domain.has_bounds());

    try {
        [[maybe_unused]] const Real length = domain.length();
    } catch (const FVGridException& exception) {
        expect_invalid_argument_from_unbounded_domain(exception);
        return;
    }

    FAIL() << "Domain1D::none() did not reject length().";
}

TEST(Domain1D, BuildsFromInitialCoordinateAndLength) {
    const Domain1D domain = Domain1D::from_length(
        XInit{2.0},
        Length{3.5}
    );

    EXPECT_TRUE(domain.has_bounds());
    EXPECT_DOUBLE_EQ(domain.xmin(), 2.0);
    EXPECT_DOUBLE_EQ(domain.xmax(), 5.5);
    EXPECT_DOUBLE_EQ(domain.length(), 3.5);
}

TEST(Domain1D, BuildsFromBounds) {
    const Domain1D domain = Domain1D::from_bounds(
        XInit{-1.0},
        XFinal{2.0}
    );

    EXPECT_TRUE(domain.has_bounds());
    EXPECT_DOUBLE_EQ(domain.xmin(), -1.0);
    EXPECT_DOUBLE_EQ(domain.xmax(), 2.0);
    EXPECT_DOUBLE_EQ(domain.length(), 3.0);
}

}  // namespace fvgrid