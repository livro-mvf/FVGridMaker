// ----------------------------------------------------------------------------
// File: ut_Grid1D.cpp
// Description: Testes unitários para a classe de dados Grid1D (API).
//              Verifica armazenamento, acesso via spans e semântica de movimento.
// ----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <vector>
#include <numeric> // std::iota

// Include da classe a ser testada
#include <FVGridMaker/Grid/Grid1D/API/Grid1D.h>

using namespace FVGridMaker::grid::grid1d::api;
using FVGridMaker::core::Real;
using FVGridMaker::core::Index;

// Fixture para criar dados de teste repetíveis
class Grid1DDataTest : public ::testing::Test {
protected:
    // Dados simulados para uma malha de 3 volumes (N=3)
    std::vector<Real> faces   = {0.0, 1.0, 2.0, 3.0}; // N+1
    std::vector<Real> centers = {0.5, 1.5, 2.5};      // N
    std::vector<Real> dF      = {1.0, 1.0, 1.0};      // N
    std::vector<Real> dC      = {0.5, 1.0, 1.0, 0.5}; // N+1 (dist bordas + miolo)
};

TEST_F(Grid1DDataTest, ConstructionAndAccess) {
    // Constrói transferindo os vetores (Move semantics)
    Grid1D grid(std::move(faces), std::move(centers), std::move(dF), std::move(dC));

    // Verifica dimensões
    EXPECT_EQ(grid.nVolumes(), 3);
    EXPECT_EQ(grid.nFaces(), 4);

    // Verifica conteúdo via Spans
    // Faces
    ASSERT_EQ(grid.faces().size(), 4);
    EXPECT_DOUBLE_EQ(grid.faces()[0], 0.0);
    EXPECT_DOUBLE_EQ(grid.faces()[3], 3.0);

    // Centros
    ASSERT_EQ(grid.centers().size(), 3);
    EXPECT_DOUBLE_EQ(grid.centers()[1], 1.5);

    // Deltas
    EXPECT_DOUBLE_EQ(grid.deltasFaces()[0], 1.0);
    EXPECT_DOUBLE_EQ(grid.deltasCenters()[1], 1.0);
}

TEST_F(Grid1DDataTest, IdentityAndMetadata) {
    Grid1D grid(faces, centers, dF, dC); // Cópia dos dados do fixture

    // Verifica herança de ID
    EXPECT_NE(grid.uid(), -1);
    EXPECT_EQ(grid.name(), "Grid1D");

    // Verifica Helpers Geométricos
    EXPECT_DOUBLE_EQ(grid.minCoord(), 0.0);
    EXPECT_DOUBLE_EQ(grid.maxCoord(), 3.0);
    EXPECT_DOUBLE_EQ(grid.length(), 3.0);
}

TEST_F(Grid1DDataTest, MoveSemantics) {
    Grid1D source(std::move(faces), std::move(centers), std::move(dF), std::move(dC));
    auto original_uid = source.uid();

    // Move Constructor
    Grid1D target(std::move(source));

    // Target deve ter os dados
    EXPECT_EQ(target.nVolumes(), 3);
    EXPECT_EQ(target.uid(), original_uid); // ID preservado

    // Source deve estar vazio/inválido (estado pós-movimento)
    EXPECT_EQ(source.nVolumes(), 0);
}

TEST(Grid1DTest, EmptyState) {
    Grid1D empty;
    
    EXPECT_EQ(empty.nVolumes(), 0);
    EXPECT_EQ(empty.nFaces(), 0);
    EXPECT_TRUE(empty.faces().empty());
    
    // Segurança: min/max/length em malha vazia
    EXPECT_DOUBLE_EQ(empty.minCoord(), 0.0);
    EXPECT_DOUBLE_EQ(empty.maxCoord(), 0.0);
    EXPECT_DOUBLE_EQ(empty.length(), 0.0);
}