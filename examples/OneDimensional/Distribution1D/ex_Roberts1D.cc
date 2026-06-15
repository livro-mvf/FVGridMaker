// ----------------------------------------------------------------------------
// File: ex_Roberts1D.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Demonstrates Roberts one-dimensional grid generation.
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
#include <FVGridMaker/OneDimensional/Distribution1D/Roberts1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/FaceCentered1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>

int main() {
    const fvgrid::Axis1D default_axis = fvgrid::Roberts1D::make(
        fvgrid::NVol{12},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0},
        fvgrid::Beta{1.5}
    );

    const fvgrid::Axis1D volume_centered_axis = fvgrid::Roberts1D::make(
        fvgrid::NVol{12},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0},
        fvgrid::Beta{1.5},
        fvgrid::VolumeCentered1D{}
    );

    const fvgrid::Axis1D face_centered_axis = fvgrid::Roberts1D::make(
        fvgrid::NVol{12},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0},
        fvgrid::Beta{1.5},
        fvgrid::FaceCentered1D{}
    );

    std::cout << "Roberts1D default volume-centred axis\n";
    std::cout << default_axis << "\n\n";

    std::cout << "Roberts1D explicit volume-centred axis\n";
    std::cout << volume_centered_axis << "\n\n";

    std::cout << "Roberts1D explicit face-centred axis\n";
    std::cout << face_centered_axis << '\n';

    return 0;
}
