// ============================================================================
// Arquivo: tst_Random1D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Testa o componente Random1D no contexto de Distribution1D.
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
#include <FVGridMaker/OneDimensional/Distribution1D/Random1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/FaceCentered1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

TEST(Random1D, BuildsDefaultFaceCenteredAxis) {
    const Axis1D axis = Random1D::make(
        NVol{5},
        Length{1.0},
        XInit{0.0},
        Seed{1234}
    );

    EXPECT_EQ(axis.num_cells(), static_cast<Size>(5));
    EXPECT_EQ(axis.num_faces(), static_cast<Size>(6));
    EXPECT_EQ(axis.pattern_name(), FaceCentered1D::name());

    EXPECT_DOUBLE_EQ(axis.faces().front(), 0.0);
    EXPECT_DOUBLE_EQ(axis.faces().back(), 1.0);
}

TEST(Random1D, BuildsShiftedDefaultAxis) {
    const Axis1D axis = Random1D::make(
        NVol{4},
        Length{2.0},
        XInit{-1.0},
        Seed{1234}
    );

    EXPECT_EQ(axis.num_cells(), static_cast<Size>(4));
    EXPECT_DOUBLE_EQ(axis.faces().front(), -1.0);
    EXPECT_DOUBLE_EQ(axis.faces().back(), 1.0);
}

TEST(Random1D, SameSeedReproducesDefaultFaceCenteredAxis) {
    const Axis1D first = Random1D::make(
        NVol{8},
        Length{1.0},
        XInit{0.0},
        Seed{99}
    );

    const Axis1D second = Random1D::make(
        NVol{8},
        Length{1.0},
        XInit{0.0},
        Seed{99}
    );

    ASSERT_EQ(first.faces().size(), second.faces().size());
    ASSERT_EQ(first.centers().size(), second.centers().size());

    for (Size i = 0; i < first.faces().size(); ++i) {
        EXPECT_DOUBLE_EQ(first.faces()[i], second.faces()[i]);
    }

    for (Size i = 0; i < first.centers().size(); ++i) {
        EXPECT_DOUBLE_EQ(first.centers()[i], second.centers()[i]);
    }
}

TEST(Random1D, DifferentSeedsCanProduceDifferentDefaultFaceCenteredAxes) {
    const Axis1D first = Random1D::make(
        NVol{8},
        Length{1.0},
        XInit{0.0},
        Seed{1}
    );

    const Axis1D second = Random1D::make(
        NVol{8},
        Length{1.0},
        XInit{0.0},
        Seed{2}
    );

    bool any_difference = false;

    for (Size i = 1; i + 1 < first.faces().size(); ++i) {
        if (first.faces()[i] != second.faces()[i]) {
            any_difference = true;
            break;
        }
    }

    EXPECT_TRUE(any_difference);
}

TEST(Random1D, DefaultFaceCenteredAxisHasIncreasingFaces) {
    const Axis1D axis = Random1D::make(
        NVol{12},
        Length{1.0},
        XInit{0.0},
        Seed{42}
    );

    for (Size i = 1; i < axis.faces().size(); ++i) {
        EXPECT_GT(axis.faces()[i], axis.faces()[i - 1]);
    }
}

TEST(Random1D, DefaultFaceCenteredAxisHasPositiveCellLengths) {
    const Axis1D axis = Random1D::make(
        NVol{12},
        Length{1.0},
        XInit{0.0},
        Seed{42}
    );

    for (Size i = 0; i < axis.cell_lengths().size(); ++i) {
        EXPECT_GT(axis.cell_lengths()[i], 0.0);
    }
}

TEST(Random1D, VolumeCenteredAxisRespectsMinimumSpacing) {
    const Axis1D axis = Random1D::make(
        NVol{8},
        Length{1.0},
        XInit{0.0},
        Seed{42},
        MinSpacing{0.05},
        VolumeCentered1D{}
    );

    for (Size i = 0; i < axis.cell_lengths().size(); ++i) {
        EXPECT_GE(axis.cell_lengths()[i], 0.049999999999);
    }
}

