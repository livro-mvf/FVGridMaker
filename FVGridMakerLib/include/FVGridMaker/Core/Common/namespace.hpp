// ----------------------------------------------------------------------------
// File: namespace.hpp
// Project: FVGridMaker
// Version: 1.3
// Description: Definição centralizada de macros de namespace para garantir
//              consistência estrutural, reduzir boilerplate e facilitar
//              a refatoração de escopos em toda a biblioteca.
// Last modified: 2025-11-25
// Author: FVGridMaker Team
// License: GNU GPL v3
// ----------------------------------------------------------------------------

#pragma once

/**
 * @file Namespace.hpp
 * @brief Macros de gerenciamento de escopo (Namespaces).
 *
 * Este arquivo define macros para abrir e fechar os namespaces utilizados
 * na biblioteca FVGridMaker. O uso destas macros é obrigatório para manter
 * a consistência e facilitar alterações futuras na hierarquia de nomes.
 *
 * A hierarquia base é:
 * `FVGridMaker::[Modulo]::[SubModulo]`
 *
 * @ingroup Core
 */

// ----------------------------------------------------------------------------
// Namespace Raiz
// ----------------------------------------------------------------------------

/// Abre o namespace raiz `FVGridMaker`.
#define FVGRIDMAKER_NAMESPACE_OPEN namespace FVGridMaker {
/// Fecha o namespace raiz `FVGridMaker`.
#define FVGRIDMAKER_NAMESPACE_CLOSE }

// ----------------------------------------------------------------------------
// Namespaces de Módulos Principais (Nível 1)
// ----------------------------------------------------------------------------

/// Abre o namespace `core`.
#define CORE_NAMESPACE_OPEN namespace core {
/// Fecha o namespace `core`.
#define CORE_NAMESPACE_CLOSE }

/// Abre o namespace `grid`.
#define GRID_NAMESPACE_OPEN namespace grid {
/// Fecha o namespace `grid`.
#define GRID_NAMESPACE_CLOSE }

/// Abre o namespace `error`.
#define ERROR_NAMESPACE_OPEN namespace error {
/// Fecha o namespace `error`.
#define ERROR_NAMESPACE_CLOSE }

// ----------------------------------------------------------------------------
// Namespaces de Sub-módulos (Nível 2+)
// ----------------------------------------------------------------------------

/// Abre o namespace `common` (dentro de core).
#define COMMON_NAMESPACE_OPEN namespace common {
/// Fecha o namespace `common`.
#define COMMON_NAMESPACE_CLOSE }

/// Abre o namespace `grid1d` (dentro de grid).
#define GRID1D_NAMESPACE_OPEN namespace grid1d {
/// Fecha o namespace `grid1d`.
#define GRID1D_NAMESPACE_CLOSE }

/// Abre o namespace `api`.
#define API_NAMESPACE_OPEN namespace api {
/// Fecha o namespace `api`.
#define API_NAMESPACE_CLOSE }

/// Abre o namespace `builders`.
#define BUILDERS_NAMESPACE_OPEN namespace builders {
/// Fecha o namespace `builders`.
#define BUILDERS_NAMESPACE_CLOSE }

/// Abre o namespace `patterns`.
#define PATTERNS_NAMESPACE_OPEN namespace patterns {
/// Fecha o namespace `patterns`.
#define PATTERNS_NAMESPACE_CLOSE }

/// Abre o namespace `distribution` (dentro de patterns).
#define DISTRIBUTION_NAMESPACE_OPEN namespace distribution {
/// Fecha o namespace `distribution`.
#define DISTRIBUTION_NAMESPACE_CLOSE }

/// Abre o namespace `centering` (dentro de patterns).
#define CENTERING_NAMESPACE_OPEN namespace centering {
/// Fecha o namespace `centering`.
#define CENTERING_NAMESPACE_CLOSE }

/// Abre o namespace `utils`.
#define UTILS_NAMESPACE_OPEN namespace utils {
/// Fecha o namespace `utils`.
#define UTILS_NAMESPACE_CLOSE }

/// Abre o namespace `detail` (detalhes de implementação, não parte da API pública).
#define DETAIL_NAMESPACE_OPEN namespace detail {
/// Fecha o namespace `detail`.
#define DETAIL_NAMESPACE_CLOSE }

// ----------------------------------------------------------------------------
// Macros Compostas (Atalhos para Implementação)
// ----------------------------------------------------------------------------

/**
 * @brief Abre a hierarquia para o namespace base do Grid1D.
 * Escopo: `FVGridMaker::grid::grid1d`
 */
#define FVG_GRID1D_OPEN \
    FVGRIDMAKER_NAMESPACE_OPEN \
    GRID_NAMESPACE_OPEN \
    GRID1D_NAMESPACE_OPEN

