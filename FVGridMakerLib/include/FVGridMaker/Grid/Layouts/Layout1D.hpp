// ----------------------------------------------------------------------------
// File: Layout1D.hpp
// Project: FVGridMaker
// Version: 1.0
// Description: Tags de layout (FaceCentered, CellCentered) e traits mínimos
//              usados pelo Grid1DBuilder para interpretar a malha 1D.
// License: GNU GPL v3
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// includes FVMGridMaker (ordem alfabética por caminho)
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/Common/namespace.hpp>

// ----------------------------------------------------------------------------
// includes C++ (ordem alfabética)
// ----------------------------------------------------------------------------
#include <cstdint>

FVGRIDMAKER_NAMESPACE_OPEN
GRID_NAMESPACE_OPEN
GRID1D_NAMESPACE_OPEN

enum class PrimaryCoord1D : std::uint8_t {
    Faces   = 0,
    Centers = 1
};

struct FaceCentered { static constexpr PrimaryCoord1D primary = PrimaryCoord1D::Faces; };
struct CellCentered { static constexpr PrimaryCoord1D primary = PrimaryCoord1D::Centers; };

namespace detail {
template <typename Layout>
inline constexpr PrimaryCoord1D primary_coord_v = Layout::primary;
}

GRID1D_NAMESPACE_CLOSE
GRID_NAMESPACE_CLOSE
FVGRIDMAKER_NAMESPACE_CLOSE
