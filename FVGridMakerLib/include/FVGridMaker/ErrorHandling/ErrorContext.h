// ----------------------------------------------------------------------------
// File: ErrorContext.h
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Defines structured diagnostic context entries for FVGridMaker
//              errors.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------


#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <string>
#include <string_view>
#include <vector>

namespace fvgrid {

struct ErrorContext final {
    std::string key;
    std::string value;
};

using ErrorContextList = std::vector<ErrorContext>;

[[nodiscard]] inline ErrorContext make_error_context(
    std::string_view key,
    std::string_view value
) {
    return ErrorContext{
        .key = std::string{key},
        .value = std::string{value},
    };
}

}  // namespace fvgrid
