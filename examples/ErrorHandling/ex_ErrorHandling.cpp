// ----------------------------------------------------------------------------
// File: ex_ErrorHandling.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Demonstrates basic FVGridMaker error handling.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <iostream>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/ErrorCodes.h>
#include <FVGridMaker/ErrorHandling/FVGridException.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>

int main() {
    try {
        fvgrid::require(
            false,
            fvgrid::error_code::kInvalidArgument,
            "The demonstration condition failed.",
            "Ex_ErrorHandling"
        );
    } catch (const fvgrid::FVGridException& exception) {
        std::cout << "Caught FVGridException\n";
        std::cout << "code   : " << exception.record().code << '\n';
        std::cout << "module : " << exception.record().module << '\n';
        std::cout << "message: " << exception.record().message << '\n';
        std::cout << "what   : " << exception.what() << '\n';
        return 0;
    }

    return 1;
}