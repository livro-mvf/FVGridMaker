// ----------------------------------------------------------------------------
// File: ex_Custom1D.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Demonstrates generic custom one-dimensional grid generation.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <iostream>
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/OneDimensional/Distribution1D/Custom1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Coordinates1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Domain1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/FaceCentered1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>

int main() {
    const fvgrid::Axis1D volume_centered_axis = fvgrid::Custom1D::make(
        fvgrid::Coordinates1D::faces(
            std::vector<fvgrid::Real>{0.0, 0.2, 0.7, 1.0}
        ),
        fvgrid::VolumeCentered1D{}
    );

    const fvgrid::Axis1D face_centered_axis = fvgrid::Custom1D::make(
        fvgrid::Coordinates1D::centers(
            std::vector<fvgrid::Real>{0.1, 0.45, 0.85}
        ),
        fvgrid::FaceCentered1D{},
        fvgrid::Domain1D::from_length(
            fvgrid::XInit{0.0},
            fvgrid::Length{1.0}
        )
    );

    std::cout << "Custom volume-centred axis\n";
    std::cout << volume_centered_axis << "\n\n";

    std::cout << "Custom face-centred axis\n";
    std::cout << face_centered_axis << '\n';

    return 0;
}