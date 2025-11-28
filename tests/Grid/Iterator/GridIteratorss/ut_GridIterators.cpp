// ----------------------------------------------------------------------------
// File: ut_GridIterators.cpp
// Project: FVGridMaker
// Description: Testes unitários para os Iteradores da Malha.
// ----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <vector>

// Includes
// Ajuste os caminhos conforme a estrutura do seu projeto
#include <FVGridMaker/Grid/Mesh.hpp>
#include <FVGridMaker/Grid/Geometry/Cartesian.hpp>
#include <FVGridMaker/Grid/Discretization/CellCentered.hpp>
#include <FVGridMaker/Grid/Iterators/GridIterators.hpp>

// Facilitadores de namespace
namespace FVG = FVGridMaker;
using namespace FVG::grid;
using namespace FVG::grid::iterators;

// Definição da malha para teste (Mesh está no Core)
using TestMesh = FVG::core::Mesh<
    geometry::Cartesian, 
    discretization::CellCentered
>;

TEST(IteratorTest, InternalCellsCount) {
    // Malha 3x3
    // Indices reais: (0,0) a (2,2) -> 9 células
    FVG::core::Size ni = 3;
    FVG::core::Size nj = 3;
    TestMesh mesh(ni, nj);

    int count = 0;
    
    // Itera sobre células internas
    for (auto idx : InternalCells(mesh)) {
        count++;
        // Validação básica: índice deve estar dentro da capacidade alocada
        EXPECT_LT(idx, mesh.size());
    }

    // Deve ter percorrido ni * nj = 9 células
    EXPECT_EQ(count, 9);
}

TEST(IteratorTest, InternalCellsIndices) {
    // Malha 2x2 com 1 camada fantasma
    // Stride (Largura Total) = 2 + 2 = 4
    // Offset inicial (0,0) na memória = 1*4 + 1 = 5
    TestMesh mesh(2, 2);
    
    std::vector<FVG::core::Size> visited_indices;
    
    for (auto idx : InternalCells(mesh)) {
        visited_indices.push_back(idx);
    }

    // Verificação manual dos índices esperados:
    // (0,0) -> 5
    // (1,0) -> 6
    // (0,1) -> 9
    // (1,1) -> 10
    
    ASSERT_EQ(visited_indices.size(), 4);
    EXPECT_EQ(visited_indices[0], 5);
    EXPECT_EQ(visited_indices[1], 6);
    EXPECT_EQ(visited_indices[2], 9);
    EXPECT_EQ(visited_indices[3], 10);
}

TEST(IteratorTest, AllCellsCount) {
    // Malha 2x2 com 1 camada de ghost
    // Dimensão total lógica: (2+2) x (2+2) = 16 células
    TestMesh mesh(2, 2);
    
    int count = 0;
    for (auto idx : AllCells(mesh)) {
        // cast para void para evitar warning de variável não usada se necessário
        (void)idx; 
        count++;
    }
    
    EXPECT_EQ(count, 16);
}