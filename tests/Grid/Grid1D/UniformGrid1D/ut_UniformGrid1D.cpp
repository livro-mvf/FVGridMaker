// ----------------------------------------------------------------------------
// File: ut_UniformGrid1D.cpp
// Project: FVGridMaker
// Description: Testes ativos para geração de malha 1D uniforme pelo builder.
// ----------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <FVGridMaker/ErrorHandling/FVGException.h>
#include <FVGridMaker/Grid/Common/Tags1D.hpp>
#include <FVGridMaker/Grid/Grid1D/Builders/Grid1DBuilder.hpp>
#include <FVGridMaker/Grid/Grid1D/Patterns/Distribution/Uniform1D.hpp>

#include <numeric>

using FVGridMaker::core::Index;
using FVGridMaker::core::Real;
using FVGridMaker::grid::CenteringTag;
using FVGridMaker::grid::DistributionTag;
using FVGridMaker::grid::grid1d::builders::Grid1DBuilder;
using FVGridMaker::grid::grid1d::patterns::distribution::Uniform1D;
using FVGridMaker::error::FVGException;

namespace {
constexpr Real eps = Real(1e-12);
}

TEST(Grid1DBuilderUniforme, GeraVetoresComTamanhosEsperados) {
    constexpr Index N = 4;

    const auto grid = Grid1DBuilder{}
        .setN(N)
        .setDomain(0.0, 1.0)
        .setDistribution<Uniform1D>()
        .build();

    EXPECT_EQ(grid.faces().size(), static_cast<std::size_t>(N + 1));
    EXPECT_EQ(grid.centers().size(), static_cast<std::size_t>(N));
    EXPECT_EQ(grid.deltasFaces().size(), static_cast<std::size_t>(N));
    EXPECT_EQ(grid.deltasCenters().size(), static_cast<std::size_t>(N + 1));

    EXPECT_EQ(grid.nVolumes(), N);
    EXPECT_EQ(grid.nFaces(), N + 1);
}

TEST(Grid1DBuilderUniforme, CalculaFacesCentrosEDistancias) {
    constexpr Index N = 4;
    constexpr Real A = 0.0;
    constexpr Real B = 1.0;
    constexpr Real dx = (B - A) / static_cast<Real>(N);

    const auto grid = Grid1DBuilder{}
        .setN(N)
        .setDomain(A, B)
        .setDistribution(DistributionTag::Uniform1D)
        .setCentering(CenteringTag::CellCentered)
        .build();

    const auto xf = grid.faces();
    const auto xc = grid.centers();
    const auto dF = grid.deltasFaces();
    const auto dC = grid.deltasCenters();

    for (Index i = 0; i <= N; ++i) {
        EXPECT_NEAR(xf[static_cast<std::size_t>(i)],
                    A + static_cast<Real>(i) * dx,
                    eps);
    }

    for (Index i = 0; i < N; ++i) {
        EXPECT_NEAR(xc[static_cast<std::size_t>(i)],
                    A + (static_cast<Real>(i) + Real(0.5)) * dx,
                    eps);
        EXPECT_NEAR(dF[static_cast<std::size_t>(i)], dx, eps);
    }

    EXPECT_NEAR(dC.front(), xc.front() - xf.front(), eps);
    for (Index i = 1; i < N; ++i) {
        EXPECT_NEAR(dC[static_cast<std::size_t>(i)],
                    xc[static_cast<std::size_t>(i)] - xc[static_cast<std::size_t>(i - 1)],
                    eps);
    }
    EXPECT_NEAR(dC.back(), xf.back() - xc.back(), eps);

    EXPECT_NEAR(std::accumulate(dF.begin(), dF.end(), Real(0)), B - A, eps);
    EXPECT_NEAR(std::accumulate(dC.begin(), dC.end(), Real(0)), B - A, eps);
}

