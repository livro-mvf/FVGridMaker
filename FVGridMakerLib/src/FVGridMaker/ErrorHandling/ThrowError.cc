// ----------------------------------------------------------------------------
// File: ThrowError.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Implements helper functions for throwing FVGridMaker exceptions.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <utility>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/ThrowError.h>

namespace fvgrid {

void throw_error(
    std::string_view code,
    std::string message,
    std::string_view module,
    std::source_location location
) {
    throw FVGridException{
        ErrorRecord{
            .code = code,
            .message = std::move(message),
            .module = module,
            .location = location,
        }
    };
}

void require(
    bool condition,
    std::string_view code,
    std::string message,
    std::string_view module,
    std::source_location location
) {
    if (!condition) {
        throw_error(code, std::move(message), module, location);
    }
}

}  // namespace fvgrid