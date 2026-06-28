// ----------------------------------------------------------------------------
// File: ex_Random1D.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Demonstrates random one-dimensional grid generation.
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
#include <FVGridMaker/OneDimensional/Distribution1D/Random1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/FaceCentered1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>

int main() {
    const fvgrid::Axis1D default_axis = fvgrid::Random1D::make(
        fvgrid::NVol{8},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0},
        fvgrid::Seed{1234}
    );

    const fvgrid::Axis1D volume_centered_axis = fvgrid::Random1D::make(
        fvgrid::NVol{8},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0},
        fvgrid::Seed{1234},
        fvgrid::MinSpacing{0.05},
        fvgrid::VolumeCentered1D{}
    );

    const fvgrid::Axis1D face_centered_axis = fvgrid::Random1D::make(
        fvgrid::NVol{8},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0},
        fvgrid::Seed{1234},
        fvgrid::MinSpacing{0.05},
        fvgrid::FaceCentered1D{}
    );

    std::cout << "Random1D default face-centred axis\n";
    std::cout << default_axis << "\n\n";

    std::cout << "Random1D explicit volume-centred axis\n";
    std::cout << volume_centered_axis << "\n\n";

    std::cout << "Random1D explicit face-centred axis\n";
    std::cout << face_centered_axis << '\n';

    return 0;
}