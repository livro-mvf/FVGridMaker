// ----------------------------------------------------------------------------
// File: ErrorCatalog.h
// Project: FVGridMaker
// Version: 0.1.0
// Description: Defines the built-in FVGridMaker error catalogue.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/ErrorCodes.h>
#include <FVGridMaker/ErrorHandling/ErrorDescriptor.h>

namespace fvgrid::error_catalog {

inline constexpr ErrorDescriptor kInvalidArgument{
    .code = ::fvgrid::error_code::kInvalidArgument,
    .message = "Invalid argument.",
    .category = "Core",
};

inline constexpr ErrorDescriptor kOutOfRange{
    .code = ::fvgrid::error_code::kOutOfRange,
    .message = "Value is out of range.",
    .category = "Core",
};

inline constexpr ErrorDescriptor kNotImplemented{
    .code = ::fvgrid::error_code::kNotImplemented,
    .message = "Requested functionality is not implemented.",
    .category = "Core",
};

inline constexpr ErrorDescriptor kInternalError{
    .code = ::fvgrid::error_code::kInternalError,
    .message = "Internal FVGridMaker error.",
    .category = "Core",
};

inline constexpr ErrorDescriptor kInvalidNVol{
    .code = ::fvgrid::error_code::kInvalidNVol,
    .message = "The number of control volumes must be positive.",
    .category = "Grid",
};

inline constexpr ErrorDescriptor kInvalidLength{
    .code = ::fvgrid::error_code::kInvalidLength,
    .message = "The physical length must be positive.",
    .category = "Grid",
};

inline constexpr ErrorDescriptor kInvalidXInit{
    .code = ::fvgrid::error_code::kInvalidXInit,
    .message = "The initial coordinate is invalid.",
    .category = "Grid",
};

inline constexpr ErrorDescriptor kInvalidMinSpacing{
    .code = ::fvgrid::error_code::kInvalidMinSpacing,
    .message = "The minimum spacing must be non-negative.",
    .category = "Grid",
};

inline constexpr ErrorDescriptor kImpossibleMinSpacing{
    .code = ::fvgrid::error_code::kImpossibleMinSpacing,
    .message =
        "The minimum spacing is too large for the requested domain.",
    .category = "Grid",
};

inline constexpr ErrorDescriptor kInvalidCoordinateKind{
    .code = ::fvgrid::error_code::kInvalidCoordinateKind,
    .message =
        "The input coordinate kind is incompatible with the grid pattern.",
    .category = "Grid",
};

inline constexpr ErrorDescriptor kNonIncreasingFaces{
    .code = ::fvgrid::error_code::kNonIncreasingFaces,
    .message = "Face coordinates must be strictly increasing.",
    .category = "Grid",
};

inline constexpr ErrorDescriptor kNonIncreasingCenters{
    .code = ::fvgrid::error_code::kNonIncreasingCenters,
    .message = "Centre coordinates must be strictly increasing.",
    .category = "Grid",
};

inline constexpr ErrorDescriptor kInvalidFaceCount{
    .code = ::fvgrid::error_code::kInvalidFaceCount,
    .message = "The number of face coordinates is invalid.",
    .category = "Grid",
};

inline constexpr ErrorDescriptor kInvalidCenterCount{
    .code = ::fvgrid::error_code::kInvalidCenterCount,
    .message = "The number of centre coordinates is invalid.",
    .category = "Grid",
};

inline constexpr ErrorDescriptor kInvalidDeltaFaceCount{
    .code = ::fvgrid::error_code::kInvalidDeltaFaceCount,
    .message = "The number of face spacings is invalid.",
    .category = "Grid",
};

inline constexpr ErrorDescriptor kInvalidDeltaCenterCount{
    .code = ::fvgrid::error_code::kInvalidDeltaCenterCount,
    .message = "The number of centre spacings is invalid.",
    .category = "Grid",
};

inline constexpr ErrorDescriptor kIncompatibleGridPatterns{
    .code = ::fvgrid::error_code::kIncompatibleGridPatterns,
    .message = "The grid patterns are incompatible.",
    .category = "Operation",
};

inline constexpr ErrorDescriptor kEmptyGridIntersection{
    .code = ::fvgrid::error_code::kEmptyGridIntersection,
    .message = "The geometric intersection between grids is empty.",
    .category = "Operation",
};

inline constexpr ErrorDescriptor kInvalidTolerance{
    .code = ::fvgrid::error_code::kInvalidTolerance,
    .message = "The tolerance value is invalid.",
    .category = "Operation",
};

inline constexpr ErrorDescriptor kOutputFileOpenFailed{
    .code = ::fvgrid::error_code::kOutputFileOpenFailed,
    .message = "Output file could not be opened.",
    .category = "Output",
};

inline constexpr ErrorDescriptor kOutputFileWriteFailed{
    .code = ::fvgrid::error_code::kOutputFileWriteFailed,
    .message = "Output file could not be written.",
    .category = "Output",
};

inline constexpr ErrorDescriptor kInvalidYamlNode{
    .code = ::fvgrid::error_code::kInvalidYamlNode,
    .message = "The YAML node is invalid.",
    .category = "YAML",
};

inline constexpr ErrorDescriptor kMissingYamlKey{
    .code = ::fvgrid::error_code::kMissingYamlKey,
    .message = "A required YAML key is missing.",
    .category = "YAML",
};

inline constexpr ErrorDescriptor kUnknownYamlPolicy{
    .code = ::fvgrid::error_code::kUnknownYamlPolicy,
    .message = "The YAML policy is unknown.",
    .category = "YAML",
};

}  // namespace fvgrid::error_catalog