TEST(Grid1DBuilderUniforme, PadraoDaMalhaEVolumeFaceCentrada) {
    const auto padrao = Grid1DBuilder{}
        .setN(4)
        .setDomain(0.0, 1.0)
        .setDistribution<Uniform1D>()
        .build();

    const auto explicita = Grid1DBuilder{}
        .setN(4)
        .setDomain(0.0, 1.0)
        .setDistribution<Uniform1D>()
        .setCentering(CenteringTag::FaceCentered)
        .build();

    EXPECT_EQ(padrao.padraoArmazenamento(), CenteringTag::FaceCentered);
    EXPECT_EQ(padrao.nomePadraoArmazenamento(), "FaceCentered");
    EXPECT_TRUE(padrao.ehFaceCentrada());
    EXPECT_FALSE(padrao.ehVolumeCentrada());

    EXPECT_EQ(explicita.padraoArmazenamento(), CenteringTag::FaceCentered);
    EXPECT_EQ(explicita.nomePadraoArmazenamento(), "FaceCentered");
    EXPECT_TRUE(explicita.ehFaceCentrada());
    EXPECT_FALSE(explicita.ehVolumeCentrada());

    ASSERT_EQ(padrao.faces().size(), explicita.faces().size());
    ASSERT_EQ(padrao.centers().size(), explicita.centers().size());
    ASSERT_EQ(padrao.deltasFaces().size(), explicita.deltasFaces().size());
    ASSERT_EQ(padrao.deltasCenters().size(), explicita.deltasCenters().size());

    for (std::size_t i = 0; i < padrao.faces().size(); ++i) {
        EXPECT_NEAR(padrao.faces()[i], explicita.faces()[i], eps);
    }
    for (std::size_t i = 0; i < padrao.centers().size(); ++i) {
        EXPECT_NEAR(padrao.centers()[i], explicita.centers()[i], eps);
    }
    for (std::size_t i = 0; i < padrao.deltasFaces().size(); ++i) {
        EXPECT_NEAR(padrao.deltasFaces()[i], explicita.deltasFaces()[i], eps);
    }
    for (std::size_t i = 0; i < padrao.deltasCenters().size(); ++i) {
        EXPECT_NEAR(padrao.deltasCenters()[i], explicita.deltasCenters()[i], eps);
    }
}

TEST(Grid1DBuilderUniforme, InformaPadraoVolumeCentradoQuandoConfigurado) {
    const auto grid = Grid1DBuilder{}
        .setN(4)
        .setDomain(0.0, 1.0)
        .setDistribution<Uniform1D>()
        .setCentering(CenteringTag::CellCentered)
        .build();

    EXPECT_EQ(grid.padraoArmazenamento(), CenteringTag::CellCentered);
    EXPECT_EQ(grid.nomePadraoArmazenamento(), "CellCentered");
    EXPECT_FALSE(grid.ehFaceCentrada());
    EXPECT_TRUE(grid.ehVolumeCentrada());
}

TEST(Grid1DBuilderUniforme, CasoMinimoComUmVolume) {
    const auto grid = Grid1DBuilder{}
        .setN(1)
        .setDomain(2.0, 4.0)
        .setDistribution<Uniform1D>()
        .build();

    ASSERT_EQ(grid.faces().size(), std::size_t{2});
    ASSERT_EQ(grid.centers().size(), std::size_t{1});
    ASSERT_EQ(grid.deltasFaces().size(), std::size_t{1});
    ASSERT_EQ(grid.deltasCenters().size(), std::size_t{2});

    EXPECT_NEAR(grid.faces()[0], 2.0, eps);
    EXPECT_NEAR(grid.faces()[1], 4.0, eps);
    EXPECT_NEAR(grid.centers()[0], 3.0, eps);
    EXPECT_NEAR(grid.deltasFaces()[0], 2.0, eps);
    EXPECT_NEAR(grid.deltasCenters()[0], 1.0, eps);
    EXPECT_NEAR(grid.deltasCenters()[1], 1.0, eps);
}

