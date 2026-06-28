// ============================================================================
// Arquivo: tst_Custom1D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Testa o componente Custom1D no contexto de Distribution1D.
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
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/FVGridException.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Custom1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/CentersFromFaces1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/ConstantWeight1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Coordinates1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Domain1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/FaceCentered1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/FacesFromCenters1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

TEST(Custom1D, BuildsVolumeCenteredAxisFromFaceCoordinates) {
    const Axis1D axis = Custom1D::make(
        Coordinates1D::faces(
            std::vector<Real>{0.0, 0.2, 0.7, 1.0}
        ),
        VolumeCentered1D{}
    );

    EXPECT_EQ(axis.num_cells(), static_cast<Size>(3));
    EXPECT_EQ(axis.num_faces(), static_cast<Size>(4));
    EXPECT_EQ(axis.pattern_name(), VolumeCentered1D::name());

    ASSERT_EQ(axis.faces().size(), static_cast<Size>(4));
    EXPECT_DOUBLE_EQ(axis.faces()[0], 0.0);
    EXPECT_DOUBLE_EQ(axis.faces()[1], 0.2);
    EXPECT_DOUBLE_EQ(axis.faces()[2], 0.7);
    EXPECT_DOUBLE_EQ(axis.faces()[3], 1.0);

    ASSERT_EQ(axis.centers().size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(axis.centers()[0], 0.1);
    EXPECT_DOUBLE_EQ(axis.centers()[1], 0.45);
    EXPECT_DOUBLE_EQ(axis.centers()[2], 0.85);
}

TEST(Custom1D, BuildsCentersFromFacesAxisWithCustomWeight) {
    const Axis1D axis = Custom1D::make(
        Coordinates1D::faces(
            std::vector<Real>{0.0, 0.2, 0.7, 1.0}
        ),
        CentersFromFaces1D{ConstantWeight1D{0.25}}
    );

    EXPECT_EQ(axis.pattern_name(), CentersFromFaces1D<ConstantWeight1D>::name());

    ASSERT_EQ(axis.faces().size(), static_cast<Size>(4));
    EXPECT_DOUBLE_EQ(axis.faces()[0], 0.0);
    EXPECT_DOUBLE_EQ(axis.faces()[1], 0.2);
    EXPECT_DOUBLE_EQ(axis.faces()[2], 0.7);
    EXPECT_DOUBLE_EQ(axis.faces()[3], 1.0);

    ASSERT_EQ(axis.centers().size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(axis.centers()[0], 0.05);
    EXPECT_DOUBLE_EQ(axis.centers()[1], 0.325);
    EXPECT_DOUBLE_EQ(axis.centers()[2], 0.775);
}

TEST(Custom1D, ComputesVolumeCenteredMetrics) {
    const Axis1D axis = Custom1D::make(
        Coordinates1D::faces(
            std::vector<Real>{0.0, 0.2, 0.7, 1.0}
        ),
        VolumeCentered1D{}
    );

    ASSERT_EQ(axis.dx_faces().size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(axis.dx_faces()[0], 0.2);
    EXPECT_DOUBLE_EQ(axis.dx_faces()[1], 0.5);
    EXPECT_DOUBLE_EQ(axis.dx_faces()[2], 0.3);

    ASSERT_EQ(axis.dx_centers().size(), static_cast<Size>(4));
    EXPECT_DOUBLE_EQ(axis.dx_centers()[0], 0.1);
    EXPECT_DOUBLE_EQ(axis.dx_centers()[1], 0.35);
    EXPECT_DOUBLE_EQ(axis.dx_centers()[2], 0.4);
    EXPECT_DOUBLE_EQ(axis.dx_centers()[3], 0.15);
}

TEST(Custom1D, BuildsFaceCenteredAxisFromCentreCoordinatesAndDomain) {
    const Axis1D axis = Custom1D::make(
        Coordinates1D::centers(
            std::vector<Real>{0.1, 0.45, 0.85}
        ),
        FaceCentered1D{},
        Domain1D::from_length(XInit{0.0}, Length{1.0})
    );

    EXPECT_EQ(axis.num_cells(), static_cast<Size>(3));
    EXPECT_EQ(axis.num_faces(), static_cast<Size>(4));
    EXPECT_EQ(axis.pattern_name(), FaceCentered1D::name());

    ASSERT_EQ(axis.centers().size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(axis.centers()[0], 0.1);
    EXPECT_DOUBLE_EQ(axis.centers()[1], 0.45);
    EXPECT_DOUBLE_EQ(axis.centers()[2], 0.85);

    ASSERT_EQ(axis.faces().size(), static_cast<Size>(4));
    EXPECT_DOUBLE_EQ(axis.faces()[0], 0.0);
    EXPECT_DOUBLE_EQ(axis.faces()[1], 0.275);
    EXPECT_DOUBLE_EQ(axis.faces()[2], 0.65);
    EXPECT_DOUBLE_EQ(axis.faces()[3], 1.0);
}

TEST(Custom1D, BuildsFacesFromCentersAxisWithCustomWeight) {
    const Axis1D axis = Custom1D::make(
        Coordinates1D::centers(
            std::vector<Real>{0.1, 0.45, 0.85}
        ),
        FacesFromCenters1D{ConstantWeight1D{0.25}},
        Domain1D::from_length(XInit{0.0}, Length{1.0})
    );

    EXPECT_EQ(axis.pattern_name(), FacesFromCenters1D<ConstantWeight1D>::name());

    ASSERT_EQ(axis.centers().size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(axis.centers()[0], 0.1);
    EXPECT_DOUBLE_EQ(axis.centers()[1], 0.45);
    EXPECT_DOUBLE_EQ(axis.centers()[2], 0.85);

    ASSERT_EQ(axis.faces().size(), static_cast<Size>(4));
    EXPECT_DOUBLE_EQ(axis.faces()[0], 0.0);
    EXPECT_DOUBLE_EQ(axis.faces()[1], 0.1875);
    EXPECT_DOUBLE_EQ(axis.faces()[2], 0.55);
    EXPECT_DOUBLE_EQ(axis.faces()[3], 1.0);
}

TEST(Custom1D, BuildsFaceCenteredAxisFromCentreCoordinatesAndBounds) {
    const Axis1D axis = Custom1D::make(
        Coordinates1D::centers(
            std::vector<Real>{0.1, 0.45, 0.85}
        ),
        FaceCentered1D{},
        Domain1D::from_bounds(XInit{0.0}, XFinal{1.0})
    );

    EXPECT_EQ(axis.pattern_name(), FaceCentered1D::name());

    ASSERT_EQ(axis.faces().size(), static_cast<Size>(4));
    EXPECT_DOUBLE_EQ(axis.faces()[0], 0.0);
    EXPECT_DOUBLE_EQ(axis.faces()[1], 0.275);
    EXPECT_DOUBLE_EQ(axis.faces()[2], 0.65);
    EXPECT_DOUBLE_EQ(axis.faces()[3], 1.0);
}

TEST(Custom1D, ComputesFaceCenteredMetrics) {
    const Axis1D axis = Custom1D::make(
        Coordinates1D::centers(
            std::vector<Real>{0.1, 0.45, 0.85}
        ),
        FaceCentered1D{},
        Domain1D::from_length(XInit{0.0}, Length{1.0})
    );

    ASSERT_EQ(axis.dx_faces().size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(axis.dx_faces()[0], 0.275);
    EXPECT_DOUBLE_EQ(axis.dx_faces()[1], 0.375);
    EXPECT_DOUBLE_EQ(axis.dx_faces()[2], 0.35);

    ASSERT_EQ(axis.dx_centers().size(), static_cast<Size>(4));
    EXPECT_DOUBLE_EQ(axis.dx_centers()[0], 0.1);
    EXPECT_DOUBLE_EQ(axis.dx_centers()[1], 0.35);
    EXPECT_DOUBLE_EQ(axis.dx_centers()[2], 0.4);
    EXPECT_DOUBLE_EQ(axis.dx_centers()[3], 0.15);
}

TEST(Custom1D, FreeFunctionBuildsVolumeCenteredAxis) {
    const Axis1D axis = custom_axis_1d(
        Coordinates1D::faces(
            std::vector<Real>{0.0, 0.2, 0.7, 1.0}
        ),
        VolumeCentered1D{}
    );

    EXPECT_EQ(axis.pattern_name(), VolumeCentered1D::name());
    EXPECT_DOUBLE_EQ(axis.centers()[0], 0.1);
    EXPECT_DOUBLE_EQ(axis.centers()[1], 0.45);
    EXPECT_DOUBLE_EQ(axis.centers()[2], 0.85);
}

TEST(Custom1D, FreeFunctionBuildsFaceCenteredAxis) {
    const Axis1D axis = custom_axis_1d(
        Coordinates1D::centers(
            std::vector<Real>{0.1, 0.45, 0.85}
        ),
        FaceCentered1D{},
        Domain1D::from_length(XInit{0.0}, Length{1.0})
    );

    EXPECT_EQ(axis.pattern_name(), FaceCentered1D::name());
    EXPECT_DOUBLE_EQ(axis.faces()[0], 0.0);
    EXPECT_DOUBLE_EQ(axis.faces()[1], 0.275);
    EXPECT_DOUBLE_EQ(axis.faces()[2], 0.65);
    EXPECT_DOUBLE_EQ(axis.faces()[3], 1.0);
}

TEST(Custom1D, StoresClassIdentity) {
    EXPECT_EQ(Custom1D::id().module(), std::string_view{"OneDimensional"});
    EXPECT_EQ(Custom1D::id().class_name(), std::string_view{"Custom1D"});
    EXPECT_EQ(
        Custom1D::id().class_id(),
        std::string_view{"fvgrid.1d.distribution.Custom1D"}
    );

    EXPECT_EQ(Custom1D::class_name(), std::string_view{"Custom1D"});
    EXPECT_EQ(
        Custom1D::class_id(),
        std::string_view{"fvgrid.1d.distribution.Custom1D"}
    );
}

TEST(Custom1D, RejectsCentersForVolumeCenteredPattern) {
    try {
        [[maybe_unused]] const Axis1D axis = Custom1D::make(
            Coordinates1D::centers(
                std::vector<Real>{0.1, 0.45, 0.85}
            ),
            VolumeCentered1D{}
        );
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.GRID.INVALID_COORDINATE_KIND"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Grid"});
        EXPECT_EQ(exception.record().source.class_name(), std::string_view{"Custom1D"});
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.distribution.Custom1D"}
        );
        return;
    }

    FAIL() << "Custom1D accepted centers for a volume-centred pattern.";
}

TEST(Custom1D, RejectsCentersForCentersFromFacesPattern) {
    try {
        [[maybe_unused]] const Axis1D axis = Custom1D::make(
            Coordinates1D::centers(
                std::vector<Real>{0.1, 0.45, 0.85}
            ),
            CentersFromFaces1D{ConstantWeight1D{0.5}}
        );
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.GRID.INVALID_COORDINATE_KIND"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Grid"});
        EXPECT_EQ(exception.record().source.class_name(), std::string_view{"Custom1D"});
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.distribution.Custom1D"}
        );
        return;
    }

    FAIL() << "Custom1D accepted centers for a faces-to-centres pattern.";
}

