// ----------------------------------------------------------------------------
// File: ID.h
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Defines a lightweight immutable identity for FVGridMaker classes.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <string_view>

namespace fvgrid {

class ID final {
public:
    constexpr ID() noexcept = default;

    constexpr ID(
        std::string_view module,
        std::string_view class_name,
        std::string_view class_id
    ) noexcept
        : module_(module),
          class_name_(class_name),
          class_id_(class_id) {}

    [[nodiscard]] constexpr std::string_view module() const noexcept {
        return module_;
    }

    [[nodiscard]] constexpr std::string_view class_name() const noexcept {
        return class_name_;
    }

    [[nodiscard]] constexpr std::string_view class_id() const noexcept {
        return class_id_;
    }

private:
    std::string_view module_ = "Unknown";
    std::string_view class_name_ = "Unknown";
    std::string_view class_id_ = "fvgrid.unknown.Unknown";
};

}  // namespace fvgrid

