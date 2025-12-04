// ----------------------------------------------------------------------------
// File: Types.hpp
// Project: FVGridMaker
// Version: 1.2
// Description: Definição de tipos fundamentais com suporte a configuração
//              de precisão em tempo de compilação via macros.
// Last modified: 2025-11-25
// Author: FVGridMaker Team
// License: GNU GPL v3
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// includes C++
// ----------------------------------------------------------------------------
#include <cstddef>    // std::size_t
#include <cstdint>    // std::int64_t, std::uint8_t

// ----------------------------------------------------------------------------
// includes FVMGridMaker
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/Common/namespace.hpp>

/**
 * @file types.hpp
 * @brief Tipos primitivos fundamentais configuráveis da biblioteca.
 * @ingroup Core
 */



// ----------------------------------------------------------------------------
// Tipo de Ponto Flutuante (Real)
// ----------------------------------------------------------------------------

FVGRIDMAKER_NAMESPACE_OPEN
CORE_NAMESPACE_OPEN

// ----------------------------------------------------------------------------
// Tipo de Ponto Flutuante (Real)
// ----------------------------------------------------------------------------

/// @brief Tipo de ponto flutuante padrão da biblioteca.
/// @details A precisão pode ser alterada definindo macros antes de incluir
///          este arquivo (geralmente via CMake `-DFVG_REAL_IS_FLOAT=ON`).
///
/// - Padrão: `double` (64-bit) – recomendado para CFD.
/// - Opção 1: `float` (32-bit) – menor uso de memória, menor precisão.
/// - Opção 2: `long double` – maior precisão (dependente da arquitetura).
#if defined(FVG_REAL_IS_FLOAT)
    using Real = float;
#elif defined(FVG_REAL_IS_LONG_DOUBLE)
    using Real = long double;
#else
    using Real = double;
#endif

// ----------------------------------------------------------------------------
// Tipos Inteiros e Tamanho
// ----------------------------------------------------------------------------

/**
 * @typedef Index
 * @brief Tipo inteiro para indexação de malhas e contadores de loops.
 *
 * @note Optou-se por `std::int64_t` (com sinal) ao invés de `size_t` por:
 * 1. Segurança em loops decrescentes (`for (Index i = N-1; i >= 0; --i)`).
 * 2. Aritmética de estêncil (`i-1` não causa underflow catastrófico).
 * 3. Compatibilidade com bibliotecas HPC (PETSc, BLAS/LAPACK usam inteiros com sinal).
 */
using Index = std::int64_t;

/**
 * @typedef Size
 * @brief Tipo para tamanhos de buffers de memória e alocação.
 * Compatível nativamente com `std::vector::size()`.
 */
using Size = std::size_t;

/**
 * @typedef Byte
 * @brief Tipo para manipulação de dados brutos (raw memory) ou serialização.
 */
using Byte = std::uint8_t;

CORE_NAMESPACE_CLOSE
FVGRIDMAKER_NAMESPACE_CLOSE