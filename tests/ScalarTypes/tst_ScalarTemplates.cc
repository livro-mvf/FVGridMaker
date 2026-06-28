// ============================================================================
// Arquivo: tst_ScalarTemplates.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Testa o componente ScalarTemplates no contexto de ScalarTypes.
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
#include <concepts>
#include <filesystem>
#include <fstream>
#include <limits>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/ErrorHandling/FVGridException.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Custom1D.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Random1D.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Roberts1D.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Coordinates1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/FaceCentered1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>
#include <FVGridMaker/OneDimensional/Quality1D/Quality1D.h>
#include <FVGridMaker/Output/CSV/Axis1DCSVWriter.h>
#include <FVGridMaker/Output/VTK/LegacyVTKRectilinearGrid2DWriter.h>
#include <FVGridMaker/Output/VTK/LegacyVTKStructuredGrid2DWriter.h>
#include <FVGridMaker/TwoDimensional/CoordinateSystem2D/CoordinateSystem2D.h>
#include <FVGridMaker/TwoDimensional/Quality2D/Quality2D.h>
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

namespace fvgrid {
namespace {

template <std::floating_point T>
constexpr T scalar(long double value) noexcept {
    return static_cast<T>(value);
}

template <std::floating_point T>
double tolerance() {
    return static_cast<double>(
        scalar<T>(256.0L) * std::numeric_limits<T>::epsilon()
    );
}

template <std::floating_point T>
void expect_near_scalar(T actual, T expected) {
    EXPECT_NEAR(
        static_cast<double>(actual),
        static_cast<double>(expected),
        tolerance<T>()
    );
}

std::string read_text_file(const std::filesystem::path& filepath) {
    std::ifstream input{filepath};
    std::ostringstream buffer;
    buffer << input.rdbuf();

    return buffer.str();
}

template <std::floating_point T>
void expect_basic_axis_type_works() {
    using Axis = BasicAxis1D<T>;

    Axis axis{std::vector<T>{scalar<T>(0.0L), scalar<T>(1.0L), scalar<T>(3.0L)}};

    static_assert(std::same_as<typename Axis::value_type, T>);

    EXPECT_EQ(axis.num_cells(), static_cast<Size>(2));
    EXPECT_EQ(axis.num_faces(), static_cast<Size>(3));
    expect_near_scalar(axis.center(static_cast<Size>(0)), scalar<T>(0.5L));
    expect_near_scalar(axis.center(static_cast<Size>(1)), scalar<T>(2.0L));
    expect_near_scalar(axis.length(), scalar<T>(3.0L));
}

template <std::floating_point T>
void expect_axis_rejects_center_outside_own_cell() {
    const std::vector<T> faces{
        scalar<T>(0.0L),
        scalar<T>(1.0L),
        scalar<T>(2.0L),
        scalar<T>(3.0L)
    };
    const std::vector<T> centers{
        scalar<T>(0.5L),
        scalar<T>(2.5L),
        scalar<T>(2.75L)
    };

    EXPECT_THROW(
        (BasicAxis1D<T>{faces, centers, "Invalid"}),
        FVGridException
    );
}

template <std::floating_point T>
void expect_distribution_builders_preserve_type() {
    const auto uniform = BasicUniform1D<T>::make(
        NVol{static_cast<Size>(4)},
        BasicLength<T>{scalar<T>(2.0L)},
        BasicXInit<T>{scalar<T>(-1.0L)}
    );

    static_assert(std::same_as<decltype(uniform), const BasicAxis1D<T>>);

    EXPECT_EQ(uniform.num_cells(), static_cast<Size>(4));
    EXPECT_EQ(uniform.pattern_name(), FaceCentered1D::name());
    expect_near_scalar(uniform.xmin(), scalar<T>(-1.0L));
    expect_near_scalar(uniform.xmax(), scalar<T>(1.0L));

    const auto uniform_from_values = uniform_axis_1d<T>(
        static_cast<Size>(4),
        scalar<T>(-1.0L),
        scalar<T>(1.0L)
    );

    static_assert(
        std::same_as<decltype(uniform_from_values), const BasicAxis1D<T>>
    );

    expect_near_scalar(
        uniform_from_values.cell_length(static_cast<Size>(0)),
        scalar<T>(0.5L)
    );

    const auto random = BasicRandom1D<T>::make(
        NVol{static_cast<Size>(5)},
        BasicLength<T>{scalar<T>(1.0L)},
        BasicXInit<T>{scalar<T>(0.0L)},
        Seed{42}
    );

    static_assert(std::same_as<decltype(random), const BasicAxis1D<T>>);

    EXPECT_EQ(random.num_cells(), static_cast<Size>(5));
    EXPECT_EQ(random.pattern_name(), FaceCentered1D::name());
    expect_near_scalar(random.xmin(), scalar<T>(0.0L));
    expect_near_scalar(random.xmax(), scalar<T>(1.0L));

    const auto random_from_values = random_axis_1d<T>(
        static_cast<Size>(5),
        scalar<T>(0.0L),
        scalar<T>(1.0L),
        static_cast<UInt64>(42)
    );

    static_assert(
        std::same_as<decltype(random_from_values), const BasicAxis1D<T>>
    );

    EXPECT_EQ(random_from_values.num_cells(), static_cast<Size>(5));

    const auto roberts = BasicRoberts1D<T>::make(
        NVol{static_cast<Size>(4)},
        BasicLength<T>{scalar<T>(1.0L)},
        BasicXInit<T>{scalar<T>(0.0L)},
        BasicBeta<T>{scalar<T>(1.2L)}
    );

    static_assert(std::same_as<decltype(roberts), const BasicAxis1D<T>>);

    EXPECT_EQ(roberts.num_cells(), static_cast<Size>(4));
    EXPECT_EQ(roberts.pattern_name(), FaceCentered1D::name());
    expect_near_scalar(roberts.xmin(), scalar<T>(0.0L));
    expect_near_scalar(roberts.xmax(), scalar<T>(1.0L));

    const auto roberts_from_values = roberts_axis_1d<T>(
        static_cast<Size>(4),
        scalar<T>(0.0L),
        scalar<T>(1.0L),
        scalar<T>(1.2L)
    );

    static_assert(
        std::same_as<decltype(roberts_from_values), const BasicAxis1D<T>>
    );

    EXPECT_EQ(roberts_from_values.num_cells(), static_cast<Size>(4));

    const auto custom = BasicCustom1D<T>::make(
        BasicCoordinates1D<T>::faces(
            std::vector<T>{scalar<T>(0.0L), scalar<T>(0.25L), scalar<T>(1.0L)}
        ),
        VolumeCentered1D{}
    );

    static_assert(std::same_as<decltype(custom), const BasicAxis1D<T>>);

    EXPECT_EQ(custom.num_cells(), static_cast<Size>(2));
    expect_near_scalar(custom.center(static_cast<Size>(0)), scalar<T>(0.125L));
    expect_near_scalar(custom.center(static_cast<Size>(1)), scalar<T>(0.625L));
}

template <std::floating_point T>
void expect_grid_and_quality_preserve_type() {
    auto x_axis = uniform_axis_1d<T>(
        static_cast<Size>(2),
        scalar<T>(0.0L),
        scalar<T>(2.0L)
    );
    auto y_axis = uniform_axis_1d<T>(
        static_cast<Size>(3),
        scalar<T>(0.0L),
        scalar<T>(3.0L)
    );

    const BasicStructuredGrid2D<T> grid{
        std::move(x_axis),
        std::move(y_axis)
    };

    static_assert(std::same_as<typename BasicStructuredGrid2D<T>::value_type, T>);

    EXPECT_EQ(grid.num_cells_x(), static_cast<Size>(2));
    EXPECT_EQ(grid.num_cells_y(), static_cast<Size>(3));
    EXPECT_EQ(grid.num_cells(), static_cast<Size>(6));
    expect_near_scalar(
        grid.cell_logical_area(static_cast<Size>(0), static_cast<Size>(0)),
        scalar<T>(1.0L)
    );
    expect_near_scalar(
        grid.cell_measure(static_cast<Size>(0), static_cast<Size>(0)),
        scalar<T>(1.0L)
    );

    const BasicQualityMetrics1D<T> axis_quality =
        Quality1D::metrics(grid.x_axis());

    static_assert(
        std::same_as<
            typename std::remove_cvref_t<decltype(axis_quality)>::value_type,
            T
        >
    );

    expect_near_scalar(axis_quality.min_cell_length, scalar<T>(1.0L));
    expect_near_scalar(axis_quality.max_cell_length, scalar<T>(1.0L));
    expect_near_scalar(axis_quality.cell_length_ratio, scalar<T>(1.0L));

    const BasicQualityReport2D<T> grid_quality = Quality2D::evaluate(grid);

    static_assert(
        std::same_as<
            typename std::remove_cvref_t<decltype(grid_quality)>::value_type,
            T
        >
    );

    expect_near_scalar(grid_quality.min_cell_measure, scalar<T>(1.0L));
    expect_near_scalar(grid_quality.max_cell_measure, scalar<T>(1.0L));
    expect_near_scalar(grid_quality.cell_measure_ratio, scalar<T>(1.0L));
}

template <std::floating_point T>
void expect_csv_writer_accepts_axis_type() {
    const BasicAxis1D<T> axis{
        std::vector<T>{scalar<T>(0.0L), scalar<T>(0.5L), scalar<T>(1.0L)}
    };

    std::ostringstream output;
    Axis1DCSVWriter::write(axis, output);

    const std::string csv = output.str();

    EXPECT_NE(csv.find("i,xF,xC,Dx,dx\n"), std::string::npos);
    EXPECT_NE(csv.find("0.25"), std::string::npos);
    EXPECT_NE(csv.find("0.75"), std::string::npos);
}

template <std::floating_point T>
BasicStructuredGrid2D<T> make_rectilinear_grid() {
    return BasicStructuredGrid2D<T>{
        BasicAxis1D<T>{
            std::vector<T>{scalar<T>(0.0L), scalar<T>(1.0L), scalar<T>(2.0L)}
        },
        BasicAxis1D<T>{
            std::vector<T>{scalar<T>(0.0L), scalar<T>(1.0L), scalar<T>(2.0L)}
        }
    };
}

}  // namespace

TEST(ScalarTemplates, AliasesPreserveDefaultDoubleApiAndExposeScalarVariants) {
    static_assert(std::same_as<Length, BasicLength<double>>);
    static_assert(std::same_as<XInit, BasicXInit<double>>);
    static_assert(std::same_as<XFinal, BasicXFinal<double>>);
    static_assert(std::same_as<MinSpacing, BasicMinSpacing<double>>);
    static_assert(std::same_as<Beta, BasicBeta<double>>);

    static_assert(std::same_as<LengthFloat, BasicLength<float>>);
    static_assert(std::same_as<XInitFloat, BasicXInit<float>>);
    static_assert(std::same_as<XFinalFloat, BasicXFinal<float>>);
    static_assert(std::same_as<MinSpacingFloat, BasicMinSpacing<float>>);
    static_assert(std::same_as<BetaFloat, BasicBeta<float>>);

    static_assert(std::same_as<LengthLongDouble, BasicLength<long double>>);
    static_assert(std::same_as<XInitLongDouble, BasicXInit<long double>>);
    static_assert(std::same_as<XFinalLongDouble, BasicXFinal<long double>>);
    static_assert(std::same_as<MinSpacingLongDouble, BasicMinSpacing<long double>>);
    static_assert(std::same_as<BetaLongDouble, BasicBeta<long double>>);

    static_assert(std::same_as<Axis1D, BasicAxis1D<double>>);
    static_assert(std::same_as<Axis1DFloat, BasicAxis1D<float>>);
    static_assert(std::same_as<Axis1DLongDouble, BasicAxis1D<long double>>);

    static_assert(std::same_as<Uniform1D, BasicUniform1D<double>>);
    static_assert(std::same_as<Uniform1DFloat, BasicUniform1D<float>>);
    static_assert(std::same_as<Uniform1DLongDouble, BasicUniform1D<long double>>);

    static_assert(std::same_as<Random1D, BasicRandom1D<double>>);
    static_assert(std::same_as<Random1DFloat, BasicRandom1D<float>>);
    static_assert(std::same_as<Random1DLongDouble, BasicRandom1D<long double>>);

    static_assert(std::same_as<Roberts1D, BasicRoberts1D<double>>);
    static_assert(std::same_as<Roberts1DFloat, BasicRoberts1D<float>>);
    static_assert(std::same_as<Roberts1DLongDouble, BasicRoberts1D<long double>>);

    static_assert(std::same_as<Custom1D, BasicCustom1D<double>>);
    static_assert(std::same_as<Custom1DFloat, BasicCustom1D<float>>);
    static_assert(std::same_as<Custom1DLongDouble, BasicCustom1D<long double>>);

    static_assert(std::same_as<StructuredGrid2D, BasicStructuredGrid2D<double>>);
    static_assert(std::same_as<StructuredGrid2DFloat, BasicStructuredGrid2D<float>>);
    static_assert(
        std::same_as<StructuredGrid2DLongDouble, BasicStructuredGrid2D<long double>>
    );

    SUCCEED();
}

TEST(ScalarTemplates, AxisSupportsFloatDoubleAndLongDouble) {
    expect_basic_axis_type_works<float>();
    expect_basic_axis_type_works<double>();
    expect_basic_axis_type_works<long double>();
}

TEST(ScalarTemplates, AxisInvariantRejectsCentersOutsideTheirOwnCells) {
    expect_axis_rejects_center_outside_own_cell<float>();
    expect_axis_rejects_center_outside_own_cell<double>();
    expect_axis_rejects_center_outside_own_cell<long double>();
}

TEST(ScalarTemplates, DistributionBuildersPreserveRequestedScalarType) {
    expect_distribution_builders_preserve_type<float>();
    expect_distribution_builders_preserve_type<double>();
    expect_distribution_builders_preserve_type<long double>();
}

TEST(ScalarTemplates, DoubleConvenienceFunctionsKeepLegacyReturnTypes) {
    const auto uniform = uniform_axis_1d(
        static_cast<Size>(2),
        0.0,
        1.0
    );
    const auto random = random_axis_1d(
        static_cast<Size>(2),
        0.0,
        1.0,
        static_cast<UInt64>(7)
    );
    const auto roberts = roberts_axis_1d(
        static_cast<Size>(2),
        0.0,
        1.0,
        1.2
    );

    static_assert(std::same_as<decltype(uniform), const Axis1D>);
    static_assert(std::same_as<decltype(random), const Axis1D>);
    static_assert(std::same_as<decltype(roberts), const Axis1D>);

    EXPECT_EQ(uniform.num_cells(), static_cast<Size>(2));
    EXPECT_EQ(random.num_cells(), static_cast<Size>(2));
    EXPECT_EQ(roberts.num_cells(), static_cast<Size>(2));
}

TEST(ScalarTemplates, StructuredGridAndQualitySupportFloatDoubleAndLongDouble) {
    expect_grid_and_quality_preserve_type<float>();
    expect_grid_and_quality_preserve_type<double>();
    expect_grid_and_quality_preserve_type<long double>();
}

TEST(ScalarTemplates, CsvWriterAcceptsFloatDoubleAndLongDoubleAxes) {
    expect_csv_writer_accepts_axis_type<float>();
    expect_csv_writer_accepts_axis_type<double>();
    expect_csv_writer_accepts_axis_type<long double>();
}

TEST(ScalarTemplates, RectilinearVtkWriterUsesRequestedScalarTypeName) {
    const auto float_grid = make_rectilinear_grid<float>();
    const auto double_grid = make_rectilinear_grid<double>();

    const std::filesystem::path float_path =
        std::filesystem::temp_directory_path() /
        "fvgridmaker_scalar_rectilinear_float.vtk";
    const std::filesystem::path double_path =
        std::filesystem::temp_directory_path() /
        "fvgridmaker_scalar_rectilinear_double.vtk";

    std::filesystem::remove(float_path);
    std::filesystem::remove(double_path);

    LegacyVTKRectilinearGrid2DWriter::write(float_grid, float_path);
    LegacyVTKRectilinearGrid2DWriter::write(double_grid, double_path);

    const std::string float_content = read_text_file(float_path);
    const std::string double_content = read_text_file(double_path);

    EXPECT_NE(float_content.find("X_COORDINATES 3 float"), std::string::npos);
    EXPECT_NE(
        float_content.find("SCALARS cell_measure float 1"),
        std::string::npos
    );
    EXPECT_NE(double_content.find("X_COORDINATES 3 double"), std::string::npos);
    EXPECT_NE(
        double_content.find("SCALARS cell_measure double 1"),
        std::string::npos
    );

    std::filesystem::remove(float_path);
    std::filesystem::remove(double_path);
}

TEST(ScalarTemplates, StructuredVtkWriterAcceptsFloatMappedGrid) {
    const BasicStructuredGrid2D<float> grid{
        BasicAxis1D<float>{std::vector<float>{1.0F, 2.0F}},
        BasicAxis1D<float>{std::vector<float>{0.0F, 1.0F}},
        BasicPolarCoordinates2D<float>{}
    };

    const std::filesystem::path output_path =
        std::filesystem::temp_directory_path() /
        "fvgridmaker_scalar_structured_float.vtk";

    std::filesystem::remove(output_path);

    LegacyVTKStructuredGrid2DWriter::write(grid, output_path);

    const std::string content = read_text_file(output_path);

    EXPECT_NE(content.find("POINTS 4 float"), std::string::npos);
    EXPECT_NE(
        content.find("SCALARS cell_measure float 1"),
        std::string::npos
    );

    std::filesystem::remove(output_path);
}

}  // namespace fvgrid