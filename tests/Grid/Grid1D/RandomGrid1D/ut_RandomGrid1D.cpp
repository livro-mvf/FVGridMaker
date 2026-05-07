// ----------------------------------------------------------------------------
// File: ut_RandomGrid1D.cpp
// Project: FVGridMaker
// Description: Testes ativos para malhas 1D aleatórias.
// ----------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <FVGridMaker/ErrorHandling/FVGException.h>
#include <FVGridMaker/Grid/Common/Tags1D.hpp>
#include <FVGridMaker/Grid/Grid1D/Builders/Grid1DBuilder.hpp>
#include <FVGridMaker/Grid/Grid1D/Patterns/Distribution/Random1D.hpp>

#include <cstddef>
#include <vector>

using FVGridMaker::core::Index;
using FVGridMaker::core::Real;
using FVGridMaker::error::FVGException;
using FVGridMaker::grid::CenteringTag;
using FVGridMaker::grid::grid1d::builders::Grid1DBuilder;
using FVGridMaker::grid::grid1d::patterns::distribution::Random1D;

namespace {

constexpr Real eps = Real(1e-12);

std::vector<Real> to_vector(auto values) {
    return {values.begin(), values.end()};
}

void expect_strictly_increasing(auto values) {
    for (std::size_t i = 1; i < values.size(); ++i) {
        EXPECT_GT(values[i], values[i - 1u]);
    }
}

} // namespace

TEST(Grid1DBuilderRandom, SeedFixoReproduzMesmaMalhaVolumeCentrada) {
    const auto g1 = Grid1DBuilder{}
        .setN(20)
        .setDomain(0.0, 1.0)
        .setCentering(CenteringTag::CellCentered)
        .setDistribution<Random1D>()
        .setRandomPercentual(0.20)
        .setRandomSeed(12345u)
        .build();

    const auto g2 = Grid1DBuilder{}
        .setN(20)
        .setDomain(0.0, 1.0)
        .setCentering(CenteringTag::CellCentered)
        .setDistribution<Random1D>()
        .setRandomPercentual(0.20)
        .setRandomSeed(12345u)
        .build();

    EXPECT_EQ(to_vector(g1.faces()), to_vector(g2.faces()));
    EXPECT_EQ(to_vector(g1.centers()), to_vector(g2.centers()));
    EXPECT_EQ(to_vector(g1.deltasFaces()), to_vector(g2.deltasFaces()));
    EXPECT_EQ(to_vector(g1.deltasCenters()), to_vector(g2.deltasCenters()));
}

TEST(Grid1DBuilderRandom, VolumeCentradoGeraFacesECalculaCentros) {
    constexpr Index N = 10;
    constexpr Real A = 0.0;
    constexpr Real B = 1.0;

    const auto grid = Grid1DBuilder{}
        .setN(N)
        .setDomain(A, B)
        .setCentering(CenteringTag::CellCentered)
        .setDistribution<Random1D>()
        .setRandomPercentual(0.20)
        .setRandomSeed(12345u)
        .build();

    EXPECT_EQ(grid.padraoArmazenamento(), CenteringTag::CellCentered);
    ASSERT_EQ(grid.faces().size(), static_cast<std::size_t>(N + 1));
    ASSERT_EQ(grid.centers().size(), static_cast<std::size_t>(N));

    EXPECT_NEAR(grid.faces().front(), A, eps);
    EXPECT_NEAR(grid.faces().back(), B, eps);
    expect_strictly_increasing(grid.faces());
    expect_strictly_increasing(grid.centers());

    for (Index i = 0; i < N; ++i) {
        const auto k = static_cast<std::size_t>(i);
        EXPECT_NEAR(grid.centers()[k],
                    Real(0.5) * (grid.faces()[k] + grid.faces()[k + 1u]),
                    eps);
        EXPECT_GT(grid.deltasFaces()[k], Real(0));
    }
}

