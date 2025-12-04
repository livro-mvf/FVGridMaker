// ----------------------------------------------------------------------------
// File: Grid1D.h
// Project: FVGridMaker
// Version: 2.3
// Description: Data class for 1D grids (SoA layout).
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// includes FVMGridMaker (alphabetical by path)
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/Common/constants.hpp>
#include <FVGridMaker/Core/Common/ID.hpp>
#include <FVGridMaker/Core/Common/namespace.hpp>
#include <FVGridMaker/Core/Common/types.hpp>

// ----------------------------------------------------------------------------
// includes C++ (alphabetical)
// ----------------------------------------------------------------------------
#include <cstddef>
#include <ostream>
#include <iomanip>
#include <span>
#include <utility>
#include <vector>

/**
 * @file Grid1D.h
 * @brief 1D grid data container (SoA layout).
 * @ingroup Grid1D
 */

FVG_GRID1D_API_OPEN

using FVGridMaker::core::Index;
using FVGridMaker::core::Real;
using FVGridMaker::core::common::ID;

/**
 * @class Grid1D
 * @brief Immutable geometry representation of a 1D grid.
 *
 * This class is a pure data container. It does not build the grid;
 * that responsibility belongs to Grid1DBuilder.
 *
 * Internal SoA structure:
 *  - faces_   (size = N_faces)   : face coordinates;
 *  - centers_ (size = N_cells)   : cell-center coordinates;
 *  - dF_      (size = N_cells)   : face-to-face distances (xf[i+1] - xf[i]);
 *  - dC_      (size = N_faces)   : center-related distances (builder convention).
 *
 * Metadata:
 *  - n_physical_ : number of physical cells (interior);
 *  - n_ghost_    : number of ghost cells per side.
 */
class Grid1D : public ID {
public:
    // ------------------------------------------------------------------------
    // Constructors
    // ------------------------------------------------------------------------

    /// @brief Default constructor: empty grid.
    Grid1D()
        : ID("EmptyGrid1D")
        , faces_{}
        , centers_{}
        , dF_{}
        , dC_{}
        , n_physical_{0}
        , n_ghost_{0} {}

    /**
     * @brief Main constructor (used by the builder).
     *
     * All vectors are passed by value and moved into the internal storage.
     * Metadata is optional; if n_physical == 0, it defaults to centers.size().
     */
    explicit Grid1D(std::vector<Real> faces,
                    std::vector<Real> centers,
                    std::vector<Real> dF,
                    std::vector<Real> dC,
                    Index n_physical = 0,
                    Index n_ghost = 0)
        : ID("Grid1D")
        , faces_(std::move(faces))
        , centers_(std::move(centers))
        , dF_(std::move(dF))
        , dC_(std::move(dC))
        , n_physical_{n_physical}
        , n_ghost_{n_ghost} {
        if (n_physical_ == 0) {
            n_physical_ = static_cast<Index>(centers_.size());
        }
    }

    Grid1D(const Grid1D&) = default;
    Grid1D& operator=(const Grid1D&) = default;
    Grid1D(Grid1D&&) noexcept = default;
    Grid1D& operator=(Grid1D&&) noexcept = default;

    ~Grid1D() override = default;

    // ------------------------------------------------------------------------
    // SoA accessors (read-only views)
    // ------------------------------------------------------------------------

    /// @brief Read-only view of face coordinates.
    [[nodiscard]] std::span<const Real> faces() const noexcept {
        return std::span<const Real>{faces_.data(), faces_.size()};
    }

    /// @brief Read-only view of cell-center coordinates.
    [[nodiscard]] std::span<const Real> centers() const noexcept {
        return std::span<const Real>{centers_.data(), centers_.size()};
    }

    /// @brief Read-only view of face-to-face distances (cell widths).
    [[nodiscard]] std::span<const Real> deltasFaces() const noexcept {
        return std::span<const Real>{dF_.data(), dF_.size()};
    }

    /// @brief Read-only view of center-related distances.
    [[nodiscard]] std::span<const Real> deltasCenters() const noexcept {
        return std::span<const Real>{dC_.data(), dC_.size()};
    }

    // ------------------------------------------------------------------------
    // Aggregate info
    // ------------------------------------------------------------------------

    /// @brief Number of stored cells (centers).
    [[nodiscard]] Index nVolumes() const noexcept {
        return static_cast<Index>(centers_.size());
    }

    /// @brief Number of stored faces.
    [[nodiscard]] Index nFaces() const noexcept {
        return static_cast<Index>(faces_.size());
    }

    /// @brief Number of physical cells (interior).
    [[nodiscard]] Index nPhysicalCells() const noexcept {
        return n_physical_;
    }

    /// @brief Number of ghost cells per side.
    [[nodiscard]] Index nGhost() const noexcept {
        return n_ghost_;
    }

    /// @brief Total number of cells (physical + ghosts).
    [[nodiscard]] Index nTotalCells() const noexcept {
        return n_physical_ + 2 * n_ghost_;
    }

    /// @brief Left boundary coordinate of the physical domain.
    [[nodiscard]] Real minCoord() const noexcept {
        return faces_.empty() ? Real(0) : faces_.front();
    }

    /// @brief Right boundary coordinate of the physical domain.
    [[nodiscard]] Real maxCoord() const noexcept {
        return faces_.empty() ? Real(0) : faces_.back();
    }

    /// @brief Physical domain length.
    [[nodiscard]] Real length() const noexcept {
        return maxCoord() - minCoord();
    }

private:
    // Geometry (SoA)
    std::vector<Real> faces_;
    std::vector<Real> centers_;
    std::vector<Real> dF_;
    std::vector<Real> dC_;

    // Metadata
    Index n_physical_;
    Index n_ghost_;
};

// ----------------------------------------------------------------------------
// Formatted printing (std::ostream << Grid1D)
// ----------------------------------------------------------------------------

inline std::ostream& operator<<(std::ostream& os, const Grid1D& grid) {

    const auto faces   = grid.faces();
    const auto centers = grid.centers();
    const auto dF      = grid.deltasFaces();
    const auto dC      = grid.deltasCenters();
    const  size_t   SIZE(80);   
           std::size_t i;

    if (centers.empty()) {   
        return os;
    }

auto flags = os.flags();

    os << std::right << std::fixed << std::setprecision(6);  
    os << std::string(SIZE, '-') << "\n";
    os << "Malha Grid1D\n";
    os << std::string(SIZE, '-') << "\n\n";

    os  << std::setw(6) << "index"
        << std::setw(15) << "faces"
        << std::setw(15) << "centers"
        << std::setw(15) << "dF"
        << std::setw(15) << "dC"
        << '\n';   

    for (i = 0; i < centers.size(); ++i) {
        os  << std::setw(6) << i
            << std::setw(15) << faces[i]
            << std::setw(15) << centers[i]
            << std::setw(15) << dF[i]
            << std::setw(15) << dC[i]
            << "\n";
    }
    
    os  << std::setw(6) << i
        << std::setw(15) << faces[i]
        << std::setw(45) << dC[i]
        << "\n";


    os.flags(flags);    
    return os;
}


FVG_GRID1D_API_CLOSE
