// ----------------------------------------------------------------------------
// File: Grid1D.h
// Project: FVGridMaker
// Version: 2.1
// Description: Classe de dados (Container) para a Malha 1D.
//              Armazena a geometria completa seguindo o padrão SoA.
// Last modified: 2025-11-26
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// includes C++
// ----------------------------------------------------------------------------
#include <span>
#include <vector>
#include <utility> // std::move

// ----------------------------------------------------------------------------
// includes FVMGridMaker (Etapa 0)
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/Common/namespace.hpp>
#include <FVGridMaker/Core/Common/types.hpp>
#include <FVGridMaker/Core/Common/constants.hpp>
#include <FVGridMaker/Core/Common/ID.hpp>

/**
 * @file Grid1D.h
 * @brief Definição da classe de dados Grid1D.
 * @ingroup Grid1D
 */

FVG_GRID1D_API_OPEN

// Importa tipos para o escopo local da classe
using FVGridMaker::core::Real;
using FVGridMaker::core::Index;
using FVGridMaker::core::common::ID;

/**
 * @class Grid1D
 * @brief Representação imutável da geometria de uma malha 1D.
 */
class Grid1D : public ID {
public:
    // ------------------------------------------------------------------------
    // Construtores
    // ------------------------------------------------------------------------

    /** @brief Construtor padrão (malha vazia). */
    Grid1D() : ID("EmptyGrid1D") {}

    /**
     * @brief Construtor principal (usado pelo Builder).
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

    // Movimentável
    Grid1D(Grid1D&&) noexcept = default;
    Grid1D& operator=(Grid1D&&) noexcept = default;

    // Copiável
    Grid1D(const Grid1D&) = default;
    Grid1D& operator=(const Grid1D&) = default;

    virtual ~Grid1D() = default;

    // ------------------------------------------------------------------------
    // Acesso a Dados (Views / Spans)
    // ------------------------------------------------------------------------

    [[nodiscard]] std::span<const Real> faces() const noexcept {
        return m_faces;
    }

    [[nodiscard]] std::span<const Real> centers() const noexcept {
        return m_centers;
    }

    [[nodiscard]] std::span<const Real> deltasFaces() const noexcept {
        return m_dF;
    }

    [[nodiscard]] std::span<const Real> deltasCenters() const noexcept {
        return m_dC;
    }

    // ------------------------------------------------------------------------
    // Informações Agregadas
    // ------------------------------------------------------------------------

    [[nodiscard]] Index nVolumes() const noexcept {
        return static_cast<Index>(m_centers.size());
    }

    [[nodiscard]] Index nFaces() const noexcept {
        return static_cast<Index>(m_faces.size());
    }

    [[nodiscard]] Real minCoord() const noexcept {
        return m_faces.empty() ? Real(0) : m_faces.front();
    }

    [[nodiscard]] Real maxCoord() const noexcept {
        return m_faces.empty() ? Real(0) : m_faces.back();
    }

    [[nodiscard]] Real length() const noexcept {
        return maxCoord() - minCoord();
    }

private:
    std::vector<Real> m_faces;
    std::vector<Real> m_centers;
    std::vector<Real> m_dF;
    std::vector<Real> m_dC;
};

FVG_GRID1D_API_CLOSE