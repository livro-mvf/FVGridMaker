// ============================================================================
// Arquivo: tst_Roberts1D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Testa o componente Roberts1D no contexto de Distribution1D.
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
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/FVGridException.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Roberts1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/FaceCentered1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

namespace {

[[nodiscard]] bool near(
    Real a,
    Real b,
    Real tolerance = 1.0e-12
) {
    return std::abs(a - b) <= tolerance;
}

}  // namespace

TEST(Roberts1D, BuildsDefaultFaceCenteredAxis) {
    const Axis1D axis = Roberts1D::make(
        NVol{8},
        Length{1.0},
        XInit{0.0},
        Beta{1.5}
    );

    EXPECT_EQ(axis.num_cells(), static_cast<Size>(8));
    EXPECT_EQ(axis.num_faces(), static_cast<Size>(9));
    EXPECT_EQ(axis.pattern_name(), FaceCentered1D::name());
}

TEST(Roberts1D, DefaultPatternUsesDomainBounds) {
    const Axis1D axis = Roberts1D::make(
        NVol{8},
        Length{2.0},
        XInit{-1.0},
        Beta{1.5}
    );

    EXPECT_NEAR(axis.faces().front(), -1.0, 1.0e-12);
    EXPECT_NEAR(axis.faces().back(), 1.0, 1.0e-12);
    EXPECT_NEAR(axis.length(), 2.0, 1.0e-12);
}

TEST(Roberts1D, VolumeCenteredFacesAreStrictlyIncreasing) {
    const Axis1D axis = Roberts1D::make(
        NVol{32},
        Length{1.0},
        XInit{0.0},
        Beta{1.2},
        VolumeCentered1D{}
    );

    for (Size i = 1; i < axis.faces().size(); ++i) {
        EXPECT_GT(axis.faces()[i], axis.faces()[i - 1]);
    }
}

TEST(Roberts1D, VolumeCenteredFacesAreSymmetric) {
    const Axis1D axis = Roberts1D::make(
        NVol{20},
        Length{1.0},
        XInit{0.0},
        Beta{2.0},
        VolumeCentered1D{}
    );

    const Size last = axis.faces().size() - 1;

    for (Size i = 0; i < axis.faces().size(); ++i) {
        EXPECT_NEAR(axis.faces()[i], 1.0 - axis.faces()[last - i], 1.0e-12);
    }
}

TEST(Roberts1D, VolumeCenteredCentersComeFromFaces) {
    const Axis1D axis = Roberts1D::make(
        NVol{12},
        Length{1.0},
        XInit{0.0},
        Beta{1.8},
        VolumeCentered1D{}
    );

    for (Size i = 0; i < axis.centers().size(); ++i) {
        const Real expected =
            static_cast<Real>(0.5) * (axis.faces()[i] + axis.faces()[i + 1]);

        EXPECT_NEAR(axis.centers()[i], expected, 1.0e-12);
    }
}

TEST(Roberts1D, SupportsExplicitFaceCenteredPattern) {
    const Axis1D axis = Roberts1D::make(
        NVol{12},
        Length{1.0},
        XInit{0.0},
        Beta{1.5},
        FaceCentered1D{}
    );

    EXPECT_EQ(axis.num_cells(), static_cast<Size>(12));
    EXPECT_EQ(axis.num_faces(), static_cast<Size>(13));
    EXPECT_EQ(axis.pattern_name(), FaceCentered1D::name());
    EXPECT_DOUBLE_EQ(axis.faces().front(), 0.0);
    EXPECT_DOUBLE_EQ(axis.faces().back(), 1.0);
}

TEST(Roberts1D, FaceCenteredCentersAreStrictlyIncreasingAndInterior) {
    const Axis1D axis = Roberts1D::make(
        NVol{16},
        Length{1.0},
        XInit{0.0},
        Beta{1.3},
        FaceCentered1D{}
    );

    EXPECT_GT(axis.centers().front(), axis.faces().front());
    EXPECT_LT(axis.centers().back(), axis.faces().back());

    for (Size i = 1; i < axis.centers().size(); ++i) {
        EXPECT_GT(axis.centers()[i], axis.centers()[i - 1]);
    }
}

TEST(Roberts1D, FaceCenteredCentersAreSymmetric) {
    const Axis1D axis = Roberts1D::make(
        NVol{18},
        Length{1.0},
        XInit{0.0},
        Beta{2.0},
        FaceCentered1D{}
    );

    const Size last = axis.centers().size() - 1;

    for (Size i = 0; i < axis.centers().size(); ++i) {
        EXPECT_NEAR(
            axis.centers()[i],
            1.0 - axis.centers()[last - i],
            1.0e-12
        );
    }
}

TEST(Roberts1D, FreeFunctionUsesDefaultPattern) {
    const Axis1D axis = roberts_axis_1d(
        NVol{8},
        Length{1.0},
        XInit{0.0},
        Beta{1.5}
    );

    EXPECT_EQ(axis.pattern_name(), FaceCentered1D::name());
    EXPECT_EQ(axis.num_cells(), static_cast<Size>(8));
}

TEST(Roberts1D, FreeFunctionSupportsFaceCenteredPattern) {
    const Axis1D axis = roberts_axis_1d(
        NVol{8},
        Length{1.0},
        XInit{0.0},
        Beta{1.5},
        FaceCentered1D{}
    );

    EXPECT_EQ(axis.pattern_name(), FaceCentered1D::name());
    EXPECT_EQ(axis.num_cells(), static_cast<Size>(8));
}

TEST(Roberts1D, RejectsInvalidBeta) {
    EXPECT_THROW(
        (void)Roberts1D::make(
            NVol{8},
            Length{1.0},
            XInit{0.0},
            Beta{1.0}
        ),
        FVGridException
    );
}

TEST(Roberts1D, RejectsInvalidNVol) {
    EXPECT_THROW(
        (void)Roberts1D::make(
            NVol{0},
            Length{1.0},
            XInit{0.0},
            Beta{1.5}
        ),
        FVGridException
    );
}

TEST(Roberts1D, RejectsInvalidLength) {
    EXPECT_THROW(
        (void)Roberts1D::make(
            NVol{8},
            Length{0.0},
            XInit{0.0},
            Beta{1.5}
        ),
        FVGridException
    );
}

}  // namespace fvgrid
