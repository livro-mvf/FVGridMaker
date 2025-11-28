// ----------------------------------------------------------------------------
// File: CellCentered.hpp
// Project: FVGridMaker
// Description: Política de Discretização para Malhas 2D Centradas na Célula.
//              Gerencia topologia, ghost cells e cálculo de métricas.
// License: GNU GPL v3
// ----------------------------------------------------------------------------

#pragma once

#include <FVGridMaker/Core/Common/namespace.hpp>
#include <FVGridMaker/Core/Common/types.hpp>

FVGRIDMAKER_NAMESPACE_OPEN
GRID_NAMESPACE_OPEN

// Sub-namespace para organização (conforme padrão de pastas)
namespace discretization {

/**
 * @struct CellCentered
 * @brief Política de discretização padrão (Finite Volume Method).
 * * Define que:
 * - As variáveis são armazenadas no centro geométrico do volume.
 * - O domínio possui uma camada de células fantasmas (Ghost Layer = 1) por padrão.
 * - Índices (i,j) variam de -1 a Ni (onde 0..Ni-1 são células reais).
 */
struct CellCentered {
    // Importando tipos para uso local
    using Size = core::Size;
    using Index = core::Index;
    using Real = core::Real;
    
    /// Número de camadas de células fantasmas
    static constexpr Size GHOST_LAYERS = 1;

    /**
     * @brief Calcula o tamanho total do buffer necessário (Memória).
     * Inclui padding para ghost cells em todas as direções.
     * Total = (Ni + 2*G) * (Nj + 2*G)
     */
    static Size requiredSize(Size ni, Size nj) {
        return (ni + 2 * GHOST_LAYERS) * (nj + 2 * GHOST_LAYERS);
    }

    /**
     * @brief Mapeia índice lógico (i, j) para índice linear no vetor SoA.
     * * @param i Índice lógico na direção I (pode ser negativo, ex: -1).
     * @param j Índice lógico na direção J (pode ser negativo).
     * @param ni Número de células reais em I.
     */
    static Size getIndex(Index i, Index j, Size ni, Size /*nj*/) {
        // Offset para trazer o índice -1 para 0 na memória
        // Casts explícitos para garantir aritmética correta entre signed/unsigned
        Index i_mem = i + static_cast<Index>(GHOST_LAYERS);
        Index j_mem = j + static_cast<Index>(GHOST_LAYERS);
        
        // Largura total da linha na memória (Stride)
        Size stride_x = ni + 2 * GHOST_LAYERS;
        
        return static_cast<Size>(j_mem) * stride_x + static_cast<Size>(i_mem);
    }

    /**
     * @brief Orquestra o cálculo da geometria chamando a GeometryPolicy.
     * * Percorre toda a malha (reais + ghosts) e preenche o Storage.
     * * @tparam MeshType A classe Mesh instanciada (Template).
     */
    template <typename MeshType>
    static void calculateMetrics(MeshType& mesh, Size ni, Size nj) {
        // Recupera o tipo da Geometria (ex: Cartesian) através da Mesh
        using Geom = typename MeshType::CoordinateSystem;
//        using Config = typename Geom::Config; // Se houver config
        
        // Configuração Hardcoded para este passo (em versão futura virá da Mesh)
        Real x_min = 0.0;
        Real x_max = 1.0;
        Real y_min = 0.0;
        Real y_max = 1.0;
        
        // Passo de malha (uniforme)
        Real dx = (x_max - x_min) / static_cast<Real>(ni);
        Real dy = (y_max - y_min) / static_cast<Real>(nj);

        // Define os limites do loop para incluir Ghost Cells
        // De -1 até Ni (inclusive)
        Index i_start = -static_cast<Index>(GHOST_LAYERS);
        Index i_end   = static_cast<Index>(ni) + static_cast<Index>(GHOST_LAYERS); // Limite exclusivo (<)
        
        Index j_start = -static_cast<Index>(GHOST_LAYERS);
        Index j_end   = static_cast<Index>(nj) + static_cast<Index>(GHOST_LAYERS);

        // Loop sobre TODAS as células
        for (Index j = j_start; j < j_end; ++j) {
            for (Index i = i_start; i < i_end; ++i) {
                Size idx = getIndex(i, j, ni, nj);

                // --- 1. Centros (P) ---
                // O centro da célula 'i' está em i + 0.5 unidades de grade
                mesh.centers_x[idx] = Geom::getX(static_cast<Real>(i) + 0.5, dx, x_min);
                mesh.centers_y[idx] = Geom::getY(static_cast<Real>(j) + 0.5, dy, y_min);
                
                // --- 2. Volumes ---
                mesh.volumes[idx] = Geom::area(dx, dy);

                // --- 3. Faces (Leste e Norte) ---
                // Face Leste de 'i' está em i + 1.0
                mesh.face_east_x[idx] = Geom::getX(static_cast<Real>(i) + 1.0, dx, x_min);
                // Em malha cartesiana alinhada, Y da face Leste é igual ao Y do centro
                mesh.face_east_y[idx] = mesh.centers_y[idx]; 
                
                // Face Norte de 'j' está em j + 1.0
                mesh.face_north_x[idx] = mesh.centers_x[idx];
                mesh.face_north_y[idx] = Geom::getY(static_cast<Real>(j) + 1.0, dy, y_min);
                
                // --- 4. Áreas de Face ---
                // No 2D Cartesiano: Área Leste = dy * 1 (profundidade)
                mesh.area_east[idx]  = dy;
                mesh.area_north[idx] = dx;
                
                // --- 5. Distâncias (Difusão) ---
                // Distância do centro P(i,j) ao vizinho E(i+1,j)
                // Num grid uniforme é apenas dx
                mesh.dist_center_east[idx]  = dx;
                mesh.dist_center_north[idx] = dy;
            }
        }
    }
};

} // namespace discretization
GRID_NAMESPACE_CLOSE
FVGRIDMAKER_NAMESPACE_CLOSE