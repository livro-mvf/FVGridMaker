// ----------------------------------------------------------------------------
// File: FVGridException.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Implements the base exception type used by FVGridMaker.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <sstream>
#include <utility>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/FVGridException.h>

namespace fvgrid {

namespace {

[[nodiscard]] std::string format_error_message(const ErrorRecord& record) {
    std::ostringstream stream;

    stream << "[" << record.code << "] " << record.message;

    if (!record.category.empty()) {
        stream << " (category: " << record.category << ")";
    }

    if (!record.source.module().empty()) {
        stream << " (module: " << record.source.module() << ")";
    }

    if (!record.source.class_name().empty()) {
        stream << " (class: " << record.source.class_name() << ")";
    }

    if (!record.source.class_id().empty()) {
        stream << " (class id: " << record.source.class_id() << ")";
    }

    stream << " at " << record.location.file_name() << ":"
           << record.location.line() << " in "
           << record.location.function_name();

    return stream.str();
}

}  // namespace

FVGridException::FVGridException(ErrorRecord record)
    : record_(std::move(record)),
      formatted_message_(format_error_message(record_)) {}

const char* FVGridException::what() const noexcept {
    return formatted_message_.c_str();
}

const ErrorRecord& FVGridException::record() const noexcept {
    return record_;
}

}  // namespace fvgrid