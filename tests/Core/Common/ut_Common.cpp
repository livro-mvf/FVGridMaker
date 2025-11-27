// ----------------------------------------------------------------------------
// File: ut_Common.cpp
// Description: Testes unitários para a infraestrutura Common (Types, Constants, ID).
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// includes c++
// ----------------------------------------------------------------------------
#include <cmath>

// ----------------------------------------------------------------------------
// includes gtest
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

// ----------------------------------------------------------------------------
// includes FVGridMaker
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/Common/constants.hpp>
#include <FVGridMaker/Core/Common/ID.hpp>

// Namespace aliases para facilitar
using namespace FVGridMaker::core;
using namespace FVGridMaker::core::common;

// --- TESTES DE TIPOS E CONSTANTES ---

TEST(CommonTest, TypesSizeAndSign) {
    // Verifica tamanho do Real (padrão double = 8 bytes)
    // Se você compilou com flags para float, ajuste a expectativa ou use sizeof(double)
    EXPECT_GE(sizeof(Real), 4); 
    
    // Verifica se Index é COM sinal (int64_t) conforme especificado
    EXPECT_TRUE(std::is_signed_v<Index>);
    EXPECT_EQ(sizeof(Index), 8); // 64-bit
}

TEST(CommonTest, ConstantsValues) {
    // Verifica PI
    EXPECT_NEAR(PI, 3.141592653589793, 1e-15);
    EXPECT_NEAR(TWO_PI, 6.283185307179586, 1e-15);
    
    // Verifica Tolerâncias
    EXPECT_GT(GEOMETRIC_TOLERANCE, 0.0);
    EXPECT_LT(GEOMETRIC_TOLERANCE, 1e-6);
    
    // Verifica definição de Infinito e NaN
    EXPECT_TRUE(std::isinf(INF_VAL));
    EXPECT_TRUE(std::isnan(QNAN_VAL));
}

// --- TESTES DA CLASSE ID ---

TEST(IDTest, UniquenessAndSequence) {
    // Cria dois IDs sequenciais
    ID id1("Objeto1");
    ID id2("Objeto2");

    EXPECT_NE(id1.uid(), id2.uid()) << "IDs devem ser únicos";
    EXPECT_LT(id1.uid(), id2.uid()) << "IDs devem ser crescentes";
    
    EXPECT_EQ(id1.name(), "Objeto1");
    EXPECT_EQ(id2.name(), "Objeto2");
}

TEST(IDTest, MoveSemantics) {
    // Cenário: Mover a identidade de A para B
    ID id_original("Original");
    Index valor_uid_original = id_original.uid();

    // Move Construction
    ID id_moved(std::move(id_original));

    // 1. O novo objeto deve ter o UID e Nome do original
    EXPECT_EQ(id_moved.uid(), valor_uid_original);
    EXPECT_EQ(id_moved.name(), "Original");
    EXPECT_TRUE(id_moved.has_valid_id());

    // 2. O objeto original deve ficar inválido (zumbi)
    EXPECT_FALSE(id_original.has_valid_id());
    EXPECT_EQ(id_original.uid(), INVALID_INDEX);
}

TEST(IDTest, ThreadSafety) {
    // Cenário: Várias threads criando IDs ao mesmo tempo.
    // Se não for atômico, pode haver IDs duplicados ou race conditions.
    const int num_threads = 10;
    const int ids_per_thread = 100;
    
    std::vector<std::thread> threads;
    std::vector<std::vector<Index>> results(num_threads);

    for(auto i=0; i<num_threads; ++i) {
        threads.emplace_back([&results, i]() {
            for(int j=0; j<ids_per_thread; ++j) {
                ID temp;
                results[static_cast<std::size_t>(i)].push_back(temp.uid());
            }
        });
    }

    for(auto& t : threads) t.join();

    // Verifica se todos os IDs gerados são únicos
    std::set<Index> all_ids;
    for(const auto& batch : results) {
        for(Index uid : batch) {
            bool inserted = all_ids.insert(uid).second;
            EXPECT_TRUE(inserted) << "ID duplicado encontrado: " << uid;
        }
    }
    
    EXPECT_EQ(all_ids.size(), num_threads * ids_per_thread);
}