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
#include <concepts>
#include <string>
#include <vector>

namespace fvgrid {

template <std::floating_point T>
struct BasicAxisGeometry1D final {
    using value_type = T;

    std::vector<T> faces;
    std::vector<T> centers;
    std::string pattern_name;
};

using AxisGeometry1D = BasicAxisGeometry1D<double>;
using AxisGeometry1DFloat = BasicAxisGeometry1D<float>;
using AxisGeometry1DLongDouble = BasicAxisGeometry1D<long double>;

}  // namespace fvgrid
