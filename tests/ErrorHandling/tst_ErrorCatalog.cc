// ----------------------------------------------------------------------------
// File: tst_ErrorCatalog.cc
// Project: FVGridMaker
// Version: 0.1.0
// Description: Tests the built-in FVGridMaker error catalogue.
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
#include <FVGridMaker/ErrorHandling/ErrorCatalog.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvgrid {

TEST(ErrorCatalog, CoreDescriptorsAreStable) {
    EXPECT_EQ(error_catalog::kInvalidArgument.code, error_code::kInvalidArgument);
    EXPECT_EQ(
        error_catalog::kInvalidArgument.message,
        std::string_view{"Invalid argument."}
    );
    EXPECT_EQ(error_catalog::kInvalidArgument.category, std::string_view{"Core"});

    EXPECT_EQ(error_catalog::kOutOfRange.code, error_code::kOutOfRange);
    EXPECT_EQ(
        error_catalog::kOutOfRange.message,
        std::string_view{"Value is out of range."}
    );
    EXPECT_EQ(error_catalog::kOutOfRange.category, std::string_view{"Core"});

    EXPECT_EQ(error_catalog::kNotImplemented.code, error_code::kNotImplemented);
    EXPECT_EQ(
        error_catalog::kNotImplemented.message,
        std::string_view{"Requested functionality is not implemented."}
    );
    EXPECT_EQ(error_catalog::kNotImplemented.category, std::string_view{"Core"});

    EXPECT_EQ(error_catalog::kInternalError.code, error_code::kInternalError);
    EXPECT_EQ(
        error_catalog::kInternalError.message,
        std::string_view{"Internal FVGridMaker error."}
    );
    EXPECT_EQ(error_catalog::kInternalError.category, std::string_view{"Core"});
}

TEST(ErrorCatalog, GridInputDescriptorsAreStable) {
    EXPECT_EQ(error_catalog::kInvalidNVol.code, error_code::kInvalidNVol);
    EXPECT_EQ(
        error_catalog::kInvalidNVol.message,
        std::string_view{"The number of control volumes must be positive."}
    );
    EXPECT_EQ(error_catalog::kInvalidNVol.category, std::string_view{"Grid"});

    EXPECT_EQ(error_catalog::kInvalidLength.code, error_code::kInvalidLength);
    EXPECT_EQ(
        error_catalog::kInvalidLength.message,
        std::string_view{"The physical length must be positive."}
    );
    EXPECT_EQ(error_catalog::kInvalidLength.category, std::string_view{"Grid"});

    EXPECT_EQ(error_catalog::kInvalidXInit.code, error_code::kInvalidXInit);
    EXPECT_EQ(
        error_catalog::kInvalidXInit.message,
        std::string_view{"The initial coordinate is invalid."}
    );
    EXPECT_EQ(error_catalog::kInvalidXInit.category, std::string_view{"Grid"});

    EXPECT_EQ(
        error_catalog::kInvalidMinSpacing.code,
        error_code::kInvalidMinSpacing
    );
    EXPECT_EQ(
        error_catalog::kInvalidMinSpacing.message,
        std::string_view{"The minimum spacing must be non-negative."}
    );
    EXPECT_EQ(
        error_catalog::kInvalidMinSpacing.category,
        std::string_view{"Grid"}
    );

    EXPECT_EQ(
        error_catalog::kImpossibleMinSpacing.code,
        error_code::kImpossibleMinSpacing
    );
    EXPECT_EQ(
        error_catalog::kImpossibleMinSpacing.message,
        std::string_view{
            "The minimum spacing is too large for the requested domain."
        }
    );
    EXPECT_EQ(
        error_catalog::kImpossibleMinSpacing.category,
        std::string_view{"Grid"}
    );

    EXPECT_EQ(
        error_catalog::kInvalidCoordinateKind.code,
        error_code::kInvalidCoordinateKind
    );
    EXPECT_EQ(
        error_catalog::kInvalidCoordinateKind.message,
        std::string_view{
            "The input coordinate kind is incompatible with the grid pattern."
        }
    );
    EXPECT_EQ(
        error_catalog::kInvalidCoordinateKind.category,
        std::string_view{"Grid"}
    );
}

TEST(ErrorCatalog, CoordinateOrderDescriptorsAreStable) {
    EXPECT_EQ(
        error_catalog::kNonIncreasingFaces.code,
        error_code::kNonIncreasingFaces
    );
    EXPECT_EQ(
        error_catalog::kNonIncreasingFaces.message,
        std::string_view{"Face coordinates must be strictly increasing."}
    );
    EXPECT_EQ(
        error_catalog::kNonIncreasingFaces.category,
        std::string_view{"Grid"}
    );

    EXPECT_EQ(
        error_catalog::kNonIncreasingCenters.code,
        error_code::kNonIncreasingCenters
    );
    EXPECT_EQ(
        error_catalog::kNonIncreasingCenters.message,
        std::string_view{"Centre coordinates must be strictly increasing."}
    );
    EXPECT_EQ(
        error_catalog::kNonIncreasingCenters.category,
        std::string_view{"Grid"}
    );
}

