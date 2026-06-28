// ============================================================================
// Arquivo: CoordinateMappingFactory2D.h
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Define fábricas de mapeamentos funcionais para sistemas de coordenadas 2D.
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

#include <concepts>
#include <string>
#include <string_view>
#include <utility>

#include <FVGridMaker/TwoDimensional/CoordinateSystem2D/CoordinateMetrics2D.h>

namespace fvgrid {

template <std::floating_point T, class Map, class CellMeasure,
          class FirstFaceMeasure, class SecondFaceMeasure>
class BasicFunctionalCoordinateMetrics2D final {
public:
    using value_type = T;

    BasicFunctionalCoordinateMetrics2D(
        std::string name,
        std::string first_name,
        std::string second_name,
        Map map,
        CellMeasure cell_measure,
        FirstFaceMeasure first_face_measure,
        SecondFaceMeasure second_face_measure,
        bool vtk_rectilinear = false
    )
        : name_(std::move(name)),
          first_name_(std::move(first_name)),
          second_name_(std::move(second_name)),
          map_(std::move(map)),
          cell_measure_(std::move(cell_measure)),
          first_face_measure_(std::move(first_face_measure)),
          second_face_measure_(std::move(second_face_measure)),
          vtk_rectilinear_(vtk_rectilinear) {}

    [[nodiscard]] std::string_view name() const noexcept {
        return name_;
    }

    [[nodiscard]] std::string_view first_coordinate_name() const noexcept {
        return first_name_;
    }

    [[nodiscard]] std::string_view second_coordinate_name() const noexcept {
        return second_name_;
    }

    [[nodiscard]] bool vtk_rectilinear() const noexcept {
        return vtk_rectilinear_;
    }

    [[nodiscard]] BasicPhysicalPoint2D<T> map(T first, T second) const {
        return map_(first, second);
    }

    [[nodiscard]] T cell_measure(BasicCoordinateCell2D<T> cell) const {
        return static_cast<T>(cell_measure_(cell));
    }

    [[nodiscard]] T first_face_measure(
        T first,
        T second_min,
        T second_max
    ) const {
        return static_cast<T>(
            first_face_measure_(first, second_min, second_max)
        );
    }

    [[nodiscard]] T second_face_measure(
        T second,
        T first_min,
        T first_max
    ) const {
        return static_cast<T>(
            second_face_measure_(second, first_min, first_max)
        );
    }

private:
    std::string name_;
    std::string first_name_;
    std::string second_name_;
    Map map_;
    CellMeasure cell_measure_;
    FirstFaceMeasure first_face_measure_;
    SecondFaceMeasure second_face_measure_;
    bool vtk_rectilinear_{};
};

template <class Map, class CellMeasure, class FirstFaceMeasure,
          class SecondFaceMeasure>
using FunctionalCoordinateMetrics2D = BasicFunctionalCoordinateMetrics2D<
    double,
    Map,
    CellMeasure,
    FirstFaceMeasure,
    SecondFaceMeasure>;

template <class Map, class CellMeasure, class FirstFaceMeasure,
          class SecondFaceMeasure>
[[nodiscard]] auto make_coordinate_mapping_2d(
    std::string name,
    std::string first_name,
    std::string second_name,
    Map map,
    CellMeasure cell_measure,
    FirstFaceMeasure first_face_measure,
    SecondFaceMeasure second_face_measure,
    bool vtk_rectilinear = false
) {
    return BasicFunctionalCoordinateMetrics2D<
        double,
        Map,
        CellMeasure,
        FirstFaceMeasure,
        SecondFaceMeasure>{
        std::move(name),
        std::move(first_name),
        std::move(second_name),
        std::move(map),
        std::move(cell_measure),
        std::move(first_face_measure),
        std::move(second_face_measure),
        vtk_rectilinear
    };
}

template <std::floating_point T, class Map, class CellMeasure,
          class FirstFaceMeasure, class SecondFaceMeasure>
[[nodiscard]] auto make_basic_coordinate_mapping_2d(
    std::string name,
    std::string first_name,
    std::string second_name,
    Map map,
    CellMeasure cell_measure,
    FirstFaceMeasure first_face_measure,
    SecondFaceMeasure second_face_measure,
    bool vtk_rectilinear = false
) {
    return BasicFunctionalCoordinateMetrics2D<
        T,
        Map,
        CellMeasure,
        FirstFaceMeasure,
        SecondFaceMeasure>{
        std::move(name),
        std::move(first_name),
        std::move(second_name),
        std::move(map),
        std::move(cell_measure),
        std::move(first_face_measure),
        std::move(second_face_measure),
        vtk_rectilinear
    };
}

}  // namespace fvgrid
