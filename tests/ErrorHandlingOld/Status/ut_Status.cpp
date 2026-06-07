// ----------------------------------------------------------------------------
// File: ut_Status.cpp
// Project: FVGridMaker
// Version: 1.0
// Description: Testes unitários para Status e StatusOr.
//              Valida o fluxo funcional (retorno de valor ou erro).
// License: GNU GPL v3
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// includes gtest
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>


// ----------------------------------------------------------------------------
// includes FVGridMaker
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/Status.h>
#include <FVGridMaker/ErrorHandling/CoreErrors.h> // Para exemplos de erro

// Namespaces
namespace Err = FVGridMaker::error;
using namespace FVGridMaker::error;

/**
 * @file ut_Status.cpp
 * @brief Validação das classes Status e StatusOr.
 * @ingroup ErrorHandlingTests
 */

// ============================================================================
// TESTES: Status (Básico)
// ============================================================================

TEST(StatusTest, SuccessState) {
    Status st = Status::OK();
    EXPECT_TRUE(st.ok());
    EXPECT_EQ(st.code(), 0);
    EXPECT_TRUE(st.message().empty());
}

TEST(StatusTest, FailureState) {
    // Cria um erro manual
    ErrorRecord rec;
    rec.code = 123;
    rec.message = "Erro teste";
    rec.severity = Severity::Error;

    Status st(rec);
    EXPECT_FALSE(st.ok());
    EXPECT_EQ(st.code(), 123);
    EXPECT_EQ(st.message(), "Erro teste");
}

// ============================================================================
// TESTES: StatusOr (Com Primitive Types)
// ============================================================================

TEST(StatusOrTest, SuccessWithInt) {
    // Construtor implícito de sucesso
    StatusOr<int> res = 42;

    ASSERT_TRUE(res.ok());
    EXPECT_EQ(res.value(), 42);
    // O status interno deve ser OK
    EXPECT_TRUE(res.status().ok());
}

TEST(StatusOrTest, FailureWithInt) {
    // Cria um erro
    ErrorRecord rec;
    rec.code = 500;
    rec.message = "Falha no calculo";
    Status st_err(rec);

    // Construtor de falha
    StatusOr<int> res = st_err;

    ASSERT_FALSE(res.ok());
    EXPECT_EQ(res.status().code(), 500);
    EXPECT_EQ(res.status().message(), "Falha no calculo");

    // Tentar acessar valor deve lançar exceção do std::optional
    EXPECT_THROW(res.value(), std::bad_optional_access);
}

// ============================================================================
// TESTES: StatusOr (Com Complex Types)
// ============================================================================

TEST(StatusOrTest, SuccessWithString) {
    std::string msg = "Hello World";
    StatusOr<std::string> res = msg; // Copia

    ASSERT_TRUE(res.ok());
    EXPECT_EQ(res.value(), "Hello World");
}

TEST(StatusOrTest, MoveSemantics) {
    // Testa se o valor é movido corretamente para dentro do StatusOr
    std::unique_ptr<int> ptr = std::make_unique<int>(99);
    
    // StatusOr<unique_ptr> só funciona se usar move, pois unique_ptr não copia
    StatusOr<std::unique_ptr<int>> res(std::move(ptr));

    ASSERT_TRUE(res.ok());
    ASSERT_NE(res.value(), nullptr);
    EXPECT_EQ(*res.value(), 99);
}