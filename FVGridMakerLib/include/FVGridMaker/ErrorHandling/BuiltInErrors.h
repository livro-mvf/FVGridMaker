// ----------------------------------------------------------------------------
// File: BuiltInErrors.h
// Project: FVGridMaker
// Version: see <FVGridMaker/Core/Version.h>
// Description: Defines the official built-in FVGridMaker errors as typed,
//              constexpr descriptor providers.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------


#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <string_view>

namespace fvgrid::errors::core {

struct InvalidArgument final {
    static constexpr std::string_view code =
        "FVGRID.CORE.INVALID_ARGUMENT";

    static constexpr std::string_view message =
        "Invalid argument.";

    static constexpr std::string_view category =
        "Core";
};

struct OutOfRange final {
    static constexpr std::string_view code =
        "FVGRID.CORE.OUT_OF_RANGE";

    static constexpr std::string_view message =
        "Value is out of range.";

    static constexpr std::string_view category =
        "Core";
};

struct NotImplemented final {
    static constexpr std::string_view code =
        "FVGRID.CORE.NOT_IMPLEMENTED";

    static constexpr std::string_view message =
        "Requested functionality is not implemented.";

    static constexpr std::string_view category =
        "Core";
};

struct InternalError final {
    static constexpr std::string_view code =
        "FVGRID.CORE.INTERNAL_ERROR";

    static constexpr std::string_view message =
        "Internal FVGridMaker error.";

    static constexpr std::string_view category =
        "Core";
};

}  // namespace fvgrid::errors::core

namespace fvgrid::errors::grid {

struct InvalidNVol final {
    static constexpr std::string_view code =
        "FVGRID.GRID.INVALID_NVOL";

    static constexpr std::string_view message =
        "The number of control volumes must be positive.";

    static constexpr std::string_view category =
        "Grid";
};

struct InvalidLength final {
    static constexpr std::string_view code =
        "FVGRID.GRID.INVALID_LENGTH";

    static constexpr std::string_view message =
        "The physical length must be positive.";

    static constexpr std::string_view category =
        "Grid";
};

struct InvalidXInit final {
    static constexpr std::string_view code =
        "FVGRID.GRID.INVALID_XINIT";

    static constexpr std::string_view message =
        "The initial coordinate is invalid.";

    static constexpr std::string_view category =
        "Grid";
};

struct InvalidMinSpacing final {
    static constexpr std::string_view code =
        "FVGRID.GRID.INVALID_MIN_SPACING";

    static constexpr std::string_view message =
        "The minimum spacing must be non-negative.";

    static constexpr std::string_view category =
        "Grid";
};

struct ImpossibleMinSpacing final {
    static constexpr std::string_view code =
        "FVGRID.GRID.IMPOSSIBLE_MIN_SPACING";

    static constexpr std::string_view message =
        "The minimum spacing is too large for the requested domain.";

    static constexpr std::string_view category =
        "Grid";
};

struct InvalidBeta final {
    static constexpr std::string_view code =
        "FVGRID.GRID.INVALID_BETA";

    static constexpr std::string_view message =
        "The Roberts stretching parameter beta must be greater than one.";

    static constexpr std::string_view category =
        "Grid";
};

struct InvalidCoordinateKind final {
    static constexpr std::string_view code =
        "FVGRID.GRID.INVALID_COORDINATE_KIND";

    static constexpr std::string_view message =
        "The input coordinate kind is incompatible with the grid pattern.";

    static constexpr std::string_view category =
        "Grid";
};

struct NonIncreasingFaces final {
    static constexpr std::string_view code =
        "FVGRID.GRID.NON_INCREASING_FACES";

    static constexpr std::string_view message =
        "Face coordinates must be strictly increasing.";

    static constexpr std::string_view category =
        "Grid";
};

struct NonIncreasingCenters final {
    static constexpr std::string_view code =
        "FVGRID.GRID.NON_INCREASING_CENTERS";

    static constexpr std::string_view message =
        "Centre coordinates must be strictly increasing.";

    static constexpr std::string_view category =
        "Grid";
};

struct InvalidFaceCount final {
    static constexpr std::string_view code =
        "FVGRID.GRID.INVALID_FACE_COUNT";

    static constexpr std::string_view message =
        "The number of face coordinates is invalid.";

    static constexpr std::string_view category =
        "Grid";
};

struct InvalidCenterCount final {
    static constexpr std::string_view code =
        "FVGRID.GRID.INVALID_CENTER_COUNT";

    static constexpr std::string_view message =
        "The number of centre coordinates is invalid.";

    static constexpr std::string_view category =
        "Grid";
};

struct InvalidDeltaFaceCount final {
    static constexpr std::string_view code =
        "FVGRID.GRID.INVALID_DELTA_FACE_COUNT";

    static constexpr std::string_view message =
        "The number of face spacings is invalid.";

    static constexpr std::string_view category =
        "Grid";
};

struct InvalidDeltaCenterCount final {
    static constexpr std::string_view code =
        "FVGRID.GRID.INVALID_DELTA_CENTER_COUNT";

    static constexpr std::string_view message =
        "The number of centre spacings is invalid.";

    static constexpr std::string_view category =
        "Grid";
};

}  // namespace fvgrid::errors::grid

namespace fvgrid::errors::operation {

struct IncompatibleGridPatterns final {
    static constexpr std::string_view code =
        "FVGRID.OPERATION.INCOMPATIBLE_GRID_PATTERNS";

    static constexpr std::string_view message =
        "The grid patterns are incompatible.";

    static constexpr std::string_view category =
        "Operation";
};

struct EmptyGridIntersection final {
    static constexpr std::string_view code =
        "FVGRID.OPERATION.EMPTY_GRID_INTERSECTION";

    static constexpr std::string_view message =
        "The geometric intersection between grids is empty.";

    static constexpr std::string_view category =
        "Operation";
};

struct InvalidTolerance final {
    static constexpr std::string_view code =
        "FVGRID.OPERATION.INVALID_TOLERANCE";

    static constexpr std::string_view message =
        "The tolerance value is invalid.";

    static constexpr std::string_view category =
        "Operation";
};

}  // namespace fvgrid::errors::operation

namespace fvgrid::errors::output {

struct FileOpenFailed final {
    static constexpr std::string_view code =
        "FVGRID.OUTPUT.FILE_OPEN_FAILED";

    static constexpr std::string_view message =
        "Output file could not be opened.";

    static constexpr std::string_view category =
        "Output";
};

struct FileWriteFailed final {
    static constexpr std::string_view code =
        "FVGRID.OUTPUT.FILE_WRITE_FAILED";

    static constexpr std::string_view message =
        "Output file could not be written.";

    static constexpr std::string_view category =
        "Output";
};

}  // namespace fvgrid::errors::output
