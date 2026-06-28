// ============================================================================
// Arquivo: CoordinateSystem2D.h
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Define pontos, células, conceitos e sistemas de coordenadas bidimensionais.
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
#include <numbers>
#include <string>
#include <string_view>
#include <utility>

#include <FVGridMaker/Core/Types.h>

namespace fvgrid {

template <std::floating_point T>
struct BasicPhysicalPoint2D final {
    using value_type = T;

    T x{};
    T y{};
    T z{};
};

using PhysicalPoint2D = BasicPhysicalPoint2D<double>;
using PhysicalPoint2DFloat = BasicPhysicalPoint2D<float>;
using PhysicalPoint2DLongDouble = BasicPhysicalPoint2D<long double>;

template <std::floating_point T>
struct BasicCoordinateCell2D final {
    using value_type = T;

    T first_min{};
    T first_max{};
    T second_min{};
    T second_max{};
};

using CoordinateCell2D = BasicCoordinateCell2D<double>;
using CoordinateCell2DFloat = BasicCoordinateCell2D<float>;
using CoordinateCell2DLongDouble = BasicCoordinateCell2D<long double>;

template <class Mapping, class T>
concept CoordinateMapping2DFor =
    std::floating_point<T> &&
    requires(
        const Mapping& mapping,
        T first,
        T second,
        BasicCoordinateCell2D<T> cell
    ) {
        { mapping.name() } -> std::convertible_to<std::string_view>;
        { mapping.first_coordinate_name() } -> std::convertible_to<std::string_view>;
        { mapping.second_coordinate_name() } -> std::convertible_to<std::string_view>;
        { mapping.vtk_rectilinear() } -> std::convertible_to<bool>;
        { mapping.map(first, second) } -> std::same_as<BasicPhysicalPoint2D<T>>;
        { mapping.cell_measure(cell) } -> std::convertible_to<T>;
    };

template <class Mapping>
concept CoordinateMapping2D = CoordinateMapping2DFor<Mapping, double>;

template <std::floating_point T>
struct BasicCartesianCoordinates2D final {
    using value_type = T;

    [[nodiscard]] constexpr std::string_view name() const noexcept {
        return "Cartesian";
    }

    [[nodiscard]] constexpr std::string_view first_coordinate_name()
        const noexcept {
        return "x";
    }

    [[nodiscard]] constexpr std::string_view second_coordinate_name()
        const noexcept {
        return "y";
    }

    [[nodiscard]] constexpr bool vtk_rectilinear() const noexcept {
        return true;
    }

    [[nodiscard]] constexpr BasicPhysicalPoint2D<T> map(
        T x,
        T y
    ) const noexcept {
        return {x, y, T{}};
    }

    [[nodiscard]] constexpr T cell_measure(
        BasicCoordinateCell2D<T> c
    ) const noexcept {
        return (c.first_max - c.first_min) * (c.second_max - c.second_min);
    }
};

template <std::floating_point T>
struct BasicPolarCoordinates2D final {
    using value_type = T;

    [[nodiscard]] constexpr std::string_view name() const noexcept {
        return "Polar";
    }

    [[nodiscard]] constexpr std::string_view first_coordinate_name()
        const noexcept {
        return "r";
    }

    [[nodiscard]] constexpr std::string_view second_coordinate_name()
        const noexcept {
        return "theta";
    }

    [[nodiscard]] constexpr bool vtk_rectilinear() const noexcept {
        return false;
    }

    [[nodiscard]] BasicPhysicalPoint2D<T> map(T r, T theta) const noexcept {
        return {r * std::cos(theta), r * std::sin(theta), T{}};
    }

    [[nodiscard]] constexpr T cell_measure(
        BasicCoordinateCell2D<T> c
    ) const noexcept {
        return T{0.5} *
            (c.first_max * c.first_max - c.first_min * c.first_min) *
            (c.second_max - c.second_min);
    }
};

template <std::floating_point T>
struct BasicAxisymmetricCoordinates2D final {
    using value_type = T;

    [[nodiscard]] constexpr std::string_view name() const noexcept {
        return "Axisymmetric";
    }

    [[nodiscard]] constexpr std::string_view first_coordinate_name()
        const noexcept {
        return "r";
    }

    [[nodiscard]] constexpr std::string_view second_coordinate_name()
        const noexcept {
        return "z";
    }

    [[nodiscard]] constexpr bool vtk_rectilinear() const noexcept {
        return true;
    }

    [[nodiscard]] constexpr BasicPhysicalPoint2D<T> map(
        T r,
        T z
    ) const noexcept {
        return {r, z, T{}};
    }

    [[nodiscard]] constexpr T cell_measure(
        BasicCoordinateCell2D<T> c
    ) const noexcept {
        return std::numbers::pi_v<T> *
            (c.first_max * c.first_max - c.first_min * c.first_min) *
            (c.second_max - c.second_min);
    }
};

using CartesianCoordinates2D = BasicCartesianCoordinates2D<double>;
using CartesianCoordinates2DFloat = BasicCartesianCoordinates2D<float>;
using CartesianCoordinates2DLongDouble = BasicCartesianCoordinates2D<long double>;

using PolarCoordinates2D = BasicPolarCoordinates2D<double>;
using PolarCoordinates2DFloat = BasicPolarCoordinates2D<float>;
using PolarCoordinates2DLongDouble = BasicPolarCoordinates2D<long double>;

using AxisymmetricCoordinates2D = BasicAxisymmetricCoordinates2D<double>;
using AxisymmetricCoordinates2DFloat = BasicAxisymmetricCoordinates2D<float>;
using AxisymmetricCoordinates2DLongDouble = BasicAxisymmetricCoordinates2D<long double>;

template <std::floating_point T, class Map, class Measure>
class BasicFunctionalCoordinateMapping2D final {
public:
    using value_type = T;

    BasicFunctionalCoordinateMapping2D(
        std::string name,
        std::string first_name,
        std::string second_name,
        Map map,
        Measure measure,
        bool vtk_rectilinear = false
    )
        : name_(std::move(name)),
          first_name_(std::move(first_name)),
          second_name_(std::move(second_name)),
          map_(std::move(map)),
          measure_(std::move(measure)),
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
        return static_cast<T>(measure_(cell));
    }

private:
    std::string name_;
    std::string first_name_;
    std::string second_name_;
    Map map_;
    Measure measure_;
    bool vtk_rectilinear_{};
};

template <class Map, class Measure>
using FunctionalCoordinateMapping2D =
    BasicFunctionalCoordinateMapping2D<double, Map, Measure>;

template <class Map, class Measure>
[[nodiscard]] auto make_coordinate_mapping_2d(
    std::string name,
    std::string first_name,
    std::string second_name,
    Map map,
    Measure measure,
    bool vtk_rectilinear = false
) {
    return BasicFunctionalCoordinateMapping2D<double, Map, Measure>{
        std::move(name),
        std::move(first_name),
        std::move(second_name),
        std::move(map),
        std::move(measure),
        vtk_rectilinear
    };
}

template <std::floating_point T, class Map, class Measure>
[[nodiscard]] auto make_basic_coordinate_mapping_2d(
    std::string name,
    std::string first_name,
    std::string second_name,
    Map map,
    Measure measure,
    bool vtk_rectilinear = false
) {
    return BasicFunctionalCoordinateMapping2D<T, Map, Measure>{
        std::move(name),
        std::move(first_name),
        std::move(second_name),
        std::move(map),
        std::move(measure),
        vtk_rectilinear
    };
}

}  // namespace fvgrid
