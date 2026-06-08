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
#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/ErrorHandling/ErrorCatalog.h>
#include <FVGridMaker/ErrorHandling/FVGridException.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>

int main() {
    const fvgrid::ID example_id{
        "Examples",
        "Ex_ErrorHandling",
        "fvgrid.examples.Ex_ErrorHandling"
    };

    try {
        fvgrid::require(
            false,
            fvgrid::error_catalog::kInvalidArgument,
            example_id
        );
    } catch (const fvgrid::FVGridException& exception) {
        const fvgrid::ErrorRecord& record = exception.record();

        std::cout << "Caught FVGridException\n";
        std::cout << "code       : " << record.code << '\n';
        std::cout << "category   : " << record.category << '\n';
        std::cout << "module     : " << record.source.module() << '\n';
        std::cout << "class name : " << record.source.class_name() << '\n';
        std::cout << "class id   : " << record.source.class_id() << '\n';
        std::cout << "message    : " << record.message << '\n';
        std::cout << "what       : " << exception.what() << '\n';

        return 0;
    }

    return 1;
}