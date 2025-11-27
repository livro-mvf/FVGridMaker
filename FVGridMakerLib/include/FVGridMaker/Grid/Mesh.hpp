// ----------------------------------------------------------------------------
// File: Mesh.hpp
// Project: FVGridMaker
// Description: Classe genérica de malha que compõe Storage com Políticas.
// License: GNU GPL v3
// ----------------------------------------------------------------------------

#pragma once

#include <FVGridMaker/Core/Common/namespace.hpp>
#include <FVGridMaker/Core/Common/types.hpp>
#include <FVGridMaker/Grid/Common/Storage.hpp>

FVGRIDMAKER_NAMESPACE_OPEN
CORE_NAMESPACE_OPEN

/**
 * @class Mesh
 * @brief Classe principal que representa uma malha estruturada.
 * * Esta classe atua como um orquestrador. Ela herda a estrutura de dados (Storage)
 * e delega a lógica matemática para as políticas fornecidas via template.
 * * @tparam CoordSys Define o sistema de coordenadas (Cartesiano, Polar, etc).
 * @tparam DiscretizationPolicy Define o arranjo das variáveis (CellCentered, etc).
 */
template <typename CoordSys, typename DiscretizationPolicy>
class Mesh : public Storage {
public:
    // Expõe os tipos para introspecção (Traits)
    using CoordinateSystem = CoordSys;
    using Discretization   = DiscretizationPolicy;

    // Metadados topológicos (Read-only)
    const Size ni; ///< Número de células reais na direção I
    const Size nj; ///< Número de células reais na direção J

    /**
     * @brief Construtor da Malha.
     * Calcula o tamanho necessário de memória consultando a Política e inicializa o Storage.
     * * @param ni_cells Número de células reais na direção I.
     * @param nj_cells Número de células reais na direção J.
     */
    Mesh(Size ni_cells, Size nj_cells) 
        : ni(ni_cells), nj(nj_cells) 
    {
        // 1. Pergunta à política quanto de memória precisamos (incluindo ghosts)
        Size total_size = DiscretizationPolicy::requiredSize(ni, nj);
        
        // 2. Aloca os vetores do Storage (herdado)
        this->resize(total_size);
    }

    /**
     * @brief Calcula e preenche todas as métricas geométricas.
     * Delega a tarefa para a Política de Discretização.
     */
    void computeGeometry() {
        DiscretizationPolicy::calculateMetrics(*this, ni, nj);
    }

    // --- Métodos de Acesso Auxiliares (Wrappers) ---

    /**
     * @brief Converte índices lógicos (i, j) para o índice linear do vetor SoA.
     * Delega para a política pois ela sabe lidar com Ghost Cells.
     */
    Size index(Index i, Index j) const {
        return DiscretizationPolicy::getIndex(i, j, ni, nj);
    }

    /**
     * @brief Acesso rápido ao volume da célula (i, j).
     */
    Real volume(Index i, Index j) const {
        return this->volumes[index(i, j)];
    }
};

CORE_NAMESPACE_CLOSE
FVGRIDMAKER_NAMESPACE_CLOSE