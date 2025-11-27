// ----------------------------------------------------------------------------
// File: Grid1D.h
// Project: FVGridMaker
// Version: 2.0
// Description: Classe de dados (Container) para a Malha 1D.
//              Armazena a geometria completa (faces, centros, volumes, distâncias)
//              seguindo o padrão SoA (Structure of Arrays).
// Last modified: 2025-11-25
// Author: FVGridMaker Team
// License: GNU GPL v3
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// includes C++
// ----------------------------------------------------------------------------
#include <span>
#include <vector>
#include <utility> // std::move

// ----------------------------------------------------------------------------
// includes FVMGridMaker
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/Common/namespace.hpp>
#include <FVGridMaker/Core/Common/types.hpp>
#include <FVGridMaker/Core/Common/iD.hpp>

/**
 * @file Grid1D.h
 * @brief Definição da classe de dados Grid1D.
 * @ingroup Grid1D
 */

FVG_GRID1D_API_OPEN

using FVGridMaker::core::Real;
using FVGridMaker::core::Index;
using FVGridMaker::core::common::ID;

/**
 * @class Grid1D
 * @brief Representação imutável da geometria de uma malha 1D.
 *
 * Esta classe atua como um container de dados geométricos pré-calculados.
 * Ela não possui lógica de geração (isso é responsabilidade do Builder).
 *
 * Estrutura interna (SoA):
 * - Faces (N+1): Coordenadas das interfaces dos volumes.
 * - Centers (N): Coordenadas dos centros nodais.
 * - dF (N): Largura dos volumes (deltas de face).
 * - dC (N+1): Distâncias entre centros adjacentes (deltas de centro).
 */
class Grid1D : public ID {

public:

    // ------------------------------------------------------------------------
    // Construtores
    // ------------------------------------------------------------------------

    Grid1D() : ID("EmptyGrid1D") {}

    /**
     * @brief Construtor principal (usado pelo Builder).
     * Adota os vetores via move semantics para eficiência.
     */
    explicit Grid1D(std::vector<Real> faces,
                    std::vector<Real> centers,
                    std::vector<Real> dF,
                    std::vector<Real> dC)
        : ID("Grid1D"),
          m_faces(std::move(faces)),
          m_centers(std::move(centers)),
          m_dF(std::move(dF)),
          m_dC(std::move(dC))
    {}

    // Movimentável (padrão)
    Grid1D(Grid1D&&) noexcept = default;
    Grid1D& operator=(Grid1D&&) noexcept = default;

    // Copiável (padrão - gera novos vetores)
    Grid1D(const Grid1D&) = default;
    Grid1D& operator=(const Grid1D&) = default;

    virtual ~Grid1D() = default;

    // ------------------------------------------------------------------------
    // Acesso a Dados (Views / Spans)
    // ------------------------------------------------------------------------

    /** @brief Retorna view das coordenadas das faces (Tamanho N+1). */
    [[nodiscard]] std::span<const Real> faces() const noexcept {
        return m_faces;
    }

    /** @brief Retorna view das coordenadas dos centros (Tamanho N). */
    [[nodiscard]] std::span<const Real> centers() const noexcept {
        return m_centers;
    }

    /** @brief Retorna view das larguras dos volumes (dX). Tamanho N. */
    [[nodiscard]] std::span<const Real> deltasFaces() const noexcept {
        return m_dF;
    }

    /** @brief Retorna view das distâncias entre centros (Diffusão). Tamanho N+1. */
    [[nodiscard]] std::span<const Real> deltasCenters() const noexcept {
        return m_dC;
    }

    // ------------------------------------------------------------------------
    // Informações Agregadas
    // ------------------------------------------------------------------------

    /** @brief Número de volumes de controle (N). */
    [[nodiscard]] Index nVolumes() const noexcept {
        return static_cast<Index>(m_centers.size());
    }

    /** @brief Número de faces (N+1). */
    [[nodiscard]] Index nFaces() const noexcept {
        return static_cast<Index>(m_faces.size());
    }

    /** @brief Limite esquerdo do domínio físico. */
    [[nodiscard]] Real minCoord() const noexcept {
        return m_faces.empty() ? Real(0) : m_faces.front();
    }

    /** @brief Limite direito do domínio físico. */
    [[nodiscard]] Real maxCoord() const noexcept {
        return m_faces.empty() ? Real(0) : m_faces.back();
    }

    /** @brief Comprimento total do domínio. */
    [[nodiscard]] Real length() const noexcept {
        return maxCoord() - minCoord();
    }

private:
    std::vector<Real> m_faces;   ///< Coordenadas das faces (xf).
    std::vector<Real> m_centers; ///< Coordenadas dos centros (xc).
    std::vector<Real> m_dF;      ///< Larguras das células (xf_i+1 - xf_i).
    std::vector<Real> m_dC;      ///< Distâncias entre centros (xc_i+1 - xc_i).
};

FVG_GRID1D_API_CLOSE