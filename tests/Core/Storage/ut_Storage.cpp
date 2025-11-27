// ----------------------------------------------------------------------------
// File: ut_Storage.cpp
// Project: FVGridMaker
// Description: Testes unitários para a classe Storage (Core).
//              Verifica alocação SoA, redimensionamento e consistência de tipos.
// ----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <type_traits> // Para std::is_same_v
#include <FVGridMaker/Grid/Common/Storage.hpp>

// Facilitadores de Namespace para o Teste
// Mapeia FVGridMaker::core para 'Core' para brevidade no teste
namespace FVG = FVGridMaker;
using namespace FVGridMaker::core;

TEST(StorageTest, Initialization) {
    // Instanciação usando o namespace correto
    Storage store;
    
    // Verifica estado inicial vazio
    EXPECT_EQ(store.size(), 0);
    EXPECT_TRUE(store.centers_x.empty());
    EXPECT_TRUE(store.centers_y.empty());
    EXPECT_TRUE(store.volumes.empty());
}

TEST(StorageTest, ResizeAndMemoryConsistency) {
    Storage store;
    // Uso do tipo 'Size' definido pela biblioteca (geralmente std::size_t)
    Size num_cells = 1000;

    // Ação: Redimensionar
    store.resize(num_cells);

    // Verificação 1: Tamanhos corretos em todos os vetores SoA
    EXPECT_EQ(store.size(), num_cells);
    EXPECT_EQ(store.centers_x.size(), num_cells);
    EXPECT_EQ(store.centers_y.size(), num_cells);
    EXPECT_EQ(store.volumes.size(), num_cells);
    
    // Verificação 2: Métricas auxiliares também devem ser alocadas
    EXPECT_EQ(store.dist_center_east.size(), num_cells);
    EXPECT_EQ(store.area_north.size(), num_cells);
    EXPECT_EQ(store.face_east_x.size(), num_cells);

    // Ação: Escrita de dados (Simulando uso SoA)
    // Usamos static_cast<Real> para garantir que estamos testando com a precisão configurada (float/double)
    for(Size i = 0; i < num_cells; ++i) {
        Real val = static_cast<Real>(i) * static_cast<Real>(0.1);
        store.centers_x[i] = val;
        store.volumes[i]   = static_cast<Real>(1.0);
    }

    // Verificação 3: Integridade dos dados
    // Utilizamos 'if constexpr' para aplicar a asserção correta dependendo da precisão compilada
    if constexpr (std::is_same_v<Real, double>) {
        EXPECT_DOUBLE_EQ(store.centers_x[50], 5.0);
    } 
    else if constexpr (std::is_same_v<Real, float>) {
        EXPECT_FLOAT_EQ(store.centers_x[50], 5.0f);
    }
    else {
        // Fallback para long double ou outros tipos
        EXPECT_NEAR(store.centers_x[50], static_cast<Real>(5.0), static_cast<Real>(1e-9));
    }
    
    EXPECT_EQ(store.volumes[999], static_cast<Real>(1.0));
}

TEST(StorageTest, Clear) {
    Storage store;
    store.resize(500);
    EXPECT_EQ(store.size(), 500);

    // Ação: Limpar memória
    store.clear();
    
    // Verificação: Deve zerar o tamanho lógico
    EXPECT_EQ(store.size(), 0);
    EXPECT_TRUE(store.centers_x.empty());
    EXPECT_TRUE(store.area_east.empty());
    
    // Nota: O método clear() de std::vector define size=0, mas não garante liberação de capacidade (capacity).
    // Isso é comportamento esperado para evitar realocações frequentes.
}