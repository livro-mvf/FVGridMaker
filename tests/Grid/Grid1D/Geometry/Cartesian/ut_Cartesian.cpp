// ----------------------------------------------------------------------------
// File: ut_Cartesian.cpp
// Project: FVGridMaker
// Description: Testes unitários para a política de geometria Cartesian.
//              Verifica transformações de coordenadas e métricas.
// ----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <type_traits> // Para std::is_same_v

// Ajuste o include conforme a localização exata do arquivo no seu projeto
#include <FVGridMaker/Grid/Geometry/Cartesian.hpp>

// Facilitadores de Namespace
namespace FVG = FVGridMaker;
using namespace FVG::grid::geometry;
using FVG::core::Real;

// ============================================================================
// Testes
// ============================================================================

TEST(CartesianTest, CoordinateTransformationX) {
    // Cenário: Domínio X de [0.0 a 10.0] com espaçamento dx = 2.0
    Real origin = static_cast<Real>(0.0);
    Real spacing = static_cast<Real>(2.0); 

    // 1. Início do domínio (i=0)
    Real x_start = Cartesian::getX(static_cast<Real>(0.0), spacing, origin);
    EXPECT_DOUBLE_EQ(static_cast<double>(x_start), 0.0);
    
    // 2. Primeiro nó (i=1) -> deve ser 2.0
    Real x_node1 = Cartesian::getX(static_cast<Real>(1.0), spacing, origin);
    EXPECT_DOUBLE_EQ(static_cast<double>(x_node1), 2.0);
    
    // 3. Centróide da primeira célula (i=0.5) -> deve ser 1.0
    Real x_center0 = Cartesian::getX(static_cast<Real>(0.5), spacing, origin);
    EXPECT_DOUBLE_EQ(static_cast<double>(x_center0), 1.0);
    
    // 4. Ponto arbitrário (i=2.5) -> 0 + 2.5 * 2 = 5.0
    Real x_arb = Cartesian::getX(static_cast<Real>(2.5), spacing, origin);
    EXPECT_DOUBLE_EQ(static_cast<double>(x_arb), 5.0);
}

TEST(CartesianTest, CoordinateTransformationY) {
    // Cenário: Domínio Y de [10.0 a 20.0] com espaçamento dy = 0.5
    Real origin = static_cast<Real>(10.0);
    Real spacing = static_cast<Real>(0.5);

    // 1. Início (j=0)
    EXPECT_DOUBLE_EQ(static_cast<double>(Cartesian::getY(static_cast<Real>(0.0), spacing, origin)), 10.0);

    // 2. Décimo nó (j=10) -> 10.0 + 10 * 0.5 = 15.0
    EXPECT_DOUBLE_EQ(static_cast<double>(Cartesian::getY(static_cast<Real>(10.0), spacing, origin)), 15.0);
}

TEST(CartesianTest, AreaCalculation) {
    Real dx = static_cast<Real>(0.5);
    Real dy = static_cast<Real>(4.0);
    
    // Área retangular = dx * dy = 2.0
    Real expected_area = static_cast<Real>(2.0);
    Real calculated_area = Cartesian::area(dx, dy);
    
    if constexpr (std::is_same_v<Real, double>) {
        EXPECT_DOUBLE_EQ(calculated_area, expected_area);
    } else {
        EXPECT_FLOAT_EQ(calculated_area, expected_area);
    }
}

TEST(CartesianTest, DistanceCalculation) {
    // Triângulo 3-4-5 Clássico
    Real x1 = static_cast<Real>(0.0);
    Real y1 = static_cast<Real>(0.0);
    Real x2 = static_cast<Real>(3.0);
    Real y2 = static_cast<Real>(4.0);
    
    Real dist = Cartesian::distance(x1, y1, x2, y2);
    
    // Verifica Pitágoras
    EXPECT_DOUBLE_EQ(static_cast<double>(dist), 5.0);
    
    // Distância alinhada ao eixo X (y1 == y2)
    Real dist_aligned = Cartesian::distance(static_cast<Real>(1.0), static_cast<Real>(1.0), 
                                            static_cast<Real>(3.0), static_cast<Real>(1.0));
    EXPECT_DOUBLE_EQ(static_cast<double>(dist_aligned), 2.0);
}

TEST(CartesianTest, DefaultConfig) {
    Cartesian::Config config;
    
    // Verifica valores padrão da struct Config
    EXPECT_DOUBLE_EQ(static_cast<double>(config.x_min), 0.0);
    EXPECT_DOUBLE_EQ(static_cast<double>(config.x_max), 1.0);
    EXPECT_DOUBLE_EQ(static_cast<double>(config.y_min), 0.0);
    EXPECT_DOUBLE_EQ(static_cast<double>(config.y_max), 1.0);
}