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
#include <string>
#include <utility>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/ThrowError.h>

namespace fvgrid {

[[noreturn]] void throw_error(
    ErrorDescriptor descriptor,
    ID source,
    std::source_location location
) {
    throw_error(
        descriptor.code,
        std::string{descriptor.message},
        descriptor.category,
        source,
        location
    );
}

[[noreturn]] void throw_error(
    std::string_view code,
    std::string message,
    std::string_view category,
    ID source,
    std::source_location location
) {
    throw FVGridException{
        ErrorRecord{
            .code = code,
            .message = std::move(message),
            .category = category,
            .source = source,
            .location = location,
        }
    };
}

void require(
    bool condition,
    ErrorDescriptor descriptor,
    ID source,
    std::source_location location
) {
    if (!condition) {
        throw_error(descriptor, source, location);
    }
}

void require(
    bool condition,
    std::string_view code,
    std::string message,
    std::string_view category,
    ID source,
    std::source_location location
) {
    if (!condition) {
        throw_error(code, std::move(message), category, source, location);
    }
}

}  // namespace fvgrid