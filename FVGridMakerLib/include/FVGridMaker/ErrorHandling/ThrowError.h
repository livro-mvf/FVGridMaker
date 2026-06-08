// ----------------------------------------------------------------------------
// File: ThrowError.h
// Project: FVGridMaker
// Version: 0.1.0
// Description: Declares helper functions for throwing FVGridMaker exceptions.
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
#include <FVGridMaker/ErrorHandling/ErrorDescriptor.h>
#include <FVGridMaker/ErrorHandling/FVGridException.h>

namespace fvgrid {

[[noreturn]] void throw_error(
    ErrorDescriptor descriptor,
    ID source,
    std::source_location location = std::source_location::current()
);

[[noreturn]] void throw_error(
    std::string_view code,
    std::string message,
    std::string_view category,
    ID source,
    std::source_location location = std::source_location::current()
);

void require(
    bool condition,
    ErrorDescriptor descriptor,
    ID source,
    std::source_location location = std::source_location::current()
);

void require(
    bool condition,
    std::string_view code,
    std::string message,
    std::string_view category,
    ID source,
    std::source_location location = std::source_location::current()
);

}  // namespace fvgrid