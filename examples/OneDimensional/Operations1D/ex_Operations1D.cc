// ----------------------------------------------------------------------------
// File: ex_Operations1D.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Demonstrates interval and coordinate operations on 1D axes.
// Author: FVGridMaker Team
// License: MIT
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
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>
#include <FVGridMaker/OneDimensional/Operations1D/AxisInterval1D.h>
#include <FVGridMaker/OneDimensional/Operations1D/Operations1D.h>

namespace {

void print_interval(
    const fvgrid::AxisInterval1D& interval
) {
    if (interval.is_empty()) {
        std::cout << "intersection: empty\n";
        return;
    }

    if (interval.is_point()) {
        std::cout << "intersection: point at x = "
                  << interval.lower()
                  << '\n';
        return;
    }

    std::cout << "intersection: interval ["
              << interval.lower()
              << ", "
              << interval.upper()
              << "]\n";
}

}  // namespace

int main() {
    const fvgrid::Axis1D left = fvgrid::Uniform1D::make(
        fvgrid::NVol{6},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0}
    );

    const fvgrid::Axis1D right = fvgrid::Uniform1D::make(
        fvgrid::NVol{6},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.35}
    );

    std::cout << std::fixed << std::setprecision(6);

    std::cout << "Left axis\n";
    std::cout << left << "\n\n";

    std::cout << "Right axis\n";
    std::cout << right << "\n\n";

    const fvgrid::AxisInterval1D intersection =
        fvgrid::Operations1D::intersection(left, right);

    print_interval(intersection);
    std::cout << '\n';

    if (intersection.is_interval()) {
        const fvgrid::Axis1D clipped =
            fvgrid::Operations1D::clip_faces_to_interval(
                left,
                intersection,
                1.0e-12
            );

        std::cout << "Left axis clipped to the intersection interval\n";
        std::cout << clipped << '\n';
    }

    return 0;
}