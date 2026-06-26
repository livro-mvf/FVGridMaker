// ----------------------------------------------------------------------------
// File: ErrorRecord.h
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Defines the structured error record used by FVGridMaker.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------


#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <source_location>
#include <string>
#include <string_view>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/ErrorHandling/ErrorContext.h>

namespace fvgrid {

struct ErrorRecord final {
    std::string_view code;
    std::string message;
    std::string_view category;
    ErrorContextList context;
    ID source;
    std::source_location location = std::source_location::current();
};

}  // namespace fvgrid
