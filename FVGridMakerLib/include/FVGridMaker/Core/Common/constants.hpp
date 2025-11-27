// ----------------------------------------------------------------------------
// File: Constants.hpp
// Project: FVGridMaker
// Version: 1.2
// Description: Constantes matemáticas e tolerâncias numéricas globais.
//              Centraliza valores mágicos para garantir consistência em validações.
// Last modified: 2025-11-25
// Author: FVGridMaker Team
// License: GNU GPL v3
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// includes C++
// ----------------------------------------------------------------------------
#include <limits>
#include <numbers> // C++20

// ----------------------------------------------------------------------------
// includes FVMGridMaker
// ----------------------------------------------------------------------------
// #include <FVGridMaker/Core/Common/namespace.hpp>
#include <FVGridMaker/Core/Common/types.hpp>

/**
 * @file Constants.hpp
 * @brief Constantes globais (Matemática, Física, Tolerâncias).
 * @ingroup Core
 */

FVGRIDMAKER_NAMESPACE_OPEN
CORE_NAMESPACE_OPEN
COMMON_NAMESPACE_OPEN

// ----------------------------------------------------------------------------
// Constantes Matemáticas
// ----------------------------------------------------------------------------

/** @brief Valor de Pi com a precisão do tipo Real. */
constexpr Real PI = std::numbers::pi_v<Real>;

/** @brief Valor de 2*Pi. */
constexpr Real TWO_PI = 2.0 * PI;

// ----------------------------------------------------------------------------
// Limites e Tolerâncias Numéricas
// ----------------------------------------------------------------------------

/** * @brief Epsilon da máquina para o tipo Real. 
 * Menor valor tal que (1.0 + eps != 1.0).
 */
constexpr Real MACHINE_EPSILON = std::numeric_limits<Real>::epsilon();

/** * @brief Tolerância absoluta para comparações geométricas.
 * Usada para verificar se dois pontos são coincidentes ou se uma face é plana.
 * Valor padrão: 1e-12 (conforme Manual RNF06).
 */
constexpr Real GEOMETRIC_TOLERANCE = static_cast<Real>(1.0e-12);

/** * @brief Limite seguro para evitar divisão por zero. 
 * (Substitui o antigo kLimit)
 */
constexpr Real SMALL_NUMBER = static_cast<Real>(1.0e-30);

/** * @brief Valor representando infinito positivo.
 */
constexpr Real INF_VAL = std::numeric_limits<Real>::infinity();

/** * @brief Valor representando "Not a Number" (Quiet NaN).
 * Útil para inicializar variáveis que devem ser obrigatoriamente calculadas.
 */
constexpr Real QNAN_VAL = std::numeric_limits<Real>::quiet_NaN();

// ----------------------------------------------------------------------------
// Constantes de Indexação
// ----------------------------------------------------------------------------

/** * @brief Valor sentinela para índices inválidos ou não encontrados.
 * Equivalente a std::string::npos ou -1.
 */
constexpr Index INVALID_INDEX = static_cast<Index>(-1);

COMMON_NAMESPACE_CLOSE
CORE_NAMESPACE_CLOSE
FVGRIDMAKER_NAMESPACE_CLOSE