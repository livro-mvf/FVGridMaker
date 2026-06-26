// ----------------------------------------------------------------------------
// File: Version.cc
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Provides the compiled version entry point for FVGridMaker.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/Version.h>

namespace fvgrid {

const char* version() noexcept {
    return kVersionString.data();
}


}  // namespace fvgrid
