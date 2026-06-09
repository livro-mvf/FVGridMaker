// ----------------------------------------------------------------------------
// File: ErrorCodes.h
// Project: FVGridMaker
// Version: 0.1.0
// Description: Defines stable textual error codes used by FVGridMaker.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <string_view>

namespace fvgrid::error_code {

inline constexpr std::string_view kInvalidArgument =
    "FVGRID.CORE.INVALID_ARGUMENT";

inline constexpr std::string_view kOutOfRange =
    "FVGRID.CORE.OUT_OF_RANGE";

inline constexpr std::string_view kNotImplemented =
    "FVGRID.CORE.NOT_IMPLEMENTED";

inline constexpr std::string_view kInternalError =
    "FVGRID.CORE.INTERNAL_ERROR";

inline constexpr std::string_view kInvalidNVol =
    "FVGRID.GRID.INVALID_NVOL";

inline constexpr std::string_view kInvalidLength =
    "FVGRID.GRID.INVALID_LENGTH";

inline constexpr std::string_view kInvalidXInit =
    "FVGRID.GRID.INVALID_XINIT";

inline constexpr std::string_view kInvalidMinSpacing =
    "FVGRID.GRID.INVALID_MIN_SPACING";

inline constexpr std::string_view kImpossibleMinSpacing =
    "FVGRID.GRID.IMPOSSIBLE_MIN_SPACING";

inline constexpr std::string_view kInvalidCoordinateKind =
    "FVGRID.GRID.INVALID_COORDINATE_KIND";

inline constexpr std::string_view kNonIncreasingFaces =
    "FVGRID.GRID.NON_INCREASING_FACES";

inline constexpr std::string_view kNonIncreasingCenters =
    "FVGRID.GRID.NON_INCREASING_CENTERS";

inline constexpr std::string_view kInvalidFaceCount =
    "FVGRID.GRID.INVALID_FACE_COUNT";

inline constexpr std::string_view kInvalidCenterCount =
    "FVGRID.GRID.INVALID_CENTER_COUNT";

inline constexpr std::string_view kInvalidDeltaFaceCount =
    "FVGRID.GRID.INVALID_DELTA_FACE_COUNT";

inline constexpr std::string_view kInvalidDeltaCenterCount =
    "FVGRID.GRID.INVALID_DELTA_CENTER_COUNT";

inline constexpr std::string_view kIncompatibleGridPatterns =
    "FVGRID.OPERATION.INCOMPATIBLE_GRID_PATTERNS";

inline constexpr std::string_view kEmptyGridIntersection =
    "FVGRID.OPERATION.EMPTY_GRID_INTERSECTION";

inline constexpr std::string_view kInvalidTolerance =
    "FVGRID.OPERATION.INVALID_TOLERANCE";

inline constexpr std::string_view kInvalidYamlNode =
    "FVGRID.YAML.INVALID_NODE";

inline constexpr std::string_view kMissingYamlKey =
    "FVGRID.YAML.MISSING_KEY";

inline constexpr std::string_view kUnknownYamlPolicy =
    "FVGRID.YAML.UNKNOWN_POLICY";

}  // namespace fvgrid::error_code