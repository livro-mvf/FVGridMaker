// ----------------------------------------------------------------------------
// File: Cartesian.hpp
// Project: FVGridMaker
// Description: Política de Geometria para Malhas Cartesianas 2D.
//              Define transformações lineares de espaço computacional para físico.
// License: GNU GPL v3
// ----------------------------------------------------------------------------

#pragma once

#include <cmath>
#include <FVGridMaker/Core/Common/namespace.hpp>
#include <FVGridMaker/Core/Common/types.hpp>

FVGRIDMAKER_NAMESPACE_OPEN
GRID_NAMESPACE_OPEN

// Opcional: Podemos criar um sub-namespace 'geometry' manualmente se não houver macro
namespace geometry {

/**
 * @struct Cartesian
 * @brief Política de Geometria para sistemas de coordenadas Cartesianos (x, y).
 * * Esta classe fornece métodos estáticos para converter posições de índice fracionário
 * em coordenadas físicas, calcular distâncias euclidianas e áreas retangulares.
 * * É utilizada pela política de discretização (ex: CellCentered) para preencher a malha.
 */
struct Cartesian {
    // Importando tipos para uso local
    using Real = core::Real;

    /**
     * @struct Config
     * @brief Parâmetros de configuração do domínio físico.
     */
    struct Config {
        Real x_min = 0.0;
        Real x_max = 1.0;
        Real y_min = 0.0;
        Real y_max = 1.0;
    };

    // ========================================================================
    // Transformações Ponto a Ponto
    // ========================================================================

    /**
     * @brief Calcula a coordenada X baseada em um índice (ou fração de índice).
     * @param i_frac Índice na direção I (pode ser fracionário, ex: i+0.5 para centro).
     * @param dx Espaçamento na direção X.
     * @param x_origin Origem do domínio (x_min).
     */
    static Real getX(Real i_frac, Real dx, Real x_origin) {
        return x_origin + i_frac * dx;
    }

    /**
     * @brief Calcula a coordenada Y baseada em um índice (ou fração de índice).
     * @param j_frac Índice na direção J (pode ser fracionário, ex: j+0.5 para centro).
     * @param dy Espaçamento na direção Y.
     * @param y_origin Origem do domínio (y_min).
     */
    static Real getY(Real j_frac, Real dy, Real y_origin) {
        return y_origin + j_frac * dy;
    }
    
    // ========================================================================
    // Métricas Geométricas
    // ========================================================================

    /**
     * @brief Calcula a distância Euclidiana 2D entre dois pontos.
     * Útil para calcular distâncias entre centróides (difusão).
     */
    static Real distance(Real x1, Real y1, Real x2, Real y2) {
        Real dx = x2 - x1;
        Real dy = y2 - y1;
        return std::sqrt(dx*dx + dy*dy);
    }
    
    /**
     * @brief Calcula a área de uma célula cartesiana (retângulo).
     * @note Em coordenadas cartesianas, a área é simplesmente dx * dy.
     */
    static Real area(Real dx, Real dy) {
        return dx * dy;
    }
};

} // namespace geometry
GRID_NAMESPACE_CLOSE
FVGRIDMAKER_NAMESPACE_CLOSE