TEST(Custom1D, RejectsFacesForFaceCenteredPattern) {
    try {
        [[maybe_unused]] const Axis1D axis = Custom1D::make(
            Coordinates1D::faces(
                std::vector<Real>{0.0, 0.2, 0.7, 1.0}
            ),
            FaceCentered1D{},
            Domain1D::from_length(XInit{0.0}, Length{1.0})
        );
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.GRID.INVALID_COORDINATE_KIND"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Grid"});
        EXPECT_EQ(exception.record().source.class_name(), std::string_view{"Custom1D"});
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.distribution.Custom1D"}
        );
        return;
    }

    FAIL() << "Custom1D accepted faces for a face-centred pattern.";
}

TEST(Custom1D, RejectsFacesForFacesFromCentersPattern) {
    try {
        [[maybe_unused]] const Axis1D axis = Custom1D::make(
            Coordinates1D::faces(
                std::vector<Real>{0.0, 0.2, 0.7, 1.0}
            ),
            FacesFromCenters1D{ConstantWeight1D{0.5}},
            Domain1D::from_length(XInit{0.0}, Length{1.0})
        );
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.GRID.INVALID_COORDINATE_KIND"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Grid"});
        EXPECT_EQ(exception.record().source.class_name(), std::string_view{"Custom1D"});
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.distribution.Custom1D"}
        );
        return;
    }

    FAIL() << "Custom1D accepted faces for a centres-to-faces pattern.";
}

