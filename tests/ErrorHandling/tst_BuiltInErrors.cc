// ============================================================================
// Arquivo: tst_BuiltInErrors.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Testa o componente BuiltInErrors no contexto de ErrorHandling.
// Autor: João Flávio Vieira de Vasconcellos
//
// SPDX-FileCopyrightText: 2026 João Flávio Vieira de Vasconcellos
// SPDX-License-Identifier: BSD-3-Clause
//
// Este arquivo faz parte do FVGridMaker.
//
// Licença: BSD 3-Clause.
// É permitido usar, copiar, modificar e redistribuir este arquivo, em código-fonte
// ou forma binária, com ou sem modificações, desde que sejam preservados os avisos
// de copyright, esta identificação de licença e as condições descritas no arquivo
// LICENSE.
//
// O nome do autor, de colaboradores ou de instituições associadas ao projeto não
// pode ser usado para endossar ou promover produtos derivados sem autorização
// prévia por escrito.
//
// Este software é fornecido sem garantias de qualquer natureza. Consulte o arquivo
// LICENSE, na raiz do repositório, para o texto completo da licença.
// ============================================================================
// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <string_view>
#include <type_traits>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/BuiltInErrors.h>
#include <FVGridMaker/ErrorHandling/ErrorTraits.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>


namespace fvgrid {

TEST(BuiltInErrors, CoreErrorsAreStable) {
    static_assert(ErrorDescriptorProvider<errors::core::InvalidArgument>);
    static_assert(ErrorDescriptorProvider<errors::core::OutOfRange>);
    static_assert(ErrorDescriptorProvider<errors::core::NotImplemented>);
    static_assert(ErrorDescriptorProvider<errors::core::InternalError>);

    EXPECT_EQ(
        errors::core::InvalidArgument::code,
        std::string_view{"FVGRID.CORE.INVALID_ARGUMENT"}
    );
    EXPECT_EQ(
        errors::core::OutOfRange::code,
        std::string_view{"FVGRID.CORE.OUT_OF_RANGE"}
    );
    EXPECT_EQ(
        errors::core::NotImplemented::code,
        std::string_view{"FVGRID.CORE.NOT_IMPLEMENTED"}
    );
    EXPECT_EQ(
        errors::core::InternalError::code,
        std::string_view{"FVGRID.CORE.INTERNAL_ERROR"}
    );
}

TEST(BuiltInErrors, GridInputErrorsAreStable) {
    static_assert(ErrorDescriptorProvider<errors::grid::InvalidNVol>);
    static_assert(ErrorDescriptorProvider<errors::grid::InvalidLength>);
    static_assert(ErrorDescriptorProvider<errors::grid::InvalidXInit>);
    static_assert(ErrorDescriptorProvider<errors::grid::InvalidMinSpacing>);
    static_assert(ErrorDescriptorProvider<errors::grid::ImpossibleMinSpacing>);
    static_assert(ErrorDescriptorProvider<errors::grid::InvalidBeta>);
    static_assert(ErrorDescriptorProvider<errors::grid::InvalidCoordinateKind>);

    EXPECT_EQ(
        errors::grid::InvalidNVol::code,
        std::string_view{"FVGRID.GRID.INVALID_NVOL"}
    );
    EXPECT_EQ(
        errors::grid::InvalidLength::code,
        std::string_view{"FVGRID.GRID.INVALID_LENGTH"}
    );
    EXPECT_EQ(
        errors::grid::InvalidXInit::code,
        std::string_view{"FVGRID.GRID.INVALID_XINIT"}
    );
    EXPECT_EQ(
        errors::grid::InvalidMinSpacing::code,
        std::string_view{"FVGRID.GRID.INVALID_MIN_SPACING"}
    );
    EXPECT_EQ(
        errors::grid::ImpossibleMinSpacing::code,
        std::string_view{"FVGRID.GRID.IMPOSSIBLE_MIN_SPACING"}
    );
    EXPECT_EQ(
        errors::grid::InvalidBeta::code,
        std::string_view{"FVGRID.GRID.INVALID_BETA"}
    );
    EXPECT_EQ(
        errors::grid::InvalidCoordinateKind::code,
        std::string_view{"FVGRID.GRID.INVALID_COORDINATE_KIND"}
    );
}

TEST(BuiltInErrors, CoordinateValidationErrorsAreStable) {
    static_assert(ErrorDescriptorProvider<errors::grid::NonIncreasingFaces>);
    static_assert(ErrorDescriptorProvider<errors::grid::NonIncreasingCenters>);
    static_assert(ErrorDescriptorProvider<errors::grid::InvalidFaceCount>);
    static_assert(ErrorDescriptorProvider<errors::grid::InvalidCenterCount>);
    static_assert(ErrorDescriptorProvider<errors::grid::InvalidDeltaFaceCount>);
    static_assert(
        ErrorDescriptorProvider<errors::grid::InvalidDeltaCenterCount>
    );

    EXPECT_EQ(
        errors::grid::NonIncreasingFaces::code,
        std::string_view{"FVGRID.GRID.NON_INCREASING_FACES"}
    );
    EXPECT_EQ(
        errors::grid::NonIncreasingCenters::code,
        std::string_view{"FVGRID.GRID.NON_INCREASING_CENTERS"}
    );
    EXPECT_EQ(
        errors::grid::InvalidFaceCount::code,
        std::string_view{"FVGRID.GRID.INVALID_FACE_COUNT"}
    );
    EXPECT_EQ(
        errors::grid::InvalidCenterCount::code,
        std::string_view{"FVGRID.GRID.INVALID_CENTER_COUNT"}
    );
    EXPECT_EQ(
        errors::grid::InvalidDeltaFaceCount::code,
        std::string_view{"FVGRID.GRID.INVALID_DELTA_FACE_COUNT"}
    );
    EXPECT_EQ(
        errors::grid::InvalidDeltaCenterCount::code,
        std::string_view{"FVGRID.GRID.INVALID_DELTA_CENTER_COUNT"}
    );
}

TEST(BuiltInErrors, OperationErrorsAreStable) {
    static_assert(
        ErrorDescriptorProvider<errors::operation::IncompatibleGridPatterns>
    );
    static_assert(
        ErrorDescriptorProvider<errors::operation::EmptyGridIntersection>
    );
    static_assert(ErrorDescriptorProvider<errors::operation::InvalidTolerance>);

    EXPECT_EQ(
        errors::operation::IncompatibleGridPatterns::code,
        std::string_view{"FVGRID.OPERATION.INCOMPATIBLE_GRID_PATTERNS"}
    );
    EXPECT_EQ(
        errors::operation::EmptyGridIntersection::code,
        std::string_view{"FVGRID.OPERATION.EMPTY_GRID_INTERSECTION"}
    );
    EXPECT_EQ(
        errors::operation::InvalidTolerance::code,
        std::string_view{"FVGRID.OPERATION.INVALID_TOLERANCE"}
    );
}

TEST(BuiltInErrors, OutputErrorsAreStable) {
    static_assert(ErrorDescriptorProvider<errors::output::FileOpenFailed>);
    static_assert(ErrorDescriptorProvider<errors::output::FileWriteFailed>);

    EXPECT_EQ(
        errors::output::FileOpenFailed::code,
        std::string_view{"FVGRID.OUTPUT.FILE_OPEN_FAILED"}
    );
    EXPECT_EQ(
        errors::output::FileWriteFailed::code,
        std::string_view{"FVGRID.OUTPUT.FILE_WRITE_FAILED"}
    );
}

TEST(BuiltInErrors, BuiltInErrorsAreFinalAndNotPolymorphic) {
    static_assert(std::is_final_v<errors::grid::InvalidNVol>);
    static_assert(!std::is_polymorphic_v<errors::grid::InvalidNVol>);
    static_assert(std::is_final_v<errors::output::FileOpenFailed>);
    static_assert(!std::is_polymorphic_v<errors::output::FileOpenFailed>);

    SUCCEED();
}

}  // namespace fvgrid
