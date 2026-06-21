// ----------------------------------------------------------------------------
// File: AxisGeometry1D.h
// Project: FVGridMaker
// Version: 0.1.0
// Description: Defines completed one-dimensional axis geometry data.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <string>
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/Types.h>

namespace fvgrid {

struct AxisGeometry1D final {
    std::vector<Real> faces;
    std::vector<Real> centers;
    std::string pattern_name;
};

}  // namespace fvgrid