// ----------------------------------------------------------------------------
// File: ErrorDescriptor.h
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Defines immutable descriptors for FVGridMaker error codes.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------


#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <string_view>

namespace fvgrid {

struct ErrorDescriptor final {
    std::string_view code;
    std::string_view message;
    std::string_view category;
};

}  // namespace fvgrid
