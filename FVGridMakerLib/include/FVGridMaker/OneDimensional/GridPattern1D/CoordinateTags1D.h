// ----------------------------------------------------------------------------
// File: CoordinateTags1D.h
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Defines type tags for one-dimensional primary coordinates.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <concepts>

namespace fvgrid {

struct FaceCoordinates1D final {};

struct CenterCoordinates1D final {};

template <class Tag>
concept PrimaryCoordinateTag1D =
    std::same_as<Tag, FaceCoordinates1D> ||
    std::same_as<Tag, CenterCoordinates1D>;

}  // namespace fvgrid