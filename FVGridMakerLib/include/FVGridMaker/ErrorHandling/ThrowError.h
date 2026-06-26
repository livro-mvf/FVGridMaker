// ----------------------------------------------------------------------------
// File: ThrowError.h
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Declares and defines helper functions for throwing FVGridMaker
//              exceptions.
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
#include <utility>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/ErrorHandling/ErrorContext.h>
#include <FVGridMaker/ErrorHandling/ErrorDescriptor.h>
#include <FVGridMaker/ErrorHandling/ErrorTraits.h>
#include <FVGridMaker/ErrorHandling/FVGridException.h>

namespace fvgrid {

// ----------------------------------------------------------------------------
// Runtime overloads.
//
// These overloads keep the error system open to user-defined or externally
// generated descriptors without requiring a built-in error tag.
// ----------------------------------------------------------------------------

[[noreturn]] inline void throw_error(
    std::string_view code,
    std::string message,
    std::string_view category,
    ID source,
    ErrorContextList context,
    std::source_location location = std::source_location::current()
) {
    throw FVGridException{
        ErrorRecord{
            .code = code,
            .message = std::move(message),
            .category = category,
            .context = std::move(context),
            .source = source,
            .location = location,
        }
    };
}

[[noreturn]] inline void throw_error(
    ErrorDescriptor descriptor,
    ID source,
    ErrorContextList context,
    std::source_location location = std::source_location::current()
) {
    throw_error(
        descriptor.code,
        std::string{descriptor.message},
        descriptor.category,
        source,
        std::move(context),
        location
    );
}

[[noreturn]] inline void throw_error(
    std::string_view code,
    std::string message,
    std::string_view category,
    ID source,
    std::source_location location = std::source_location::current()
) {
    throw_error(
        code,
        std::move(message),
        category,
        source,
        ErrorContextList{},
        location
    );
}

[[noreturn]] inline void throw_error(
    ErrorDescriptor descriptor,
    ID source,
    std::source_location location = std::source_location::current()
) {
    throw_error(
        descriptor,
        source,
        ErrorContextList{},
        location
    );
}

inline void require(
    bool condition,
    std::string_view code,
    std::string message,
    std::string_view category,
    ID source,
    ErrorContextList context,
    std::source_location location = std::source_location::current()
) {
    if (!condition) {
        throw_error(
            code,
            std::move(message),
            category,
            source,
            std::move(context),
            location
        );
    }
}

inline void require(
    bool condition,
    ErrorDescriptor descriptor,
    ID source,
    ErrorContextList context,
    std::source_location location = std::source_location::current()
) {
    if (!condition) {
        throw_error(
            descriptor,
            source,
            std::move(context),
            location
        );
    }
}

inline void require(
    bool condition,
    std::string_view code,
    std::string message,
    std::string_view category,
    ID source,
    std::source_location location = std::source_location::current()
) {
    if (!condition) {
        throw_error(
            code,
            std::move(message),
            category,
            source,
            location
        );
    }
}

inline void require(
    bool condition,
    ErrorDescriptor descriptor,
    ID source,
    std::source_location location = std::source_location::current()
) {
    if (!condition) {
        throw_error(
            descriptor,
            source,
            location
        );
    }
}

// ----------------------------------------------------------------------------
// Typed overloads.
//
// These overloads are the preferred FVGridMaker API. They accept any type that
// satisfies ErrorDescriptorProvider, including built-in FVGridMaker error tags
// and user-defined error tags.
// ----------------------------------------------------------------------------

template <ErrorDescriptorProvider Error>
[[noreturn]] void throw_error(
    ID source,
    ErrorContextList context,
    std::source_location location = std::source_location::current()
) {
    throw_error(
        descriptor_of<Error>(),
        source,
        std::move(context),
        location
    );
}


template <ErrorDescriptorProvider Error>
[[noreturn]] void throw_error(
    ID source,
    std::source_location location = std::source_location::current()
) {
    throw_error<Error>(
        source,
        ErrorContextList{},
        location
    );
}

template <ErrorDescriptorProvider Error>
void require(
    bool condition,
    ID source,
    ErrorContextList context,
    std::source_location location = std::source_location::current()
) {
    if (!condition) {
        throw_error<Error>(
            source,
            std::move(context),
            location
        );
    }
}

template <ErrorDescriptorProvider Error>
void require(
    bool condition,
    ID source,
    std::source_location location = std::source_location::current()
) {
    if (!condition) {
        throw_error<Error>(
            source,
            location
        );
    }
}

}  // namespace fvgrid