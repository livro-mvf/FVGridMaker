// ----------------------------------------------------------------------------
// File: FVGridMaker.h
// Project: FVGridMaker
// Version: 0.1.0
// Description: Provides the root public include for the FVGridMaker library.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
// #include <FVGridMaker/Core/ID.h>
// #include <FVGridMaker/Core/StrongTypes.h>
// #include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/Core/Version.h>
// #include <FVGridMaker/ErrorHandling/ErrorCodes.h>
// #include <FVGridMaker/ErrorHandling/ErrorRecord.h>
// #include <FVGridMaker/ErrorHandling/FVGridException.h>
// #include <FVGridMaker/ErrorHandling/ThrowError.h>

namespace fvgrid {

[[nodiscard]] const char* version() noexcept;

}  // namespace fvgrid