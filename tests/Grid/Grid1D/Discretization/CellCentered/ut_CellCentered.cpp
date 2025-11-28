// ----------------------------------------------------------------------------
// File: ut_CellCentered.cpp
// Project: FVGridMaker
// Description: Testes unitários para a política de discretização CellCentered.
//              Verifica topologia, mapeamento de índices e integração com geometria.
// ----------------------------------------------------------------------------

#include <gtest/gtest.h>

// Includes das classes necessárias
// Certifique-se de que os caminhos estão corretos conforme sua estrutura
#include <FVGridMaker/Grid/Discretization/CellCentered.hpp>
#include <FVGridMaker/Grid/Geometry/Cartesian.hpp>
#include <FVGridMaker/Grid/Mesh.hpp>

// Facilitadores de Namespace
namespace FVG = FVGridMaker;

// Traz os componentes para o escopo global do teste
using namespace FVG::grid::discretization; // Para CellCentered
using namespace FVG::grid::geometry;       // Para Cartesian

// Tipos fundamentais
using FVG::core::Size;
using FVG::core::Index;
using FVG::core::Real;

// ============================================================================
// Testes Estáticos (Lógica Topológica)
// ============================================================================

TEST(CellCenteredTest, RequiredSizeCalculation) {
    Size ni = 10;
    Size nj = 10;
    Size expected_size = 144; // (10+2)*(10+2)
    
    EXPECT_EQ(CellCentered::requiredSize(ni, nj), expected_size);
    EXPECT_EQ(CellCentered::requiredSize(100, 50), static_cast<Size>(5304));
}

TEST(CellCenteredTest, IndexMapping) {
    Size ni = 3;
    Size nj = 3;
    
    // (-1, -1) -> 0
    EXPECT_EQ(CellCentered::getIndex(-1, -1, ni, nj), static_cast<Size>(0));
    
    // (0, 0) -> 6
    EXPECT_EQ(CellCentered::getIndex(0, 0, ni, nj), static_cast<Size>(6));
    
    // (2, 2) -> 18
    EXPECT_EQ(CellCentered::getIndex(2, 2, ni, nj), static_cast<Size>(18));
    
    // (3, 3) -> 24
    EXPECT_EQ(CellCentered::getIndex(3, 3, ni, nj), static_cast<Size>(24));
}

// ============================================================================
// Teste de Integração / Cálculo de Métricas
// ============================================================================

TEST(CellCenteredTest, MetricsPopulation_2x2) {
    // Definição do Tipo de Malha DENTRO do teste para garantir escopo local.
    // CORREÇÃO: Usamos FVGridMaker::core::Mesh para corresponder à definição no Mesh.hpp enviado.
    using TestMeshType = FVGridMaker::core::Mesh<
        FVG::grid::geometry::Cartesian, 
        FVG::grid::discretization::CellCentered
    >;

    // Configuração
    Size ni = 2;
    Size nj = 2;
    
    // Instanciação
    TestMeshType mesh(ni, nj);
    
    // Execução (CORREÇÃO: 'mesh' em vez de 'mes')
    mesh.computeGeometry(); 
    
    // --- Validação ---
    
    // 1. Célula Real (0, 0)
    // dx=0.5, dy=0.5 -> Centro deve ser (0.25, 0.25)
    Size idx_00 = mesh.index(0, 0);
    EXPECT_DOUBLE_EQ(static_cast<double>(mesh.centers_x[idx_00]), 0.25);
    EXPECT_DOUBLE_EQ(static_cast<double>(mesh.centers_y[idx_00]), 0.25);
    EXPECT_DOUBLE_EQ(static_cast<double>(mesh.volumes[idx_00]), 0.25);
    
    // 2. Distâncias
    EXPECT_DOUBLE_EQ(static_cast<double>(mesh.dist_center_east[idx_00]), 0.5);
    EXPECT_DOUBLE_EQ(static_cast<double>(mesh.dist_center_north[idx_00]), 0.5);
    
    // 3. Ghost Cell Oeste (-1, 0) -> x = -0.25
    Size idx_ghost_w = mesh.index(-1, 0);
    EXPECT_DOUBLE_EQ(static_cast<double>(mesh.centers_x[idx_ghost_w]), -0.25);
    
    // 4. Face Leste da célula (0,0) -> x = 0.5
    EXPECT_DOUBLE_EQ(static_cast<double>(mesh.face_east_x[idx_00]), 0.5);
}