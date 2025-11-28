// ----------------------------------------------------------------------------
// File: VTKWriter.hpp
// Project: FVGridMaker
// Description: Exportador de malhas para formato VTK Legacy (.vtk).
//              Suporta Structured Grid para visualização no ParaView.
// License: GNU GPL v3
// ----------------------------------------------------------------------------

#pragma once

#include <string>
#include <fstream>
#include <iomanip>
#include <vector>

// Includes da biblioteca
// Ajuste os caminhos conforme a sua estrutura de pastas
#include <FVGridMaker/Core/Common/namespace.hpp>
#include <FVGridMaker/Core/Common/types.hpp>
#include <FVGridMaker/Grid/Iterators/GridIterators.hpp>

// Se não existir macro específica para IO no namespace.hpp, usamos a raiz
FVGRIDMAKER_NAMESPACE_OPEN

// Namespace IO para separar responsabilidades
namespace io {

/**
 * @class VTKWriter
 * @brief Classe utilitária para exportar a malha para formato VTK Legacy.
 * * Gera ficheiros ASCII fáceis de inspecionar.
 * * Tipo de Grid: STRUCTURED_GRID.
 * * Dados Exportados:
 * - Pontos (Vértices): Reconstruídos a partir das faces da malha FVM.
 * - Cell Data: Volumes e IDs para validação.
 */
class VTKWriter {
public:
    // Importa tipos do Core para consistência
    using Real = core::Real;
    using Size = core::Size;
    using Index = core::Index;

    /**
     * @brief Escreve a malha em disco no formato VTK.
     * * @tparam MeshType Tipo da malha (template).
     * @param mesh Referência para a malha a ser exportada.
     * @param filename Caminho do ficheiro de saída (ex: "output.vtk").
     */
    template <typename MeshType>
    static void write(const MeshType& mesh, const std::string& filename) {
        std::ofstream file(filename);
        
        // Em produção, deve-se tratar erros de IO adequadamente
        if (!file.is_open()) {
            return; 
        }

        // Configuração de precisão para garantir dados corretos no ASCII
        file << std::scientific << std::setprecision(6);

        // ====================================================================
        // 1. Cabeçalho VTK
        // ====================================================================
        file << "# vtk DataFile Version 3.0\n";
        file << "FVGridMaker Mesh Export\n";
        file << "ASCII\n";
        file << "DATASET STRUCTURED_GRID\n";

        // ====================================================================
        // 2. Dimensões (Nós = Células + 1)
        // ====================================================================
        // O formato STRUCTURED_GRID define a topologia pelos vértices (pontos).
        // Se temos Ni células, teremos Ni+1 pontos na direção I.
        // O cast para Index é seguro pois ni/nj são positivos.
        Size nx_nodes = mesh.ni + 1;
        Size ny_nodes = mesh.nj + 1;
        Size nz_nodes = 1; // Malha 2D é tratada como fatia 3D plana no VTK

        file << "DIMENSIONS " << nx_nodes << " " << ny_nodes << " " << nz_nodes << "\n";

        // ====================================================================
        // 3. Pontos (POINTS) - Vértices da Malha
        // ====================================================================
        Size num_points = nx_nodes * ny_nodes * nz_nodes;
        file << "POINTS " << num_points << " double\n";

        // Loop sobre os NÓS (0 a Ni, 0 a Nj)
        // Precisamos reconstruir as coordenadas dos nós usando as faces armazenadas.
        // O nó (i, j) é o canto inferior esquerdo da célula (i, j)
        // ou o canto superior direito da célula (i-1, j-1).
        
        for (Index j = 0; j <= static_cast<Index>(mesh.nj); ++j) {
            for (Index i = 0; i <= static_cast<Index>(mesh.ni); ++i) {
                
                // Índices das células que definem este nó
                // i-1 pega a face Leste da célula anterior (que é a posição do nó i)
                Index i_ref = i - 1; 
                Index j_ref = j - 1; 
                
                // Para acessar os vetores, precisamos do índice linear.
                // Usamos j=0 e i=0 como referência segura para grades ortogonais.
                Size idx_x = mesh.index(i_ref, 0); 
                Size idx_y = mesh.index(0, j_ref); 

                // Recupera coordenadas das faces
                Real x = mesh.face_east_x[idx_x];
                Real y = mesh.face_north_y[idx_y];
                Real z = 0.0;

                file << x << " " << y << " " << z << "\n";
            }
        }

        // ====================================================================
        // 4. Dados das Células (CELL_DATA) - Centróides e Volumes
        // ====================================================================
        Size num_cells = mesh.ni * mesh.nj;
        file << "\nCELL_DATA " << num_cells << "\n";
        
        // Exporta o Volume como escalar
        file << "SCALARS Volume double 1\n";
        file << "LOOKUP_TABLE default\n";
        
        // Usa o namespace completo para os iteradores
        // Assumindo que grid::iterators::InternalCells está disponível via include
        for (auto idx : grid::iterators::InternalCells(mesh)) {
            file << mesh.volumes[idx] << "\n";
        }

        // Exporta ID global para debug visual
        file << "SCALARS CellID int 1\n";
        file << "LOOKUP_TABLE default\n";
        Index id_counter = 0;
        for (auto idx : grid::iterators::InternalCells(mesh)) {
            (void)idx; // supress warning
            file << id_counter++ << "\n";
        }

        file.close();
    }
};

} // namespace io
FVGRIDMAKER_NAMESPACE_CLOSE