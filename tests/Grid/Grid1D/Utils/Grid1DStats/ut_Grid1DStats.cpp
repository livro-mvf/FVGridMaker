// ----------------------------------------------------------------------------
// File: ut_Grid1DStats.cpp
// Project: FVGridMaker
// Description: Testes para estatisticas de malha 1D.
// ----------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <FVGridMaker/Grid/Common/Tags1D.hpp>
#include <FVGridMaker/Grid/Grid1D/Builders/Grid1DBuilder.hpp>
#include <FVGridMaker/Grid/Grid1D/Patterns/Distribution/Random1D.hpp>
#include <FVGridMaker/Grid/Grid1D/Patterns/Distribution/Uniform1D.hpp>
#include <FVGridMaker/Grid/Grid1D/Utils/Grid1DStats.hpp>
#include <FVGridMaker/Grid/Grid1D/Utils/Grid1DStatsExec.hpp>

#include <span>
#include <vector>

using FVGridMaker::core::Index;
using FVGridMaker::core::Real;
using FVGridMaker::grid::CenteringTag;
using FVGridMaker::grid::grid1d::builders::Grid1DBuilder;
using FVGridMaker::grid::grid1d::patterns::distribution::Random1D;
using FVGridMaker::grid::grid1d::patterns::distribution::Uniform1D;
using FVGridMaker::grid::grid1d::utils::ExecPolicy;
using FVGridMaker::grid::grid1d::utils::Grid1DStats;
using FVGridMaker::grid::grid1d::utils::Grid1DStatsExec;

namespace {
constexpr Real eps = Real(1e-12);
}

TEST(Grid1DStats, CalculaEstatisticasBasicasDeSpan) {
    const std::vector<Real> values{1.0, 2.0, 3.0};

    const auto stats =
        Grid1DStats::basic(std::span<const Real>{values.data(), values.size()});

    EXPECT_EQ(stats.count, std::size_t{3});
    EXPECT_NEAR(stats.sum, 6.0, eps);
    EXPECT_NEAR(stats.min, 1.0, eps);
    EXPECT_NEAR(stats.max, 3.0, eps);
    EXPECT_NEAR(stats.mean, 2.0, eps);
    EXPECT_NEAR(stats.aspect, 3.0, eps);
    EXPECT_GT(stats.stddev, Real(0));
    EXPECT_GT(stats.cv, Real(0));
}

TEST(Grid1DStats, UniformeFisicaIgnoraVolumesFicticios) {
    constexpr Index N = 4;

    const auto grid = Grid1DBuilder{}
        .setN(N)
        .setDomain(0.0, 1.0)
        .setNVolumesFicticios(2)
        .setDistribution<Uniform1D>()
        .build();

    const auto stored = Grid1DStats::basicFaces(grid);
    const auto physical = Grid1DStats::basicPhysicalFaces(grid);

    EXPECT_EQ(stored.count, static_cast<std::size_t>(N + 4));
    EXPECT_EQ(physical.count, static_cast<std::size_t>(N));
    EXPECT_NEAR(physical.sum, 1.0, eps);
    EXPECT_NEAR(physical.min, 0.25, eps);
    EXPECT_NEAR(physical.max, 0.25, eps);
    EXPECT_NEAR(physical.mean, 0.25, eps);
    EXPECT_NEAR(physical.aspect, 1.0, eps);
    EXPECT_NEAR(physical.cv, 0.0, eps);
}

TEST(Grid1DStats, UniformidadeRelativaDetectaMalhaUniforme) {
    const auto grid = Grid1DBuilder{}
        .setN(5)
        .setDomain(0.0, 1.0)
        .setDistribution<Uniform1D>()
        .build();

    EXPECT_NEAR(Grid1DStats::relativeUniformityPhysicalFaces(grid), 1.0, eps);
    EXPECT_TRUE(Grid1DStats::physicalFacesArePositive(grid));
    EXPECT_TRUE(Grid1DStats::isAlmostUniform(grid.deltasFaces(), Real(1e-12)));
}

TEST(Grid1DStats, RandomFisicaTemAspectoMaiorOuIgualAUm) {
    const auto grid = Grid1DBuilder{}
        .setN(20)
        .setDomain(0.0, 1.0)
        .setCentering(CenteringTag::CellCentered)
        .setDistribution<Random1D>()
        .setRandomPercentual(0.40)
        .setRandomSeed(12345u)
        .build();

    const auto basic = Grid1DStats::basicPhysicalFaces(grid);
    const auto adjacent = Grid1DStats::adjacentRatiosPhysicalFaces(grid);
    const auto smooth = Grid1DStats::smoothnessPhysicalFaces(grid);

    EXPECT_EQ(basic.count, std::size_t{20});
    EXPECT_NEAR(basic.sum, 1.0, eps);
    EXPECT_GT(basic.min, Real(0));
    EXPECT_GE(basic.aspect, Real(1));
    EXPECT_GE(basic.cv, Real(0));
    EXPECT_EQ(adjacent.ratios.size(), std::size_t{19});
    EXPECT_GE(adjacent.worst_sym_ratio, Real(1));
    EXPECT_GE(smooth.max_grad, Real(0));
}

TEST(Grid1DStats, HistogramaContaTodosOsValores) {
    const std::vector<Real> values{0.1, 0.2, 0.3, 0.4};
    const auto histogram = Grid1DStats::histogram(
        std::span<const Real>{values.data(), values.size()},
        2);

    ASSERT_EQ(histogram.counts.size(), std::size_t{2});
    EXPECT_EQ(histogram.counts[0] + histogram.counts[1], values.size());
    EXPECT_GT(histogram.bin_width, Real(0));
}

TEST(Grid1DStats, ExecSerialRetornaMesmoResultadoDaClasseBase) {
    const auto grid = Grid1DBuilder{}
        .setN(4)
        .setDomain(0.0, 1.0)
        .setNVolumesFicticios(1)
        .setDistribution<Uniform1D>()
        .build();

    bool used_parallel = true;
    const auto base = Grid1DStats::basicPhysicalFaces(grid);
    const auto exec = Grid1DStatsExec::basicPhysicalFaces(
        grid,
        ExecPolicy::Serial,
        &used_parallel);

    EXPECT_FALSE(used_parallel);
    EXPECT_EQ(exec.count, base.count);
    EXPECT_NEAR(exec.sum, base.sum, eps);
    EXPECT_NEAR(exec.min, base.min, eps);
    EXPECT_NEAR(exec.max, base.max, eps);
    EXPECT_NEAR(exec.mean, base.mean, eps);
}
