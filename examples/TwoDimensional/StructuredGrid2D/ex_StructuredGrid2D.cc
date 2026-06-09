// ----------------------------------------------------------------------------
// File: ex_StructuredGrid2D.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Demonstrates a two-dimensional rectilinear structured FV grid.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------
//
// This example builds a 2D structured grid from two independent Axis1D
// objects. The grid is rectilinear: x and y coordinates are stored as separate
// one-dimensional axes.
//
// The example demonstrates:
//
//   1. building x and y axes;
//   2. composing them into StructuredGrid2D;
//   3. using row-major linear cell indexing;
//   4. computing Cartesian cell areas;
//   5. computing vertical and horizontal face lengths.
//
// No coordinate-system abstraction is used in this block. The metrics are the
// basic Cartesian/rectilinear measures:
//
//   cell_area(i, j)          = dx_x[i] * dx_y[j]
//   vertical_face_length(j)  = dy[j]
//   horizontal_face_length(i)= dx[i]
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <iomanip>
#include <iostream>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

int main() {
    // ------------------------------------------------------------------------
    // Step 1: build the two independent one-dimensional axes.
    // ------------------------------------------------------------------------
    const fvgrid::Axis1D x_axis = fvgrid::Uniform1D::make(
        fvgrid::NVol{4},
        fvgrid::Length{2.0},
        fvgrid::XInit{0.0}
    );

    const fvgrid::Axis1D y_axis = fvgrid::Uniform1D::make(
        fvgrid::NVol{3},
        fvgrid::Length{1.5},
        fvgrid::XInit{-0.5}
    );

    // ------------------------------------------------------------------------
    // Step 2: compose the 2D rectilinear structured grid.
    // ------------------------------------------------------------------------
    const fvgrid::StructuredGrid2D grid{x_axis, y_axis};

    std::cout << std::fixed << std::setprecision(6);

    // ------------------------------------------------------------------------
    // Step 3: print a compact grid summary.
    // ------------------------------------------------------------------------
    std::cout << "StructuredGrid2D\n";
    std::cout << "number of cells in x : " << grid.num_cells_x() << '\n';
    std::cout << "number of cells in y : " << grid.num_cells_y() << '\n';
    std::cout << "total number of cells: " << grid.num_cells() << '\n';
    std::cout << "x-domain             : ["
              << grid.xmin() << ", " << grid.xmax() << "]\n";
    std::cout << "y-domain             : ["
              << grid.ymin() << ", " << grid.ymax() << "]\n";
    std::cout << '\n';

    // ------------------------------------------------------------------------
    // Step 4: inspect cells using row-major indexing.
    // ------------------------------------------------------------------------
    std::cout << "Cells\n";
    std::cout
        << std::setw(6) << "i"
        << std::setw(6) << "j"
        << std::setw(10) << "linear"
        << std::setw(14) << "xcenter"
        << std::setw(14) << "ycenter"
        << std::setw(14) << "area"
        << '\n';

    std::cout << std::string(64, '-') << '\n';

    for (fvgrid::Size j = 0; j < grid.num_cells_y(); ++j) {
        for (fvgrid::Size i = 0; i < grid.num_cells_x(); ++i) {
            std::cout
                << std::setw(6) << i
                << std::setw(6) << j
                << std::setw(10) << grid.linear_cell_index(i, j)
                << std::setw(14) << grid.x_center(i)
                << std::setw(14) << grid.y_center(j)
                << std::setw(14) << grid.cell_area(i, j)
                << '\n';
        }
    }

    std::cout << '\n';

    // ------------------------------------------------------------------------
    // Step 5: inspect Cartesian face lengths.
    // ------------------------------------------------------------------------
    std::cout << "Face lengths\n";

    for (fvgrid::Size j = 0; j < grid.num_cells_y(); ++j) {
        std::cout << "vertical face length at row " << j
                  << " = " << grid.vertical_face_length(j) << '\n';
    }

    for (fvgrid::Size i = 0; i < grid.num_cells_x(); ++i) {
        std::cout << "horizontal face length at column " << i
                  << " = " << grid.horizontal_face_length(i) << '\n';
    }

    return 0;
}