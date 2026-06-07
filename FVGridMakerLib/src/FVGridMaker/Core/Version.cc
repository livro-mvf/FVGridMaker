// ----------------------------------------------------------------------------
// File: Version.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Provides the compiled version entry point for FVGridMaker.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/Version.h>
#include <FVGridMaker/FVGridMaker.h>

namespace fvgrid {

const char* version() noexcept {
    return kVersionString.data();
}

}  // namespace fvgrid