// ----------------------------------------------------------------------------
// File: Storage.hpp
// Project: FVGridMaker
// Description: Estrutura de armazenamento SoA utilizando os tipos e 
//              namespaces padrão da biblioteca.
// License: GNU GPL v3
// ----------------------------------------------------------------------------

#pragma once

#include <vector>

// Inclusão dos tipos e namespaces do projeto existente
// Assumindo que Types.hpp está em Core/Common baseado na estrutura do namespace.hpp
#include <FVGridMaker/Core/Common/types.hpp>
#include <FVGridMaker/Core/Common/namespace.hpp>

FVGRIDMAKER_NAMESPACE_OPEN
CORE_NAMESPACE_OPEN

/**
 * @struct Storage
 * @brief Contêiner de dados geométricos brutos utilizando Structure of Arrays (SoA).
 * * Esta estrutura foi adaptada para utilizar o sistema de tipos do FVGridMaker (`Real`, `Size`),
 * garantindo que a precisão (float/double/long double) seja controlada globalmente
 * pelas macros definidas em `Types.hpp`.
 * * Implementa o armazenamento contíguo para máxima eficiência de cache.
 */
struct Storage {
    
    // ========================================================================
    // 1. Geometria dos Centros (Volumes de Controle)
    // ========================================================================
    
    /// @brief Coordenada X do centróide geométrico da célula (P).
    std::vector<Real> centers_x;
    
    /// @brief Coordenada Y do centróide geométrico da célula (P).
    std::vector<Real> centers_y;
    
    /// @brief Volume escalar (ou Área em 2D) da célula \f$ \Omega_P \f$.
    std::vector<Real> volumes;

    // ========================================================================
    // 2. Geometria das Faces (Interfaces de Fluxo)
    // ========================================================================
    
    /// @brief Coordenada X do ponto central da face Leste (i + 1/2).
    std::vector<Real> face_east_x;
    
    /// @brief Coordenada Y do ponto central da face Leste (i + 1/2).
    std::vector<Real> face_east_y;
    
    /// @brief Coordenada X do ponto central da face Norte (j + 1/2).
    std::vector<Real> face_north_x;
    
    /// @brief Coordenada Y do ponto central da face Norte (j + 1/2).
    std::vector<Real> face_north_y;

    // ========================================================================
    // 3. Métricas Pré-Calculadas
    // ========================================================================

    /// @brief Distância física \f$ d_{PE} \f$ entre o centro P(i,j) e o vizinho Leste.
    std::vector<Real> dist_center_east; 
    
    /// @brief Distância física \f$ d_{PN} \f$ entre o centro P(i,j) e o vizinho Norte.
    std::vector<Real> dist_center_north;

    /// @brief Área (magnitude) da face Leste \f$ A_e \f$.
    std::vector<Real> area_east;
    
    /// @brief Área (magnitude) da face Norte \f$ A_n \f$.
    std::vector<Real> area_north;

    // ========================================================================
    // Métodos de Gerenciamento de Memória
    // ========================================================================

    /**
     * @brief Realoca a memória de todos os vetores geométricos.
     * @param total_cells Número total de elementos (incluindo ghosts e padding).
     */
    void resize(Size total_cells) {
        // Alocação para Centros
        centers_x.resize(total_cells);
        centers_y.resize(total_cells);
        volumes.resize(total_cells);
        
        // Alocação para Faces
        face_east_x.resize(total_cells);
        face_east_y.resize(total_cells);
        face_north_x.resize(total_cells);
        face_north_y.resize(total_cells);
        
        // Alocação para Métricas
        dist_center_east.resize(total_cells);
        dist_center_north.resize(total_cells);
        area_east.resize(total_cells);
        area_north.resize(total_cells);
    }

    /**
     * @brief Retorna a capacidade atual da estrutura.
     * @return Número de células alocadas (tipo `Size` definido em Types.hpp).
     */
    Size size() const {
        return centers_x.size();
    }

    /**
     * @brief Libera toda a memória.
     */
    void clear() {
        centers_x.clear(); 
        centers_y.clear(); 
        volumes.clear();
        
        face_east_x.clear(); 
        face_east_y.clear();
        face_north_x.clear(); 
        face_north_y.clear();
        
        dist_center_east.clear(); 
        dist_center_north.clear();
        area_east.clear(); 
        area_north.clear();
    }
};

CORE_NAMESPACE_CLOSE
FVGRIDMAKER_NAMESPACE_CLOSE