TEST(Grid1DBuilderUniforme, InformaSePossuiVolumesFicticios) {
    const auto sem_ficticios = Grid1DBuilder{}
        .setN(4)
        .setDomain(0.0, 1.0)
        .setDistribution<Uniform1D>()
        .build();

    const auto com_ficticios = Grid1DBuilder{}
        .setN(4)
        .setDomain(0.0, 1.0)
        .setNVolumesFicticios(1)
        .setDistribution<Uniform1D>()
        .build();

    EXPECT_FALSE(sem_ficticios.possuiVolumesFicticios());
    EXPECT_TRUE(com_ficticios.possuiVolumesFicticios());
}

TEST(Grid1DBuilderUniforme, IncluiVolumesFicticiosNosVetores) {
    constexpr Index N = 4;
    constexpr Index NFicticios = 2;
    constexpr Real A = 0.0;
    constexpr Real B = 1.0;
    constexpr Real dx = (B - A) / static_cast<Real>(N);
    constexpr Index NTotais = N + 2 * NFicticios;

    const auto grid = Grid1DBuilder{}
        .setN(N)
        .setDomain(A, B)
        .setNVolumesFicticios(NFicticios)
        .setDistribution<Uniform1D>()
        .build();

    EXPECT_EQ(grid.nPhysicalCells(), N);
    EXPECT_EQ(grid.nVolumesFisicos(), N);
    EXPECT_EQ(grid.nFicticios(), NFicticios);
    EXPECT_EQ(grid.nVolumesFicticios(), NFicticios);
    EXPECT_EQ(grid.nTotalCells(), NTotais);
    EXPECT_EQ(grid.nVolumesTotais(), NTotais);

    ASSERT_EQ(grid.faces().size(), static_cast<std::size_t>(NTotais + 1));
    ASSERT_EQ(grid.centers().size(), static_cast<std::size_t>(NTotais));
    ASSERT_EQ(grid.deltasFaces().size(), static_cast<std::size_t>(NTotais));
    ASSERT_EQ(grid.deltasCenters().size(), static_cast<std::size_t>(NTotais + 1));

    const auto xf = grid.faces();
    const auto xc = grid.centers();
    const auto dF = grid.deltasFaces();
    const auto dC = grid.deltasCenters();

    EXPECT_NEAR(xf.front(), A - static_cast<Real>(NFicticios) * dx, eps);
    EXPECT_NEAR(xf[static_cast<std::size_t>(NFicticios)], A, eps);
    EXPECT_NEAR(xf[static_cast<std::size_t>(NFicticios + N)], B, eps);
    EXPECT_NEAR(xf.back(), B + static_cast<Real>(NFicticios) * dx, eps);

    for (Index i = 0; i < NTotais; ++i) {
        const auto k = static_cast<std::size_t>(i);
        EXPECT_NEAR(dF[k], dx, eps);
        EXPECT_NEAR(xc[k], Real(0.5) * (xf[k] + xf[k + 1]), eps);
    }

    EXPECT_NEAR(dC.front(), xc.front() - xf.front(), eps);
    for (Index i = 1; i < NTotais; ++i) {
        EXPECT_NEAR(dC[static_cast<std::size_t>(i)],
                    xc[static_cast<std::size_t>(i)] - xc[static_cast<std::size_t>(i - 1)],
                    eps);
    }
    EXPECT_NEAR(dC.back(), xf.back() - xc.back(), eps);
}

