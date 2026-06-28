// ============================================================================
// Arquivo: VolumeCentered1D.h
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Define o padrão volume centrado que reconstrói centros a partir de faces.
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
#include <FVGridMaker/OneDimensional/GridPattern1D/CentersFromFaces1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/ConstantWeight1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/CoordinateTags1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Domain1D.h>

namespace fvgrid {

struct VolumeCentered1D final {
    using primary_coordinates = FaceCoordinates1D;

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "VolumeCentered1D",
            "fvgrid.1d.grid_pattern.VolumeCentered1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view name() noexcept {
        return "VolumeCentered1D";
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] static constexpr std::string_view primary_coordinates_name()
        noexcept {
        return "faces";
    }

    [[nodiscard]] static constexpr std::string_view secondary_coordinates()
        noexcept {
        return "centers";
    }

    template <std::floating_point T>
    [[nodiscard]] static std::vector<T> centers_from_faces(
        std::span<const T> faces
    ) {
        validate_face_count_for_volume_centered(faces.size());

        std::vector<T> face_values(faces.begin(), faces.end());

        BasicAxisGeometry1D<T> geometry = make_midpoint_reconstructor<T>()
            .complete_geometry(std::move(face_values), BasicDomain1D<T>::none());

        return std::move(geometry.centers);
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
        std::vector<T> faces(cell_count + static_cast<Size>(1));
        const T deta = T{1} / static_cast<T>(cell_count);

        for (Size i = 0; i <= cell_count; ++i) {
            faces[i] = static_cast<T>(map(static_cast<T>(i) * deta));
        }

        return faces;
    }

    template <std::floating_point T>
    [[nodiscard]] static BasicAxisGeometry1D<T> complete_geometry(
        std::vector<T> faces,
        BasicDomain1D<T> domain
    ) {
        validate_face_count_for_volume_centered(faces.size());

        BasicAxisGeometry1D<T> geometry = make_midpoint_reconstructor<T>()
            .complete_geometry(std::move(faces), domain);

        geometry.pattern_name = std::string{name()};

        return geometry;
    }

private:
    template <std::floating_point T>
    [[nodiscard]] static CentersFromFaces1D<BasicConstantWeight1D<T>>
    make_midpoint_reconstructor() {
        return CentersFromFaces1D{BasicConstantWeight1D<T>{T{0.5}}};
    }

    static void validate_face_count_for_volume_centered(Size count) {
        require<errors::grid::InvalidFaceCount>(
            count >= static_cast<Size>(2),
            VolumeCentered1D::id()
        );
    }
};

}  // namespace fvgrid
