// ----------------------------------------------------------------------------
// File: ut_Mesh.cpp
// Project: FVGridMaker
// Description: Testes unitários para a classe template Mesh.
//              Utiliza políticas Mock para validar a orquestração e alocação
//              sem depender de implementações reais de física.
// ----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <FVGridMaker/Grid/Mesh.hpp>

// Facilitadores de namespace para o teste
using namespace FVGridMaker::core;
using FVGridMaker::core::Size;
using FVGridMaker::core::Index;
using FVGridMaker::core::Real;

// ============================================================================
// 1. Mocks (Políticas Simuladas)
// ============================================================================

/**
 * @brief Mock de Sistema de Coordenadas.
 * Não precisa de implementação para este teste estrutural.
 */
struct MockCoordSys {};

/**
 * @brief Mock de Política de Discretização.
 * Simula um comportamento simples (sem ghost cells) para verificar se a Mesh
 * delega as chamadas corretamente.
 */
struct MockDiscretization {
    
    // Simula a requisição de tamanho: apenas células reais (ni * nj)
    static Size requiredSize(Size ni, Size nj) {
        return ni * nj;
    }

    // Simula o mapeamento de índice: Linear Row-Major simples
    // Correção: Garantimos que a aritmética é feita com tipos compatíveis para evitar warnings de conversão de sinal
    static Size getIndex(Index i, Index j, Size ni, Size /*nj*/) {
        // Converte índices (que podem ser negativos em ghost cells reais, mas aqui são positivos)
        // para Size antes de calcular o deslocamento de memória.
        return static_cast<Size>(j) * ni + static_cast<Size>(i);
    }

    // Simula o cálculo de métricas: Preenche com valores previsíveis para asserção
    template <typename MeshType>
    static void calculateMetrics(MeshType& mesh, Size ni, Size nj) {
        // Usamos Index para o loop (para permitir decréscimo se necessário), 
        // mas convertemos para Size ao chamar métodos que esperam Size.
        for (Index j = 0; j < static_cast<Index>(nj); ++j) {
            for (Index i = 0; i < static_cast<Index>(ni); ++i) {
                Size idx = getIndex(i, j, ni, nj);
                
                // Preenche com dados dummy para verificar se a Mesh guardou
                mesh.volumes[idx]   = static_cast<Real>(1.0);       // Volume unitário
                mesh.centers_x[idx] = static_cast<Real>(i);         // X = índice i
                mesh.centers_y[idx] = static_cast<Real>(j);         // Y = índice j
            }
        }
    }
};

// ============================================================================
// 2. Casos de Teste
// ============================================================================

// Instanciação da classe Mesh com os Mocks
using TestMeshType = Mesh<MockCoordSys, MockDiscretization>;

TEST(MeshTest, ConstructorAllocation) {
    Size ni = 10;
    Size nj = 20;

    // Ação: Instanciar a malha
    // O construtor deve chamar MockDiscretization::requiredSize e Storage::resize
    TestMeshType mesh(ni, nj);

    // Verificação 1: Metadados topológicos
    EXPECT_EQ(mesh.ni, ni);
    EXPECT_EQ(mesh.nj, nj);

    // Verificação 2: Alocação de memória (Storage herdado)
    // O mock pede ni*nj = 200 células
    EXPECT_EQ(mesh.size(), 200); 
    EXPECT_EQ(mesh.volumes.size(), 200);
    EXPECT_EQ(mesh.centers_x.size(), 200);
}

TEST(MeshTest, ComputeGeometryDelegation) {
    Size ni = 5;
    Size nj = 5;
    TestMeshType mesh(ni, nj);

    // Estado inicial (vetores zerados pelo resize)
    EXPECT_DOUBLE_EQ(static_cast<double>(mesh.volume(0, 0)), 0.0);

    // Ação: Calcular geometria
    // Deve delegar para MockDiscretization::calculateMetrics
    mesh.computeGeometry();

    // Verificação: Os dados foram preenchidos conforme a lógica do Mock?
    // Mock diz: volume = 1.0, x = i, y = j
    
    // Célula (2, 3) -> i=2, j=3
    // Como MockCoordSys usa tipos Index (com sinal), passamos literais compatíveis ou cast
    EXPECT_DOUBLE_EQ(static_cast<double>(mesh.volume(2, 3)), 1.0);
    
    // Acesso direto aos vetores (via Storage) para confirmar
    // Índice linear: j * ni + i = 3 * 5 + 2 = 17
    Size linear_idx = 17;
    EXPECT_DOUBLE_EQ(static_cast<double>(mesh.centers_x[linear_idx]), 2.0);
    EXPECT_DOUBLE_EQ(static_cast<double>(mesh.centers_y[linear_idx]), 3.0);
}

TEST(MeshTest, IndexingWrapper) {
    Size ni = 4;
    Size nj = 4;
    TestMeshType mesh(ni, nj);

    // Testa se o método mesh.index() chama corretamente a política
    // Mock: j * ni + i
    // (1, 2) -> 2 * 4 + 1 = 9
    EXPECT_EQ(mesh.index(1, 2), 9);
    EXPECT_EQ(mesh.index(3, 3), 15);
    EXPECT_EQ(mesh.index(0, 0), 0);
}

TEST(MeshTest, VolumeAccessWrapper) {
    Size ni = 2;
    Size nj = 2;
    TestMeshType mesh(ni, nj);
    
    // Preenche manualmente para testar o getter
    mesh.computeGeometry(); // Define volumes como 1.0
    
    // Altera um valor específico manualmente no vetor bruto
    Size idx = mesh.index(1, 1);
    mesh.volumes[idx] = 5.5;

    // Verifica se mesh.volume(i, j) retorna o valor correto
    EXPECT_DOUBLE_EQ(static_cast<double>(mesh.volume(1, 1)), 5.5);
    EXPECT_DOUBLE_EQ(static_cast<double>(mesh.volume(0, 0)), 1.0); // O resto continua 1.0
}