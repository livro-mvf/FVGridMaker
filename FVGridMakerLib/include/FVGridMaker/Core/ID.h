// ----------------------------------------------------------------------------
// File: ID.h
// Project: FVGridMaker
// Version: 0.1.0
// Description: Defines a lightweight base type for stable textual class
//              identification in FVGridMaker.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <string_view>

namespace fvgrid {

class ID {
public:
    constexpr ID() noexcept = default;
    constexpr ID(const ID&) noexcept = default;
    constexpr ID(ID&&) noexcept = default;

    constexpr ID& operator=(const ID&) noexcept = default;
    constexpr ID& operator=(ID&&) noexcept = default;

    ~ID() noexcept = default;

    [[nodiscard]] constexpr std::string_view class_name() const noexcept {
        return "ID";
    }

    [[nodiscard]] constexpr std::string_view class_id() const noexcept {
        return "fvgrid.core.ID";
    }
};

}  // namespace fvgrid