TEST(Custom1D, RejectsMissingDomainForFaceCenteredPattern) {
    try {
        [[maybe_unused]] const Axis1D axis = Custom1D::make(
            Coordinates1D::centers(
                std::vector<Real>{0.1, 0.45, 0.85}
            ),
            FaceCentered1D{}
        );
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.CORE.INVALID_ARGUMENT"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Core"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"FaceCentered1D"}
        );
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.grid_pattern.FaceCentered1D"}
        );
        return;
    }

    FAIL() << "Custom1D accepted a face-centred pattern without domain bounds.";
}

TEST(Custom1D, RejectsNonIncreasingFaceCenteredCenters) {
    try {
        [[maybe_unused]] const Axis1D axis = Custom1D::make(
            Coordinates1D::centers(
                std::vector<Real>{0.1, 0.45, 0.45}
            ),
            FaceCentered1D{},
            Domain1D::from_length(XInit{0.0}, Length{1.0})
        );
    } catch (const FVGridException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVGRID.GRID.NON_INCREASING_CENTERS"}
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Grid"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"FaceCentered1D"}
        );
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvgrid.1d.grid_pattern.FaceCentered1D"}
        );
        return;
    }

    FAIL() << "Custom1D accepted non-increasing face-centred centres.";
}

}  // namespace fvgrid