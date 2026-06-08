// ----------------------------------------------------------------------------
// File: ex_Uniform1D.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Demonstrates uniform one-dimensional grid generation.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <iomanip>
#include <iostream>
#include <string>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>

int main() {
    const fvgrid::Axis1D axis = fvgrid::Uniform1D::make(
        fvgrid::NVol{10},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0},
        fvgrid::VolumeCentered1D{}
    );

    const auto faces = axis.faces();
    const auto centers = axis.centers();
    const auto dx_faces = axis.dx_faces();
    const auto dx_centers = axis.dx_centers();

    const fvgrid::Size nvol = axis.num_cells();

    std::cout << "Uniform1D example\n";
    std::cout << "pattern         : " << axis.pattern_name() << '\n';
    std::cout << "number of faces : " << axis.num_faces() << '\n';
    std::cout << "number of cells : " << axis.num_cells() << '\n';
    std::cout << "xmin            : " << axis.xmin() << '\n';
    std::cout << "xmax            : " << axis.xmax() << '\n';
    std::cout << "length          : " << axis.length() << '\n';
    std::cout << '\n';

    std::cout << std::setw(6) << "i"
              << std::setw(16) << "xface[i]"
              << std::setw(16) << "xcenter[i]"
              << std::setw(16) << "dxface[i]"
              << std::setw(16) << "dxcenter[i]"
              << '\n';

    std::cout << std::string(70, '-') << '\n';

    for (fvgrid::Size i = 0; i < nvol; ++i) {
        std::cout << std::setw(6) << i
                  << std::setw(16) << faces[i]
                  << std::setw(16) << centers[i]
                  << std::setw(16) << dx_faces[i]
                  << std::setw(16) << dx_centers[i]
                  << '\n';
    }

    std::cout << std::setw(6) << nvol
              << std::setw(16) << faces[nvol]
              << std::setw(16) << ""
              << std::setw(16) << ""
              << std::setw(16) << dx_centers[nvol]
              << '\n';

    return 0;
}