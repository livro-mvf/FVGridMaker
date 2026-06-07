// ----------------------------------------------------------------------------
// File: ErrorRecord.h
// Project: FVGridMaker
// Version: 0.1.0
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

namespace fvgrid {

struct ErrorRecord final {
    std::string_view code;
    std::string message;
    std::string_view module;
    std::source_location location = std::source_location::current();
};

}  // namespace fvgrid