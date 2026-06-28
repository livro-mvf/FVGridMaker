// ============================================================================
// Arquivo: FaceCentered1D.h
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Define o padrão face centrada que reconstrói faces a partir de centros.
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

#include <algorithm>
#include <concepts>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/ErrorHandling/BuiltInErrors.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/AxisGeometry1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/ConstantWeight1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/CoordinateTags1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Domain1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/FacesFromCenters1D.h>

namespace fvgrid {

struct FaceCentered1D final {
    using primary_coordinates = CenterCoordinates1D;

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "FaceCentered1D",
            "fvgrid.1d.grid_pattern.FaceCentered1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view name() noexcept {
        return "FaceCentered1D";
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] static constexpr std::string_view primary_coordinates_name()
        noexcept {
        return "centers";
    }

    [[nodiscard]] static constexpr std::string_view secondary_coordinates()
        noexcept {
        return "faces";
    }

    template <std::floating_point T>
    [[nodiscard]] static std::vector<T> faces_from_centers(
        std::span<const T> centers,
        T x_min,
        T x_max
    ) {
        const BasicDomain1D<T> domain = BasicDomain1D<T>::from_bounds(
            BasicXInit<T>{x_min},
            BasicXFinal<T>{x_max}
        );

        validate_domain_for_face_centered(domain);
        validate_centers_for_face_centered(centers, x_min, x_max);

        std::vector<T> center_values(centers.begin(), centers.end());

        BasicAxisGeometry1D<T> geometry = make_midpoint_reconstructor<T>()
            .complete_geometry(std::move(center_values), domain);

        return std::move(geometry.faces);
    }

    template <class CoordinateMap>
    [[nodiscard]] static std::vector<Real> primary_coordinates_from_map(
        Size cell_count,
        CoordinateMap&& map
    ) {
        return primary_coordinates_from_map<Real>(
            cell_count,
            std::forward<CoordinateMap>(map)
        );
    }

    template <std::floating_point T, class CoordinateMap>
    [[nodiscard]] static std::vector<T> primary_coordinates_from_map(
        Size cell_count,
        CoordinateMap&& map
    ) {
        std::vector<T> centers(cell_count);
        const T deta = T{1} / static_cast<T>(cell_count);

        for (Size i = 0; i < cell_count; ++i) {
            centers[i] = static_cast<T>(
                map((static_cast<T>(i) + T{0.5}) * deta)
            );
        }

        return centers;
    }

    template <std::floating_point T>
    [[nodiscard]] static BasicAxisGeometry1D<T> complete_geometry(
        std::vector<T> centers,
        BasicDomain1D<T> domain
    ) {
        validate_domain_for_face_centered(domain);

        validate_centers_for_face_centered(
            std::span<const T>{centers.data(), centers.size()},
            domain.xmin(),
            domain.xmax()
        );

        BasicAxisGeometry1D<T> geometry = make_midpoint_reconstructor<T>()
            .complete_geometry(std::move(centers), domain);

        geometry.pattern_name = std::string{name()};

        return geometry;
    }

private:
    template <std::floating_point T>
    [[nodiscard]] static FacesFromCenters1D<BasicConstantWeight1D<T>>
    make_midpoint_reconstructor() {
        return FacesFromCenters1D{BasicConstantWeight1D<T>{T{0.5}}};
    }

    template <std::floating_point T>
    static void validate_domain_for_face_centered(BasicDomain1D<T> domain) {
        require<errors::core::InvalidArgument>(
            domain.has_bounds(),
            FaceCentered1D::id()
        );

        require<errors::grid::InvalidLength>(
            domain.xmax() > domain.xmin(),
            FaceCentered1D::id()
        );
    }

    template <std::floating_point T>
    static void validate_centers_for_face_centered(
        std::span<const T> centers,
        T x_min,
        T x_max
    ) {
        require<errors::grid::InvalidCenterCount>(
            !centers.empty(),
            FaceCentered1D::id()
        );

        const bool centers_strictly_increasing =
            std::ranges::adjacent_find(
                centers,
                [](T left, T right) {
                    return !(right > left);
                }
            ) == centers.end();

        require<errors::grid::NonIncreasingCenters>(
            centers_strictly_increasing,
            FaceCentered1D::id()
        );

        require<errors::core::OutOfRange>(
            centers.front() > x_min,
            FaceCentered1D::id()
        );

        require<errors::core::OutOfRange>(
            centers.back() < x_max,
            FaceCentered1D::id()
        );
    }
};

}  // namespace fvgrid