TEST(Grid1DBuilderUniforme, ClassificaIndicesFisicosEFicticios) {
    constexpr Index N = 4;
    constexpr Index NFicticios = 2;

    const auto grid = Grid1DBuilder{}
        .setN(N)
        .setDomain(0.0, 1.0)
        .setNVolumesFicticios(NFicticios)
        .setDistribution<Uniform1D>()
        .build();

    EXPECT_EQ(grid.primeiroVolumeFisico(), 2);
    EXPECT_EQ(grid.ultimoVolumeFisico(), 5);
    EXPECT_EQ(grid.fimVolumesFisicos(), 6);

    EXPECT_EQ(grid.inicioVolumesFicticiosEsquerda(), 0);
    EXPECT_EQ(grid.fimVolumesFicticiosEsquerda(), 2);
    EXPECT_EQ(grid.inicioVolumesFicticiosDireita(), 6);
    EXPECT_EQ(grid.fimVolumesFicticiosDireita(), 8);

    EXPECT_FALSE(grid.indiceValido(-1));
    EXPECT_TRUE(grid.indiceValido(0));
    EXPECT_TRUE(grid.indiceValido(7));
    EXPECT_FALSE(grid.indiceValido(8));

    EXPECT_TRUE(grid.ehVolumeFicticioEsquerda(0));
    EXPECT_TRUE(grid.ehVolumeFicticioEsquerda(1));
    EXPECT_FALSE(grid.ehVolumeFicticioEsquerda(2));

    for (Index i = 2; i <= 5; ++i) {
        EXPECT_TRUE(grid.ehVolumeFisico(i));
        EXPECT_FALSE(grid.ehVolumeFicticio(i));
    }

    EXPECT_FALSE(grid.ehVolumeFisico(1));
    EXPECT_FALSE(grid.ehVolumeFisico(6));

    EXPECT_TRUE(grid.ehVolumeFicticioDireita(6));
    EXPECT_TRUE(grid.ehVolumeFicticioDireita(7));
    EXPECT_FALSE(grid.ehVolumeFicticioDireita(5));

    EXPECT_TRUE(grid.ehVolumeFicticio(0));
    EXPECT_TRUE(grid.ehVolumeFicticio(7));
    EXPECT_FALSE(grid.ehVolumeFicticio(3));
}

TEST(Grid1DBuilderUniforme, AcessaGrandezasPorIndiceTotal) {
    const auto grid = Grid1DBuilder{}
        .setN(4)
        .setDomain(0.0, 1.0)
        .setNVolumesFicticios(1)
        .setDistribution<Uniform1D>()
        .build();

    EXPECT_NEAR(grid.face(0), -0.25, eps);
    EXPECT_NEAR(grid.face(1),  0.0, eps);
    EXPECT_NEAR(grid.face(5),  1.0, eps);
    EXPECT_NEAR(grid.face(6),  1.25, eps);

    EXPECT_NEAR(grid.centro(0), -0.125, eps);
    EXPECT_NEAR(grid.centro(1),  0.125, eps);
    EXPECT_NEAR(grid.centro(4),  0.875, eps);
    EXPECT_NEAR(grid.centro(5),  1.125, eps);

    EXPECT_NEAR(grid.deltaFace(0), 0.25, eps);
    EXPECT_NEAR(grid.deltaFace(5), 0.25, eps);

    EXPECT_NEAR(grid.deltaCentro(0), 0.125, eps);
    EXPECT_NEAR(grid.deltaCentro(1), 0.25, eps);
    EXPECT_NEAR(grid.deltaCentro(6), 0.125, eps);

    EXPECT_EQ(grid.centros().size(), grid.centers().size());
    EXPECT_EQ(grid.deltasCentros().size(), grid.deltasCenters().size());
}