TEST(Grid1DBuilderRandom, FaceCentradaGeraCentrosECalculaFaces) {
    constexpr Index N = 10;
    constexpr Real A = 0.0;
    constexpr Real B = 1.0;

    const auto grid = Grid1DBuilder{}
        .setN(N)
        .setDomain(A, B)
        .setCentering(CenteringTag::FaceCentered)
        .setDistribution<Random1D>()
        .setRandomPercentual(0.20)
        .setRandomSeed(12345u)
        .build();

    EXPECT_EQ(grid.padraoArmazenamento(), CenteringTag::FaceCentered);
    ASSERT_EQ(grid.faces().size(), static_cast<std::size_t>(N + 1));
    ASSERT_EQ(grid.centers().size(), static_cast<std::size_t>(N));

    EXPECT_NEAR(grid.faces().front(), A, eps);
    EXPECT_NEAR(grid.faces().back(), B, eps);
    expect_strictly_increasing(grid.faces());
    expect_strictly_increasing(grid.centers());

    for (Index i = 1; i < N; ++i) {
        const auto k = static_cast<std::size_t>(i);
        EXPECT_NEAR(grid.faces()[k],
                    Real(0.5) * (grid.centers()[k - 1u] + grid.centers()[k]),
                    eps);
    }
}

TEST(Grid1DBuilderRandom, ComprimentoFisicoEntreFacesExtremasEPositivo) {
    constexpr Index N = 10;

    const auto volume_centrado = Grid1DBuilder{}
        .setN(N)
        .setDomain(0.0, 1.0)
        .setCentering(CenteringTag::CellCentered)
        .setDistribution<Random1D>()
        .setRandomPercentual(0.20)
        .setRandomSeed(12345u)
        .build();

    const auto face_centrada = Grid1DBuilder{}
        .setN(N)
        .setDomain(0.0, 1.0)
        .setCentering(CenteringTag::FaceCentered)
        .setDistribution<Random1D>()
        .setRandomPercentual(0.20)
        .setRandomSeed(12345u)
        .build();

    EXPECT_GT(volume_centrado.faceFisica(N) - volume_centrado.faceFisica(0),
              Real(0));
    EXPECT_GT(face_centrada.faceFisica(N) - face_centrada.faceFisica(0),
              Real(0));
}

TEST(Grid1DBuilderRandom, VolumesFicticiosEspelhamVolumesFisicos) {
    constexpr Index N = 8;
    constexpr Index NFicticios = 2;
    constexpr Real A = 0.0;
    constexpr Real B = 1.0;

    const auto volume_centrado = Grid1DBuilder{}
        .setN(N)
        .setDomain(A, B)
        .setNVolumesFicticios(NFicticios)
        .setCentering(CenteringTag::CellCentered)
        .setDistribution<Random1D>()
        .setRandomPercentual(0.20)
        .setRandomSeed(12345u)
        .build();

    EXPECT_NEAR(volume_centrado.faceFisica(0), A, eps);
    EXPECT_NEAR(volume_centrado.faceFisica(N), B, eps);
    EXPECT_NEAR(volume_centrado.deltaFace(1),
                volume_centrado.deltaFaceFisica(0),
                eps);
    EXPECT_NEAR(volume_centrado.deltaFace(0),
                volume_centrado.deltaFaceFisica(1),
                eps);
    EXPECT_NEAR(volume_centrado.deltaFace(NFicticios + N),
                volume_centrado.deltaFaceFisica(N - 1),
                eps);
    EXPECT_NEAR(volume_centrado.deltaFace(NFicticios + N + 1),
                volume_centrado.deltaFaceFisica(N - 2),
                eps);

    const auto face_centrada = Grid1DBuilder{}
        .setN(N)
        .setDomain(A, B)
        .setNVolumesFicticios(NFicticios)
        .setCentering(CenteringTag::FaceCentered)
        .setDistribution<Random1D>()
        .setRandomPercentual(0.20)
        .setRandomSeed(12345u)
        .build();

    EXPECT_NEAR(face_centrada.faceFisica(0), A, eps);
    EXPECT_NEAR(face_centrada.faceFisica(N), B, eps);
    EXPECT_NEAR(face_centrada.centro(1),
                Real(2) * A - face_centrada.centroFisico(0),
                eps);
    EXPECT_NEAR(face_centrada.centro(0),
                Real(2) * A - face_centrada.centroFisico(1),
                eps);
    EXPECT_NEAR(face_centrada.centro(NFicticios + N),
                Real(2) * B - face_centrada.centroFisico(N - 1),
                eps);
    EXPECT_NEAR(face_centrada.centro(NFicticios + N + 1),
                Real(2) * B - face_centrada.centroFisico(N - 2),
                eps);
}

