// ----------------------------------------------------------------------------
// File: Axis1DRows.h
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Internal row traversal helpers for Axis1D tabular output.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

#include <concepts>

#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>

namespace fvgrid::detail {

template <std::floating_point T, class Callback>
void for_each_axis1d_row(
    const BasicAxis1D<T>& axis,
    Callback&& callback
) {
    const auto faces = axis.faces();
    const auto centers = axis.centers();
    const auto dx_faces = axis.dx_faces();
    const auto dx_centers = axis.dx_centers();

    const Size cell_count = axis.num_cells();

    for (Size i = 0; i < cell_count; ++i) {
        callback(
            i,
            faces[i],
            centers[i],
            dx_faces[i],
            dx_centers[i],
            true
        );
    }

    callback(
        cell_count,
        faces[cell_count],
        T{},
        T{},
        dx_centers[cell_count],
        false
    );
}

}  // namespace fvgrid::detail