TEST(Grid1DBuilderUniforme, AcessaGrandezasPorIndiceFisico) {
    const auto grid = Grid1DBuilder{}
        .setN(4)
        .setDomain(0.0, 1.0)
        .setNVolumesFicticios(1)
        .setDistribution<Uniform1D>()
        .build();

    EXPECT_EQ(grid.primeiroVolumeFisico(), 1);

    EXPECT_NEAR(grid.faceFisica(0), 0.0, eps);
    EXPECT_NEAR(grid.faceFisica(4), 1.0, eps);

    EXPECT_NEAR(grid.centroFisico(0), 0.125, eps);
    EXPECT_NEAR(grid.centroFisico(3), 0.875, eps);

    EXPECT_NEAR(grid.deltaFaceFisica(0), 0.25, eps);
    EXPECT_NEAR(grid.deltaFaceFisica(3), 0.25, eps);

    EXPECT_NEAR(grid.deltaCentroFisico(0), 0.125, eps);
    EXPECT_NEAR(grid.deltaCentroFisico(1), 0.25, eps);
    EXPECT_NEAR(grid.deltaCentroFisico(4), 0.125, eps);
}

TEST(Grid1DBuilderUniforme, ComprimentoFisicoEntreFacesExtremasEPositivo) {
    constexpr Index N = 4;

    const auto grid = Grid1DBuilder{}
        .setN(N)
        .setDomain(0.0, 1.0)
        .setNVolumesFicticios(2)
        .setDistribution<Uniform1D>()
        .build();

    EXPECT_GT(grid.faceFisica(N) - grid.faceFisica(0), Real(0));
}

TEST(Grid1DBuilderUniforme, AcessoEscalarInvalidoLancaExcecaoPropria) {
    const auto grid = Grid1DBuilder{}
        .setN(4)
        .setDomain(0.0, 1.0)
        .setNVolumesFicticios(1)
        .setDistribution<Uniform1D>()
        .build();

    EXPECT_THROW((void)grid.face(-1), FVGException);
    EXPECT_THROW((void)grid.face(grid.nFaces()), FVGException);
    EXPECT_THROW((void)grid.centro(grid.nVolumes()), FVGException);
    EXPECT_THROW((void)grid.deltaFace(grid.nVolumes()), FVGException);
    EXPECT_THROW((void)grid.deltaCentro(grid.nFaces()), FVGException);

    EXPECT_THROW((void)grid.centroFisico(-1), FVGException);
    EXPECT_THROW((void)grid.centroFisico(grid.nVolumesFisicos()), FVGException);
    EXPECT_THROW((void)grid.faceFisica(grid.nVolumesFisicos() + 1), FVGException);
}

TEST(Grid1DBuilderUniforme, ConfiguracoesInvalidasLancamExcecaoPropria) {
    EXPECT_THROW(
        (void)Grid1DBuilder{}
            .setN(0)
            .setDomain(0.0, 1.0)
            .setDistribution<Uniform1D>()
            .build(),
        FVGException);

    EXPECT_THROW(
        (void)Grid1DBuilder{}
            .setN(-1)
            .setDomain(0.0, 1.0)
            .setDistribution<Uniform1D>()
            .build(),
        FVGException);

    EXPECT_THROW(
        (void)Grid1DBuilder{}
            .setN(-100)
            .setDomain(0.0, 1.0)
            .setDistribution<Uniform1D>()
            .build(),
        FVGException);

    EXPECT_THROW(
        (void)Grid1DBuilder{}
            .setN(4)
            .setDomain(1.0, 1.0)
            .setDistribution<Uniform1D>()
            .build(),
        FVGException);

    EXPECT_THROW(
        (void)Grid1DBuilder{}
            .setN(4)
            .setDomain(0.0, 1.0)
            .setNVolumesFicticios(-1)
            .setDistribution<Uniform1D>()
            .build(),
        FVGException);
}

TEST(Grid1DBuilderUniforme, ExcecaoUsaMensagemPadraoEmPortugues) {
    try {
        (void)Grid1DBuilder{}
            .setN(0)
            .setDomain(0.0, 1.0)
            .setDistribution<Uniform1D>()
            .build();
        FAIL() << "Era esperada uma FVGException.";
    } catch (const FVGException& e) {
        EXPECT_NE(std::string{e.what()}.find("Número de volumes N inválido"),
                  std::string::npos);
    }
}
