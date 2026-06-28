// ============================================================================
// Arquivo: Coordinates1D.h
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Define coordenadas primárias tipadas para construção customizada de eixos 1D.
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

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <concepts>
#include <span>
#include <utility>
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/CoordinateTags1D.h>

namespace fvgrid {

template <std::floating_point T>
class BasicCoordinates1D final {
public:
    using value_type = T;

    [[nodiscard]] static BasicCoordinates1D faces(std::vector<T> values) {
        return BasicCoordinates1D{std::move(values), true};
    }

    [[nodiscard]] static BasicCoordinates1D centers(std::vector<T> values) {
        return BasicCoordinates1D{std::move(values), false};
    }

    template <PrimaryCoordinateTag1D Tag>
    [[nodiscard]] bool has_tag() const noexcept {
        if constexpr (std::same_as<Tag, FaceCoordinates1D>) {
            return is_face_coordinates_;
        } else {
            return !is_face_coordinates_;
        }
    }

    [[nodiscard]] std::span<const T> values() const noexcept {
        return values_;
    }

    [[nodiscard]] std::vector<T> release_values() && noexcept {
        return std::move(values_);
    }

private:
    BasicCoordinates1D(
        std::vector<T> values,
        bool is_face_coordinates
    ) noexcept
        : values_(std::move(values)),
          is_face_coordinates_(is_face_coordinates) {}

    std::vector<T> values_;
    bool is_face_coordinates_{};
};

using Coordinates1D = BasicCoordinates1D<double>;
using Coordinates1DFloat = BasicCoordinates1D<float>;
using Coordinates1DLongDouble = BasicCoordinates1D<long double>;

}  // namespace fvgrid
