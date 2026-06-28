// ============================================================================
// Arquivo: tst_Uniform1D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Testa o componente Uniform1D no contexto de Distribution1D.
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
#include <string_view>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/FVGridException.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/FaceCentered1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {
namespace {

template <class Pattern>
concept Uniform1DAcceptsPatternArgument =
    requires(Pattern pattern) {
        Uniform1D::make(
            NVol{4},
            Length{1.0},
            XInit{0.0},
            pattern
        );
    };

template <class Pattern>
concept UniformAxis1DFreeFunctionAcceptsPatternArgument =
    requires(Pattern pattern) {
        uniform_axis_1d(
            NVol{4},
            Length{1.0},
            XInit{0.0},
            pattern
        );
    };

}  // namespace

TEST(Uniform1D, BuildsUniformAxisWithDefaultFaceCenteredPattern) {
    const Axis1D axis = Uniform1D::make(
        NVol{4},
        Length{1.0},
        XInit{0.0}
    );

    EXPECT_EQ(axis.num_cells(), static_cast<Size>(4));
    EXPECT_EQ(axis.num_faces(), static_cast<Size>(5));
    EXPECT_EQ(axis.pattern_name(), FaceCentered1D::name());
}

TEST(Uniform1D, ComputesUniformFaces) {
    const Axis1D axis = Uniform1D::make(
        NVol{4},
        Length{1.0},
        XInit{0.0}
    );

    ASSERT_EQ(axis.faces().size(), static_cast<Size>(5));
    EXPECT_DOUBLE_EQ(axis.faces()[0], 0.0);
    EXPECT_DOUBLE_EQ(axis.faces()[1], 0.25);
    EXPECT_DOUBLE_EQ(axis.faces()[2], 0.5);
    EXPECT_DOUBLE_EQ(axis.faces()[3], 0.75);
    EXPECT_DOUBLE_EQ(axis.faces()[4], 1.0);
}

TEST(Uniform1D, ComputesMidpointCenters) {
    const Axis1D axis = Uniform1D::make(
        NVol{4},
        Length{1.0},
        XInit{0.0}
    );

    ASSERT_EQ(axis.centers().size(), static_cast<Size>(4));
    EXPECT_DOUBLE_EQ(axis.centers()[0], 0.125);
    EXPECT_DOUBLE_EQ(axis.centers()[1], 0.375);
    EXPECT_DOUBLE_EQ(axis.centers()[2], 0.625);
    EXPECT_DOUBLE_EQ(axis.centers()[3], 0.875);
}

TEST(Uniform1D, ComputesUniformCellLengths) {
    const Axis1D axis = Uniform1D::make(
        NVol{4},
        Length{1.0},
        XInit{0.0}
    );

    ASSERT_EQ(axis.cell_lengths().size(), static_cast<Size>(4));
    EXPECT_DOUBLE_EQ(axis.cell_lengths()[0], 0.25);
    EXPECT_DOUBLE_EQ(axis.cell_lengths()[1], 0.25);
    EXPECT_DOUBLE_EQ(axis.cell_lengths()[2], 0.25);
    EXPECT_DOUBLE_EQ(axis.cell_lengths()[3], 0.25);
}

TEST(Uniform1D, ComputesCenterDistances) {
    const Axis1D axis = Uniform1D::make(
        NVol{4},
        Length{1.0},
        XInit{0.0}
    );

    ASSERT_EQ(axis.dx_centers().size(), static_cast<Size>(5));
    EXPECT_DOUBLE_EQ(axis.dx_centers()[0], 0.125);
    EXPECT_DOUBLE_EQ(axis.dx_centers()[1], 0.25);
    EXPECT_DOUBLE_EQ(axis.dx_centers()[2], 0.25);
    EXPECT_DOUBLE_EQ(axis.dx_centers()[3], 0.25);
    EXPECT_DOUBLE_EQ(axis.dx_centers()[4], 0.125);
}

TEST(Uniform1D, SupportsShiftedInitialCoordinate) {
    const Axis1D axis = Uniform1D::make(
        NVol{2},
        Length{4.0},
        XInit{-1.0}
    );

    ASSERT_EQ(axis.faces().size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(axis.faces()[0], -1.0);
    EXPECT_DOUBLE_EQ(axis.faces()[1], 1.0);
    EXPECT_DOUBLE_EQ(axis.faces()[2], 3.0);

    ASSERT_EQ(axis.centers().size(), static_cast<Size>(2));
    EXPECT_DOUBLE_EQ(axis.centers()[0], 0.0);
    EXPECT_DOUBLE_EQ(axis.centers()[1], 2.0);
}

TEST(Uniform1D, FreeFunctionUsesDefaultFaceCenteredPattern) {
    const Axis1D axis = uniform_axis_1d(
        NVol{4},
        Length{1.0},
        XInit{0.0}
    );

    EXPECT_EQ(axis.num_cells(), static_cast<Size>(4));
    EXPECT_EQ(axis.num_faces(), static_cast<Size>(5));
    EXPECT_EQ(axis.pattern_name(), FaceCentered1D::name());

    EXPECT_DOUBLE_EQ(axis.faces()[0], 0.0);
    EXPECT_DOUBLE_EQ(axis.faces()[4], 1.0);
    EXPECT_DOUBLE_EQ(axis.centers()[0], 0.125);
    EXPECT_DOUBLE_EQ(axis.centers()[3], 0.875);
}

TEST(Uniform1D, DoesNotAcceptExplicitPatternArgument) {
    EXPECT_FALSE((Uniform1DAcceptsPatternArgument<VolumeCentered1D>));
    EXPECT_FALSE((Uniform1DAcceptsPatternArgument<FaceCentered1D>));
}

TEST(Uniform1D, FreeFunctionDoesNotAcceptExplicitPatternArgument) {
    EXPECT_FALSE(
        (UniformAxis1DFreeFunctionAcceptsPatternArgument<VolumeCentered1D>)
    );
    EXPECT_FALSE(
        (UniformAxis1DFreeFunctionAcceptsPatternArgument<FaceCentered1D>)
    );
}

TEST(Uniform1D, RejectsZeroVolumes) {
    try {
        [[maybe_unused]] const Axis1D axis = Uniform1D::make(
            NVol{0},
            Length{1.0},
            XInit{0.0}
        );
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.GRID.INVALID_NVOL"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Grid"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"Uniform1D"}
        );
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.distribution.Uniform1D"}
        );
        return;
    }

    FAIL() << "Uniform1D did not reject zero volumes.";
}

TEST(Uniform1D, RejectsZeroLength) {
    try {
        [[maybe_unused]] const Axis1D axis = Uniform1D::make(
            NVol{4},
            Length{0.0},
            XInit{0.0}
        );
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.GRID.INVALID_LENGTH"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Grid"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"Uniform1D"}
        );
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.distribution.Uniform1D"}
        );
        return;
    }

    FAIL() << "Uniform1D did not reject zero length.";
}

TEST(Uniform1D, RejectsNegativeLength) {
    try {
        [[maybe_unused]] const Axis1D axis = Uniform1D::make(
            NVol{4},
            Length{-1.0},
            XInit{0.0}
        );
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.GRID.INVALID_LENGTH"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Grid"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"Uniform1D"}
        );
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.distribution.Uniform1D"}
        );
        return;
    }

    FAIL() << "Uniform1D did not reject negative length.";
}

}  // namespace fvgrid