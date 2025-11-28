// ----------------------------------------------------------------------------
// File: GridIterators.hpp
// Project: FVGridMaker
// Description: Iteradores e Ranges para percorrer a malha de forma segura.
//              Implementa RF10 (Iteradores de Domínio e Fronteira).
// License: GNU GPL v3
// ----------------------------------------------------------------------------

#pragma once

#include <iterator>
#include <FVGridMaker/Core/Common/namespace.hpp>
#include <FVGridMaker/Core/Common/types.hpp>

FVGRIDMAKER_NAMESPACE_OPEN
GRID_NAMESPACE_OPEN

namespace iterators {

/**
 * @class CellIterator
 * @brief Iterador simples que percorre uma região retangular de células (i, j).
 */
template <typename MeshType>
class CellIterator {
public:
    using Size = core::Size;
    using Index = core::Index;
    
    using iterator_category = std::forward_iterator_tag;
    using value_type        = Size;
    using difference_type   = std::ptrdiff_t;
    using pointer           = const Size*;
    using reference         = const Size&;

    CellIterator(const MeshType& mesh, Index i, Index j, Index i_start, Index i_end)
        : mesh_(mesh), i_(i), j_(j), i_start_(i_start), i_end_(i_end) 
    {
        updateCurrentIndex();
    }

    CellIterator& operator++() {
        ++i_;
        if (i_ >= i_end_) {
            i_ = i_start_;
            ++j_;
        }
        updateCurrentIndex();
        return *this;
    }

    Size operator*() const { return current_linear_idx_; }

    bool operator!=(const CellIterator& other) const {
        return i_ != other.i_ || j_ != other.j_;
    }

    bool operator==(const CellIterator& other) const { return !(*this != other); }

private:
    const MeshType& mesh_;
    Index i_, j_;
    Index i_start_, i_end_;
    Size current_linear_idx_;

    void updateCurrentIndex() {
        // Usa a política da malha para converter (i,j) em índice linear
        current_linear_idx_ = MeshType::Discretization::getIndex(i_, j_, mesh_.ni, mesh_.nj);
    }
};

/**
 * @class CellRange
 * @brief Representa um intervalo iterável (range-based for loop).
 */
template <typename MeshType>
class CellRange {
public:
    using Index = core::Index;

    CellRange(const MeshType& mesh, Index i_min, Index i_max, Index j_min, Index j_max)
        : mesh_(mesh), i_start_(i_min), i_end_(i_max), j_start_(j_min), j_end_(j_max) 
    {}

    auto begin() const {
        return CellIterator<MeshType>(mesh_, i_start_, j_start_, i_start_, i_end_);
    }

    auto end() const {
        return CellIterator<MeshType>(mesh_, i_start_, j_end_, i_start_, i_end_);
    }

private:
    const MeshType& mesh_;
    Index i_start_, i_end_;
    Index j_start_, j_end_;
};

// --- Funções Helper ---

template <typename MeshType>
auto InternalCells(const MeshType& mesh) {
    using Index = core::Index;
    return CellRange<MeshType>(
        mesh, 
        0, static_cast<Index>(mesh.ni), 
        0, static_cast<Index>(mesh.nj)
    );
}

template <typename MeshType>
auto AllCells(const MeshType& mesh) {
    using Index = core::Index;
    Index ghost = static_cast<Index>(MeshType::Discretization::GHOST_LAYERS);
    return CellRange<MeshType>(
        mesh, 
        -ghost, static_cast<Index>(mesh.ni) + ghost,
        -ghost, static_cast<Index>(mesh.nj) + ghost
    );
}

} // namespace iterators

GRID_NAMESPACE_CLOSE
FVGRIDMAKER_NAMESPACE_CLOSE