TEST(Random1D, SupportsExplicitVolumeCenteredPattern) {
    const Axis1D axis = Random1D::make(
        NVol{5},
        Length{1.0},
        XInit{0.0},
        Seed{1234},
        VolumeCentered1D{}
    );

    EXPECT_EQ(axis.pattern_name(), VolumeCentered1D::name());
    EXPECT_EQ(axis.num_cells(), static_cast<Size>(5));
    EXPECT_EQ(axis.num_faces(), static_cast<Size>(6));
    EXPECT_DOUBLE_EQ(axis.faces().front(), 0.0);
    EXPECT_DOUBLE_EQ(axis.faces().back(), 1.0);
}

TEST(Random1D, SupportsExplicitFaceCenteredPattern) {
    const Axis1D axis = Random1D::make(
        NVol{5},
        Length{1.0},
        XInit{0.0},
        Seed{1234},
        FaceCentered1D{}
    );

    EXPECT_EQ(axis.pattern_name(), FaceCentered1D::name());
    EXPECT_EQ(axis.num_cells(), static_cast<Size>(5));
    EXPECT_EQ(axis.num_faces(), static_cast<Size>(6));
    EXPECT_DOUBLE_EQ(axis.faces().front(), 0.0);
    EXPECT_DOUBLE_EQ(axis.faces().back(), 1.0);

    for (Size i = 0; i < axis.centers().size(); ++i) {
        EXPECT_GT(axis.centers()[i], 0.0);
        EXPECT_LT(axis.centers()[i], 1.0);
    }
}

TEST(Random1D, SameSeedReproducesFaceCenteredAxis) {
    const Axis1D first = Random1D::make(
        NVol{8},
        Length{1.0},
        XInit{0.0},
        Seed{99},
        FaceCentered1D{}
    );

    const Axis1D second = Random1D::make(
        NVol{8},
        Length{1.0},
        XInit{0.0},
        Seed{99},
        FaceCentered1D{}
    );

    ASSERT_EQ(first.faces().size(), second.faces().size());
    ASSERT_EQ(first.centers().size(), second.centers().size());

    for (Size i = 0; i < first.faces().size(); ++i) {
        EXPECT_DOUBLE_EQ(first.faces()[i], second.faces()[i]);
    }

    for (Size i = 0; i < first.centers().size(); ++i) {
        EXPECT_DOUBLE_EQ(first.centers()[i], second.centers()[i]);
    }
}

TEST(Random1D, FaceCenteredAxisHasIncreasingCenters) {
    const Axis1D axis = Random1D::make(
        NVol{10},
        Length{1.0},
        XInit{0.0},
        Seed{42},
        FaceCentered1D{}
    );

    for (Size i = 1; i < axis.centers().size(); ++i) {
        EXPECT_GT(axis.centers()[i], axis.centers()[i - 1]);
    }
}

TEST(Random1D, FaceCenteredAxisRespectsMinimumCenterSpacing) {
    const Axis1D axis = Random1D::make(
        NVol{5},
        Length{1.0},
        XInit{0.0},
        Seed{42},
        MinSpacing{0.05},
        FaceCentered1D{}
    );

    ASSERT_EQ(axis.dx_centers().size(), static_cast<Size>(6));

    for (Size i = 0; i < axis.dx_centers().size(); ++i) {
        EXPECT_GE(axis.dx_centers()[i], 0.049999999999);
    }
}

TEST(Random1D, FreeFunctionUsesDefaultPattern) {
    const Axis1D axis = random_axis_1d(
        NVol{5},
        Length{1.0},
        XInit{0.0},
        Seed{7}
    );

    EXPECT_EQ(axis.pattern_name(), FaceCentered1D::name());
    EXPECT_EQ(axis.num_cells(), static_cast<Size>(5));
}

