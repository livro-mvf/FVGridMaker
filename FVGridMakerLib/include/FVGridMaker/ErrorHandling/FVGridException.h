// ----------------------------------------------------------------------------
// File: FVGridException.h
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Defines the base exception type used by FVGridMaker.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <exception>
#include <string>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/ErrorRecord.h>

namespace fvgrid {

class FVGridException final : public std::exception {
public:
    explicit FVGridException(ErrorRecord record);

    [[nodiscard]] const char* what() const noexcept override;

    [[nodiscard]] const ErrorRecord& record() const noexcept;

private:
    ErrorRecord record_;
    std::string formatted_message_;
};

}  // namespace fvgrid