#define FVG_GRID1D_CLOSE \
    GRID1D_NAMESPACE_CLOSE \
    GRID_NAMESPACE_CLOSE \
    FVGRIDMAKER_NAMESPACE_CLOSE

/**
 * @brief Abre a hierarquia completa para a API do Grid1D (Classes de Dados).
 * Escopo: `FVGridMaker::grid::grid1d::api`
 */
#define FVG_GRID1D_API_OPEN \
    FVGRIDMAKER_NAMESPACE_OPEN \
    GRID_NAMESPACE_OPEN \
    GRID1D_NAMESPACE_OPEN \
    API_NAMESPACE_OPEN

#define FVG_GRID1D_API_CLOSE \
    API_NAMESPACE_CLOSE \
    GRID1D_NAMESPACE_CLOSE \
    GRID_NAMESPACE_CLOSE \
    FVGRIDMAKER_NAMESPACE_CLOSE

/**
 * @brief Abre a hierarquia completa para Builders de Grid1D.
 * Escopo: `FVGridMaker::grid::grid1d::builders`
 */
#define FVG_GRID1D_BUILDERS_OPEN \
    FVGRIDMAKER_NAMESPACE_OPEN \
    GRID_NAMESPACE_OPEN \
    GRID1D_NAMESPACE_OPEN \
    BUILDERS_NAMESPACE_OPEN

#define FVG_GRID1D_BUILDERS_CLOSE \
    BUILDERS_NAMESPACE_CLOSE \
    GRID1D_NAMESPACE_CLOSE \
    GRID_NAMESPACE_CLOSE \
    FVGRIDMAKER_NAMESPACE_CLOSE

/**
 * @brief Abre a hierarquia completa para Utilitários de Grid1D.
 * Escopo: `FVGridMaker::grid::grid1d::utils`
 */
#define FVG_GRID1D_UTILS_OPEN \
    FVGRIDMAKER_NAMESPACE_OPEN \
    GRID_NAMESPACE_OPEN \
    GRID1D_NAMESPACE_OPEN \
    UTILS_NAMESPACE_OPEN

#define FVG_GRID1D_UTILS_CLOSE \
    UTILS_NAMESPACE_CLOSE \
    GRID1D_NAMESPACE_CLOSE \
    GRID_NAMESPACE_CLOSE \
    FVGRIDMAKER_NAMESPACE_CLOSE

/**
 * @brief Abre a hierarquia completa para Padrões de Distribuição 1D.
 * Escopo: `FVGridMaker::grid::grid1d::patterns::distribution`
 */
#define FVG_GRID1D_DIST_OPEN \
    FVGRIDMAKER_NAMESPACE_OPEN \
    GRID_NAMESPACE_OPEN \
    GRID1D_NAMESPACE_OPEN \
    PATTERNS_NAMESPACE_OPEN \
    DISTRIBUTION_NAMESPACE_OPEN

#define FVG_GRID1D_DIST_CLOSE \
    DISTRIBUTION_NAMESPACE_CLOSE \
    PATTERNS_NAMESPACE_CLOSE \
    GRID1D_NAMESPACE_CLOSE \
    GRID_NAMESPACE_CLOSE \
    FVGRIDMAKER_NAMESPACE_CLOSE

/**
 * @brief Abre a hierarquia completa para Padrões de Centralização 1D.
 * Escopo: `FVGridMaker::grid::grid1d::patterns::centering`
 */
#define FVG_GRID1D_CENTERING_OPEN \
    FVGRIDMAKER_NAMESPACE_OPEN \
    GRID_NAMESPACE_OPEN \
    GRID1D_NAMESPACE_OPEN \
    PATTERNS_NAMESPACE_OPEN \
    CENTERING_NAMESPACE_OPEN

#define FVG_GRID1D_CENTERING_CLOSE \
    CENTERING_NAMESPACE_CLOSE \
    PATTERNS_NAMESPACE_CLOSE \
    GRID1D_NAMESPACE_CLOSE \
    GRID_NAMESPACE_CLOSE \
    FVGRIDMAKER_NAMESPACE_CLOSE

/**
 * @brief Abre a hierarquia para Common Core.
 * Escopo: `FVGridMaker::core::common`
 */
#define FVG_CORE_COMMON_OPEN \
    FVGRIDMAKER_NAMESPACE_OPEN \
    CORE_NAMESPACE_OPEN \
    COMMON_NAMESPACE_OPEN

#define FVG_CORE_COMMON_CLOSE \
    COMMON_NAMESPACE_CLOSE \
    CORE_NAMESPACE_CLOSE \
    FVGRIDMAKER_NAMESPACE_CLOSE