TEST(Random1D, FreeFunctionSupportsMinimumSpacing) {
    const Axis1D axis = random_axis_1d(
        NVol{5},
        Length{1.0},
        XInit{0.0},
        Seed{7},
        MinSpacing{0.05}
    );

    for (Size i = 0; i < axis.dx_centers().size(); ++i) {
        EXPECT_GE(axis.dx_centers()[i], 0.049999999999);
    }
}

TEST(Random1D, FreeFunctionSupportsPatternObject) {
    const Axis1D axis = random_axis_1d(
        NVol{5},
        Length{1.0},
        XInit{0.0},
        Seed{7},
        FaceCentered1D{}
    );

    EXPECT_EQ(axis.pattern_name(), FaceCentered1D::name());
    EXPECT_EQ(axis.num_cells(), static_cast<Size>(5));
}

TEST(Random1D, FreeFunctionSupportsMinimumSpacingAndPatternObject) {
    const Axis1D axis = random_axis_1d(
        NVol{5},
        Length{1.0},
        XInit{0.0},
        Seed{7},
        MinSpacing{0.05},
        FaceCentered1D{}
    );

    EXPECT_EQ(axis.pattern_name(), FaceCentered1D::name());

    for (Size i = 0; i < axis.dx_centers().size(); ++i) {
        EXPECT_GE(axis.dx_centers()[i], 0.049999999999);
    }
}

TEST(Random1D, RejectsZeroVolumes) {
    try {
        const Axis1D axis = Random1D::make(
            NVol{0},
            Length{1.0},
            XInit{0.0},
            Seed{1}
        );
        (void)axis;
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.GRID.INVALID_NVOL"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Grid"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"Random1D"}
        );
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.distribution.Random1D"}
        );
        return;
    }

    FAIL() << "Random1D did not reject zero volumes.";
}

TEST(Random1D, RejectsZeroLength) {
    try {
        const Axis1D axis = Random1D::make(
            NVol{4},
            Length{0.0},
            XInit{0.0},
            Seed{1}
        );
        (void)axis;
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.GRID.INVALID_LENGTH"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Grid"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"Random1D"}
        );
        return;
    }

    FAIL() << "Random1D did not reject zero length.";
}

TEST(Random1D, RejectsNegativeLength) {
    try {
        const Axis1D axis = Random1D::make(
            NVol{4},
            Length{-1.0},
            XInit{0.0},
            Seed{1}
        );
        (void)axis;
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.GRID.INVALID_LENGTH"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Grid"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"Random1D"}
        );
        return;
    }

    FAIL() << "Random1D did not reject negative length.";
}

TEST(Random1D, RejectsNegativeMinimumSpacing) {
    try {
        const Axis1D axis = Random1D::make(
            NVol{4},
            Length{1.0},
            XInit{0.0},
            Seed{1},
            MinSpacing{-0.1}
        );
        (void)axis;
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.GRID.INVALID_MIN_SPACING"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Grid"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"Random1D"}
        );
        return;
    }

    FAIL() << "Random1D did not reject negative minimum spacing.";
}

TEST(Random1D, RejectsImpossibleMinimumSpacingForFaces) {
    try {
        const Axis1D axis = Random1D::make(
            NVol{4},
            Length{1.0},
            XInit{0.0},
            Seed{1},
            MinSpacing{0.30},
            VolumeCentered1D{}
        );
        (void)axis;
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.GRID.IMPOSSIBLE_MIN_SPACING"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Grid"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"Random1D"}
        );
        return;
    }

    FAIL() << "Random1D accepted an impossible face spacing.";
}

TEST(Random1D, RejectsImpossibleMinimumSpacingForCenters) {
    try {
        const Axis1D axis = Random1D::make(
            NVol{4},
            Length{1.0},
            XInit{0.0},
            Seed{1},
            MinSpacing{0.30},
            FaceCentered1D{}
        );
        (void)axis;
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.GRID.IMPOSSIBLE_MIN_SPACING"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Grid"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"Random1D"}
        );
        return;
    }

    FAIL() << "Random1D accepted an impossible centre spacing.";
}

}  // namespace fvgrid