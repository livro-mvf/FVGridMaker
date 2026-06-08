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
#include <iostream>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

int main() {
    const fvgrid::Axis1D axis = fvgrid::Uniform1D::make(
        fvgrid::NVol{10},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0}
    );

    std::cout << axis << '\n';

    return 0;
}