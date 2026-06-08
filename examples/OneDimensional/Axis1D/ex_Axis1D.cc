// ----------------------------------------------------------------------------
// File: ex_Axis1D.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Demonstrates basic construction and inspection of Axis1D.
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
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>

int main() {
    const fvgrid::Axis1D axis{
        std::vector<fvgrid::Real>{0.0, 0.25, 0.5, 1.0}
    };

    std::cout << "Axis1D example\n";
    std::cout << "number of faces : " << axis.num_faces() << '\n';
    std::cout << "number of cells : " << axis.num_cells() << '\n';
    std::cout << "xmin            : " << axis.xmin() << '\n';
    std::cout << "xmax            : " << axis.xmax() << '\n';
    std::cout << "length          : " << axis.length() << '\n';

    std::cout << "faces           : ";
    for (const fvgrid::Real face : axis.faces()) {
        std::cout << face << ' ';
    }
    std::cout << '\n';

    std::cout << "centers         : ";
    for (const fvgrid::Real center : axis.centers()) {
        std::cout << center << ' ';
    }
    std::cout << '\n';

    std::cout << "cell lengths    : ";
    for (const fvgrid::Real cell_length : axis.cell_lengths()) {
        std::cout << cell_length << ' ';
    }
    std::cout << '\n';

    return 0;
}