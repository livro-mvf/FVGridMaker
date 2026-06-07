// ----------------------------------------------------------------------------
// File: tst_ErrorCodes.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Tests the stable textual error codes used by FVGridMaker.
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <string_view>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/ErrorCodes.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

TEST(ErrorCodes, CoreCodesAreStable) {
    EXPECT_EQ(error_code::kInvalidArgument, std::string_view{"FVGRID.CORE.INVALID_ARGUMENT"});
    EXPECT_EQ(error_code::kOutOfRange, std::string_view{"FVGRID.CORE.OUT_OF_RANGE"});
    EXPECT_EQ(error_code::kNotImplemented, std::string_view{"FVGRID.CORE.NOT_IMPLEMENTED"});
    EXPECT_EQ(error_code::kInternalError, std::string_view{"FVGRID.CORE.INTERNAL_ERROR"});
}

TEST(ErrorCodes, GridCodesAreStable) {
    EXPECT_EQ(error_code::kInvalidNVol, std::string_view{"FVGRID.GRID.INVALID_NVOL"});
    EXPECT_EQ(error_code::kInvalidLength, std::string_view{"FVGRID.GRID.INVALID_LENGTH"});
    EXPECT_EQ(error_code::kInvalidXInit, std::string_view{"FVGRID.GRID.INVALID_XINIT"});
    EXPECT_EQ(error_code::kNonIncreasingFaces,
              std::string_view{"FVGRID.GRID.NON_INCREASING_FACES"});
    EXPECT_EQ(error_code::kNonIncreasingCenters,
              std::string_view{"FVGRID.GRID.NON_INCREASING_CENTERS"});
}

TEST(ErrorCodes, GridCountCodesAreStable) {
    EXPECT_EQ(error_code::kInvalidFaceCount,
              std::string_view{"FVGRID.GRID.INVALID_FACE_COUNT"});
    EXPECT_EQ(error_code::kInvalidCenterCount,
              std::string_view{"FVGRID.GRID.INVALID_CENTER_COUNT"});
    EXPECT_EQ(error_code::kInvalidDeltaFaceCount,
              std::string_view{"FVGRID.GRID.INVALID_DELTA_FACE_COUNT"});
    EXPECT_EQ(error_code::kInvalidDeltaCenterCount,
              std::string_view{"FVGRID.GRID.INVALID_DELTA_CENTER_COUNT"});
}

TEST(ErrorCodes, OperationCodesAreStable) {
    EXPECT_EQ(error_code::kIncompatibleGridPatterns,
              std::string_view{"FVGRID.OPERATION.INCOMPATIBLE_GRID_PATTERNS"});
    EXPECT_EQ(error_code::kEmptyGridIntersection,
              std::string_view{"FVGRID.OPERATION.EMPTY_GRID_INTERSECTION"});
    EXPECT_EQ(error_code::kInvalidTolerance,
              std::string_view{"FVGRID.OPERATION.INVALID_TOLERANCE"});
}

TEST(ErrorCodes, YamlCodesAreStable) {
    EXPECT_EQ(error_code::kInvalidYamlNode, std::string_view{"FVGRID.YAML.INVALID_NODE"});
    EXPECT_EQ(error_code::kMissingYamlKey, std::string_view{"FVGRID.YAML.MISSING_KEY"});
    EXPECT_EQ(error_code::kUnknownYamlPolicy, std::string_view{"FVGRID.YAML.UNKNOWN_POLICY"});
}

}  // namespace fvgrid