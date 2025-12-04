// ----------------------------------------------------------------------------
// File: Tags1D.hpp
// Project: FVGridMaker
// Version: 1.4
// Description: Define as enumerações `CenteringTag` e `DistributionTag` para
//              configuração de malhas 1D. Utiliza X-Macros para gerar
//              automaticamente as conversões para string (to_string).
// Last modified: 2025-11-25
// Author: FVGridMaker Team
// License: GNU GPL v3
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// includes C++
// ----------------------------------------------------------------------------
#include <cstdint>
#include <string_view>

// ----------------------------------------------------------------------------
// includes FVMGridMaker
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/Common/namespace.hpp>

/**
 * @file Tags1D.hpp
 * @brief Identificadores (Tags) para estratégias de Malha 1D.
 *
 * Este arquivo define:
 * 1. `CenteringTag`: Onde as variáveis são armazenadas (Face vs Célula).
 * 2. `DistributionTag`: Qual o padrão de espaçamento (Uniforme, Aleatório, etc.).
 *
 * @note A implementação usa X-Macros para permitir que novos tipos sejam
 * adicionados facilmente e para gerar nomes de string automaticamente.
 *
 * @ingroup Grid
 */

FVGRIDMAKER_NAMESPACE_OPEN
GRID_NAMESPACE_OPEN

// ----------------------------------------------------------------------------
// Definição das Listas (X-Macros)
// ----------------------------------------------------------------------------

// Se o usuário não definiu uma lista customizada antes do include, usa o padrão.

#ifndef FVMG_CENTERINGS
  /**
   * @brief Lista mestre de tipos de centralização.
   * Expande para: X(FaceCentered) X(CellCentered)
   */
  #define FVMG_CENTERINGS(X) \
      X(FaceCentered)        \
      X(CellCentered)
#endif

#ifndef FVMG_DISTRIBUTIONS
  /**
   * @brief Lista mestre de distribuições padrão (Presets).
   * Expande para: X(Uniform1D) X(Random1D)
   */
  #define FVMG_DISTRIBUTIONS(X) \
      X(Uniform1D)              \
      X(Random1D)
#endif

// ----------------------------------------------------------------------------
// CenteringTag
// ----------------------------------------------------------------------------

/**
 * @enum CenteringTag
 * @brief Define a topologia de armazenamento de variáveis.
 */
enum class CenteringTag : std::uint8_t {
  #define FVMG_ENUM_ELEM(name) name,
    FVMG_CENTERINGS(FVMG_ENUM_ELEM)
  #undef  FVMG_ENUM_ELEM
    Count ///< Número total de tags definidas.
};

// Gera array de nomes estático
#define FVMG_NAME_ELEM(name) #name,
inline constexpr const char* kCenteringNames[] = {
    FVMG_CENTERINGS(FVMG_NAME_ELEM)
};
#undef FVMG_NAME_ELEM

/**
 * @brief Converte uma CenteringTag para sua representação em string.
 * @param t A tag a ser convertida.
 * @return std::string_view O nome da tag (ex: "FaceCentered") ou "Unknown".
 */
inline constexpr std::string_view to_string(CenteringTag t) noexcept {
    const auto idx = static_cast<std::uint8_t>(t);
    if (idx < static_cast<std::uint8_t>(CenteringTag::Count)) {
        return std::string_view{kCenteringNames[idx]};
    }
    return std::string_view{"Unknown"};
}

// Garante integridade entre enum e array de nomes
static_assert(
    static_cast<std::size_t>(CenteringTag::Count) ==
    (sizeof(kCenteringNames) / sizeof(kCenteringNames[0])),
    "Erro interno: O tamanho de kCenteringNames não corresponde a CenteringTag::Count"
);

// ----------------------------------------------------------------------------
// DistributionTag
// ----------------------------------------------------------------------------

/**
 * @enum DistributionTag
 * @brief Identifica padrões de distribuição pré-definidos.
 *
 * Útil para configurações via arquivo ou CLI, onde o usuário escolhe
 * "Uniform1D" ou "Random1D" por nome/enum.
 */
enum class DistributionTag : std::uint8_t {
  #define FVMG_ENUM_ELEM(name) name,
    FVMG_DISTRIBUTIONS(FVMG_ENUM_ELEM)
  #undef  FVMG_ENUM_ELEM
    Count ///< Número total de tags definidas.
};

// Gera array de nomes estático
#define FVMG_NAME_ELEM(name) #name,
inline constexpr const char* kDistributionNames[] = {
    FVMG_DISTRIBUTIONS(FVMG_NAME_ELEM)
};
#undef FVMG_NAME_ELEM

/**
 * @brief Converte uma DistributionTag para sua representação em string.
 * @param t A tag a ser convertida.
 * @return std::string_view O nome da tag (ex: "Uniform1D") ou "Unknown".
 */
inline constexpr std::string_view to_string(DistributionTag t) noexcept {
    const auto idx = static_cast<std::uint8_t>(t);
    if (idx < static_cast<std::uint8_t>(DistributionTag::Count)) {
        return std::string_view{kDistributionNames[idx]};
    }
    return std::string_view{"Unknown"};
}

static_assert(
    static_cast<std::size_t>(DistributionTag::Count) ==
    (sizeof(kDistributionNames) / sizeof(kDistributionNames[0])),
    "Erro interno: O tamanho de kDistributionNames não corresponde a DistributionTag::Count"
);

GRID_NAMESPACE_CLOSE
FVGRIDMAKER_NAMESPACE_CLOSE