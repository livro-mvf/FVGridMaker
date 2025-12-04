// ----------------------------------------------------------------------------
// File: Ex_UniformGrid1D.cpp
// Purpose: Example of building a 1D uniform grid using Grid1DBuilder.
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// includes FVMGridMaker (alphabetical by path)
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/Common/namespace.hpp>
#include <FVGridMaker/Core/Common/types.hpp>
#include <FVGridMaker/Grid/Common/Tags1D.hpp>
#include <FVGridMaker/Grid/Grid1D/API/Grid1D.h>
#include <FVGridMaker/Grid/Grid1D/Builders/Grid1DBuilder.hpp>
#include <FVGridMaker/Grid/Grid1D/Patterns/Distribution/Uniform1D.hpp>

// ----------------------------------------------------------------------------
// includes C++ (alphabetical)
// ----------------------------------------------------------------------------
#include <iostream>

int main() {
    using FVGridMaker::core::Index;
    using FVGridMaker::core::Real;

    using FVGridMaker::grid::CenteringTag;
    using FVGridMaker::grid::DistributionTag;
    using FVGridMaker::grid::grid1d::api::Grid1D;
    using FVGridMaker::grid::grid1d::builders::Grid1DBuilder;
    using FVGridMaker::grid::grid1d::patterns::distribution::Uniform1D;

    // ------------------------------------------------------------------------
    // 1. Basic configuration
    // ------------------------------------------------------------------------
    const Index N      = 20;   // number of physical control volumes
    const Real  A      = 0.0;  // left physical limit
    const Real  B      = 1.0;  // right physical limit
    const Index NGhost = 1;    // number of ghost cells at each side (0, 1, 2, ...)

    // ------------------------------------------------------------------------
    // 2. Build a uniform, cell-centred grid with optional ghost cells
    // ------------------------------------------------------------------------
    Grid1D grid = Grid1DBuilder{}
        .setN(N)
        .setDomain(A, B)
        .setNGhost(NGhost)
        .setDistribution<Uniform1D>()
        .build();

    // Avoid unused-variable warnings if Grid1D does not yet expose accessors.
    (void)grid;

    // ------------------------------------------------------------------------
    // 3. Basic output (only using local configuration parameters)
    // ------------------------------------------------------------------------
    std::cout << "=== Grid1D basic info ===\n";
    std::cout << "Physical cells (N)      : " << N << '\n';
    std::cout << "Ghost cells per side    : " << NGhost << '\n';
    std::cout << "Physical domain         : [" << A << ", " << B << "]\n";
    std::cout << "Physical length         : " << (B - A) << "\n\n";

    std::cout << "Grid1D object successfully built.\n";

    std::cout << grid << '\n';

    return 0;
}