TEST(Grid1DBuilderRandom, PercentualAltoRejeitaAmostrasInvalidasEAindaGeraMalha) {
    constexpr Index N = 10;

    const auto grid = Grid1DBuilder{}
        .setN(N)
        .setDomain(0.0, 1.0)
        .setDistribution<Random1D>()
        .setRandomPercentual(0.50)
        .setRandomSeed(12345u)
        .build();

    const Real dxUniforme = Real(1) / static_cast<Real>(N);
    const Real minimo = Real(0.01) * dxUniforme;

    for (Real dx : grid.deltasFaces()) {
        EXPECT_GE(dx, minimo - eps);
    }
}

TEST(Grid1DBuilderRandom, PercentualForaDaFaixaDeEntradaFalha) {
    EXPECT_THROW(
        (void)Grid1DBuilder{}
            .setN(2)
            .setDomain(0.0, 1.0)
            .setDistribution<Random1D>()
            .setRandomPercentual(-0.01)
            .build(),
        FVGException);

    EXPECT_THROW(
        (void)Grid1DBuilder{}
            .setN(2)
            .setDomain(0.0, 1.0)
            .setDistribution<Random1D>()
            .setRandomPercentual(1.01)
            .build(),
        FVGException);

    EXPECT_NO_THROW(
        (void)Grid1DBuilder{}
            .setN(2)
            .setDomain(0.0, 1.0)
            .setDistribution<Random1D>()
            .setRandomPercentual(0.90)
            .build());
}

TEST(Grid1DBuilderRandom, PercentualQuarentaENoveEMeioPorCentoTambemEValido) {
    EXPECT_NO_THROW(
        (void)Grid1DBuilder{}
            .setN(2)
            .setDomain(0.0, 1.0)
            .setDistribution<Random1D>()
            .setRandomPercentual(0.495)
            .build());
}

TEST(Grid1DBuilderRandom, EspacamentoPrimarioMinimoRespeitaUmPorCentoDoUniforme) {
    constexpr Index N = 30;
    constexpr Real A = 0.0;
    constexpr Real B = 3.0;
    constexpr Real percentual = 0.90;
    constexpr Real dxUniforme = (B - A) / static_cast<Real>(N);
    constexpr Real minimo = Real(0.01) * dxUniforme;

    const auto volume_centrado = Grid1DBuilder{}
        .setN(N)
        .setDomain(A, B)
        .setCentering(CenteringTag::CellCentered)
        .setDistribution<Random1D>()
        .setRandomPercentual(percentual)
        .setRandomSeed(12345u)
        .build();

    for (Real dx : volume_centrado.deltasFaces()) {
        EXPECT_GE(dx, minimo - eps);
    }

    const auto face_centrada = Grid1DBuilder{}
        .setN(N)
        .setDomain(A, B)
        .setCentering(CenteringTag::FaceCentered)
        .setDistribution<Random1D>()
        .setRandomPercentual(percentual)
        .setRandomSeed(12345u)
        .build();

    const auto centros = face_centrada.centers();
    for (std::size_t i = 1; i < centros.size(); ++i) {
        EXPECT_GE(centros[i] - centros[i - 1u], minimo - eps);
    }
}
