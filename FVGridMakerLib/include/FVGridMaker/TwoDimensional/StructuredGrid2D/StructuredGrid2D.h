// ============================================================================
// Arquivo: StructuredGrid2D.h
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Declara a malha estruturada 2D construída por composição de dois eixos 1D.
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
#pragma once

#include <cmath>
#include <concepts>
#include <iosfwd>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/ErrorHandling/BuiltInErrors.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/TwoDimensional/CoordinateSystem2D/CoordinateMetrics2D.h>
#include <FVGridMaker/TwoDimensional/CoordinateSystem2D/CoordinateSystem2D.h>

namespace fvgrid {

template <std::floating_point T>
class BasicStructuredGrid2D final {
public:
    using value_type = T;

    BasicStructuredGrid2D(BasicAxis1D<T> first_axis, BasicAxis1D<T> second_axis);

    template <class Mapping>
        requires CoordinateMapping2DFor<Mapping, T>
    BasicStructuredGrid2D(
        BasicAxis1D<T> first_axis,
        BasicAxis1D<T> second_axis,
        const Mapping& mapping
    )
        : first_axis_(std::move(first_axis)),
          second_axis_(std::move(second_axis)),
          coordinate_system_name_(mapping.name()),
          first_coordinate_name_(mapping.first_coordinate_name()),
          second_coordinate_name_(mapping.second_coordinate_name()),
          vtk_rectilinear_(mapping.vtk_rectilinear()) {
        validate_axis_patterns();
        build_derived_geometry(mapping);
    }

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "TwoDimensional",
            "StructuredGrid2D",
            "fvgrid.2d.grid.StructuredGrid2D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] std::string_view coordinate_system_name() const noexcept;
    [[nodiscard]] std::string_view first_coordinate_name() const noexcept;
    [[nodiscard]] std::string_view second_coordinate_name() const noexcept;
    [[nodiscard]] bool vtk_rectilinear() const noexcept;

    [[nodiscard]] const BasicAxis1D<T>& first_axis() const noexcept;
    [[nodiscard]] const BasicAxis1D<T>& second_axis() const noexcept;
    [[nodiscard]] const BasicAxis1D<T>& x_axis() const noexcept;
    [[nodiscard]] const BasicAxis1D<T>& y_axis() const noexcept;

    [[nodiscard]] Size num_cells_x() const noexcept;
    [[nodiscard]] Size num_cells_y() const noexcept;
    [[nodiscard]] Size num_cells() const noexcept;
    [[nodiscard]] Size num_faces_x() const noexcept;
    [[nodiscard]] Size num_faces_y() const noexcept;

    [[nodiscard]] T xmin() const noexcept;
    [[nodiscard]] T xmax() const noexcept;
    [[nodiscard]] T ymin() const noexcept;
    [[nodiscard]] T ymax() const noexcept;
    [[nodiscard]] T length_x() const noexcept;
    [[nodiscard]] T length_y() const noexcept;

    [[nodiscard]] Size linear_cell_index(Size i, Size j) const;

    [[nodiscard]] T first_face(Size i) const;
    [[nodiscard]] T second_face(Size j) const;
    [[nodiscard]] T first_center(Size i) const;
    [[nodiscard]] T second_center(Size j) const;
    [[nodiscard]] T first_cell_length(Size i) const;
    [[nodiscard]] T second_cell_length(Size j) const;

    [[nodiscard]] T x_face(Size i) const;
    [[nodiscard]] T y_face(Size j) const;
    [[nodiscard]] T x_center(Size i) const;
    [[nodiscard]] T y_center(Size j) const;
    [[nodiscard]] T x_cell_length(Size i) const;
    [[nodiscard]] T y_cell_length(Size j) const;

    [[nodiscard]] T cell_logical_area(Size i, Size j) const;
    [[nodiscard]] T cell_area(Size i, Size j) const;

    [[nodiscard]] T cell_measure(Size i, Size j) const;
    [[nodiscard]] T first_face_measure(Size i, Size j) const;
    [[nodiscard]] T second_face_measure(Size i, Size j) const;
    [[nodiscard]] std::span<const T> cell_measures() const noexcept;
    [[nodiscard]] std::span<const T> first_face_measures() const noexcept;
    [[nodiscard]] std::span<const T> second_face_measures() const noexcept;

    [[nodiscard]] T vertical_face_length(Size j) const;
    [[nodiscard]] T horizontal_face_length(Size i) const;

    [[nodiscard]] BasicPhysicalPoint2D<T> physical_vertex(Size i, Size j) const;
    [[nodiscard]] BasicPhysicalPoint2D<T> physical_face_point(Size i, Size j) const;
    [[nodiscard]] BasicPhysicalPoint2D<T> physical_cell_center(Size i, Size j) const;
    [[nodiscard]] BasicPhysicalPoint2D<T> physical_first_face_center(
        Size i,
        Size j
    ) const;
    [[nodiscard]] BasicPhysicalPoint2D<T> physical_second_face_center(
        Size i,
        Size j
    ) const;

private:
    BasicAxis1D<T> first_axis_;
    BasicAxis1D<T> second_axis_;
    std::string coordinate_system_name_;
    std::string first_coordinate_name_;
    std::string second_coordinate_name_;
    bool vtk_rectilinear_{};
    std::vector<BasicPhysicalPoint2D<T>> physical_face_points_;
    std::vector<BasicPhysicalPoint2D<T>> physical_cell_centers_;
    std::vector<BasicPhysicalPoint2D<T>> physical_first_face_centers_;
    std::vector<BasicPhysicalPoint2D<T>> physical_second_face_centers_;
    std::vector<T> cell_measures_;
    std::vector<T> first_face_measures_;
    std::vector<T> second_face_measures_;

    [[nodiscard]] static bool physical_point_is_finite(
        BasicPhysicalPoint2D<T> point
    ) noexcept;

    void validate_axis_patterns() const;

    template <class Mapping>
        requires CoordinateMapping2DFor<Mapping, T>
    void build_derived_geometry(const Mapping& mapping);

    template <class Mapping>
        requires CoordinateMapping2DFor<Mapping, T>
    void build_face_measures(const Mapping& mapping);

    void validate_x_cell_index(Size i) const;
    void validate_y_cell_index(Size j) const;
    void validate_x_face_index(Size i) const;
    void validate_y_face_index(Size j) const;
    void validate_cell_index(Size i, Size j) const;
};

using StructuredGrid2D = BasicStructuredGrid2D<double>;
using StructuredGrid2DFloat = BasicStructuredGrid2D<float>;
using StructuredGrid2DLongDouble = BasicStructuredGrid2D<long double>;

template <std::floating_point T>
std::ostream& operator<<(
    std::ostream& output,
    const BasicStructuredGrid2D<T>& grid
);

}  // namespace fvgrid

#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.tpp>