TEST(ErrorCatalog, CountDescriptorsAreStable) {
    EXPECT_EQ(
        error_catalog::kInvalidFaceCount.code,
        error_code::kInvalidFaceCount
    );
    EXPECT_EQ(
        error_catalog::kInvalidFaceCount.message,
        std::string_view{"The number of face coordinates is invalid."}
    );
    EXPECT_EQ(
        error_catalog::kInvalidFaceCount.category,
        std::string_view{"Grid"}
    );

    EXPECT_EQ(
        error_catalog::kInvalidCenterCount.code,
        error_code::kInvalidCenterCount
    );
    EXPECT_EQ(
        error_catalog::kInvalidCenterCount.message,
        std::string_view{"The number of centre coordinates is invalid."}
    );
    EXPECT_EQ(
        error_catalog::kInvalidCenterCount.category,
        std::string_view{"Grid"}
    );

    EXPECT_EQ(
        error_catalog::kInvalidDeltaFaceCount.code,
        error_code::kInvalidDeltaFaceCount
    );
    EXPECT_EQ(
        error_catalog::kInvalidDeltaFaceCount.message,
        std::string_view{"The number of face spacings is invalid."}
    );
    EXPECT_EQ(
        error_catalog::kInvalidDeltaFaceCount.category,
        std::string_view{"Grid"}
    );

    EXPECT_EQ(
        error_catalog::kInvalidDeltaCenterCount.code,
        error_code::kInvalidDeltaCenterCount
    );
    EXPECT_EQ(
        error_catalog::kInvalidDeltaCenterCount.message,
        std::string_view{"The number of centre spacings is invalid."}
    );
    EXPECT_EQ(
        error_catalog::kInvalidDeltaCenterCount.category,
        std::string_view{"Grid"}
    );
}

TEST(ErrorCatalog, OperationDescriptorsAreStable) {
    EXPECT_EQ(
        error_catalog::kIncompatibleGridPatterns.code,
        error_code::kIncompatibleGridPatterns
    );
    EXPECT_EQ(
        error_catalog::kIncompatibleGridPatterns.message,
        std::string_view{"The grid patterns are incompatible."}
    );
    EXPECT_EQ(
        error_catalog::kIncompatibleGridPatterns.category,
        std::string_view{"Operation"}
    );

    EXPECT_EQ(
        error_catalog::kEmptyGridIntersection.code,
        error_code::kEmptyGridIntersection
    );
    EXPECT_EQ(
        error_catalog::kEmptyGridIntersection.message,
        std::string_view{"The geometric intersection between grids is empty."}
    );
    EXPECT_EQ(
        error_catalog::kEmptyGridIntersection.category,
        std::string_view{"Operation"}
    );

    EXPECT_EQ(error_catalog::kInvalidTolerance.code, error_code::kInvalidTolerance);
    EXPECT_EQ(
        error_catalog::kInvalidTolerance.message,
        std::string_view{"The tolerance value is invalid."}
    );
    EXPECT_EQ(
        error_catalog::kInvalidTolerance.category,
        std::string_view{"Operation"}
    );
}

TEST(ErrorCatalog, OutputDescriptorsAreStable) {
    EXPECT_EQ(
        error_catalog::kOutputFileOpenFailed.code,
        error_code::kOutputFileOpenFailed
    );
    EXPECT_EQ(
        error_catalog::kOutputFileOpenFailed.message,
        std::string_view{"Output file could not be opened."}
    );
    EXPECT_EQ(
        error_catalog::kOutputFileOpenFailed.category,
        std::string_view{"Output"}
    );

    EXPECT_EQ(
        error_catalog::kOutputFileWriteFailed.code,
        error_code::kOutputFileWriteFailed
    );
    EXPECT_EQ(
        error_catalog::kOutputFileWriteFailed.message,
        std::string_view{"Output file could not be written."}
    );
    EXPECT_EQ(
        error_catalog::kOutputFileWriteFailed.category,
        std::string_view{"Output"}
    );
}

TEST(ErrorCatalog, YamlDescriptorsAreStable) {
    EXPECT_EQ(
        error_catalog::kInvalidYamlNode.code,
        error_code::kInvalidYamlNode
    );
    EXPECT_EQ(
        error_catalog::kInvalidYamlNode.message,
        std::string_view{"The YAML node is invalid."}
    );
    EXPECT_EQ(error_catalog::kInvalidYamlNode.category, std::string_view{"YAML"});

    EXPECT_EQ(
        error_catalog::kMissingYamlKey.code,
        error_code::kMissingYamlKey
    );
    EXPECT_EQ(
        error_catalog::kMissingYamlKey.message,
        std::string_view{"A required YAML key is missing."}
    );
    EXPECT_EQ(error_catalog::kMissingYamlKey.category, std::string_view{"YAML"});

    EXPECT_EQ(
        error_catalog::kUnknownYamlPolicy.code,
        error_code::kUnknownYamlPolicy
    );
    EXPECT_EQ(
        error_catalog::kUnknownYamlPolicy.message,
        std::string_view{"The YAML policy is unknown."}
    );
    EXPECT_EQ(
        error_catalog::kUnknownYamlPolicy.category,
        std::string_view{"YAML"}
    );
}

}  // namespace fvgrid