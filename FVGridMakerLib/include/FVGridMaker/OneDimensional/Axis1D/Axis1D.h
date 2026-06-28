// ============================================================================
// Arquivo: Axis1D.h
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Declara o eixo unidimensional, suas métricas e aliases escalares públicos.
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
#include <iosfwd>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/AxisGeometry1D.h>

namespace fvgrid {

template <std::floating_point T>
class BasicAxis1D final {
public:
    using value_type = T;

    BasicAxis1D() = delete;

    explicit BasicAxis1D(std::vector<T> faces);

    BasicAxis1D(
        std::vector<T> faces,
        std::vector<T> centers,
        std::string pattern_name
    );

    [[nodiscard]] static BasicAxis1D from_geometry(
        BasicAxisGeometry1D<T> geometry
    );

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "Axis1D",
            "fvgrid.1d.axis.Axis1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] Size num_cells() const noexcept;
    [[nodiscard]] Size num_faces() const noexcept;

    [[nodiscard]] std::span<const T> faces() const noexcept;
    [[nodiscard]] std::span<const T> centers() const noexcept;

    [[nodiscard]] std::span<const T> dx_faces() const noexcept;
    [[nodiscard]] std::span<const T> dx_centers() const noexcept;

    [[nodiscard]] std::span<const T> cell_lengths() const noexcept;

    [[nodiscard]] T face(Size i) const noexcept;
    [[nodiscard]] T center(Size p) const noexcept;

    [[nodiscard]] T west_face(Size p) const noexcept;
    [[nodiscard]] T east_face(Size p) const noexcept;

    [[nodiscard]] T cell_length(Size p) const noexcept;
    [[nodiscard]] T center_distance(Size i) const noexcept;

    [[nodiscard]] T DxP(Size p) const noexcept;
    [[nodiscard]] T DxW(Size p) const noexcept;
    [[nodiscard]] T DxE(Size p) const noexcept;

    [[nodiscard]] T deltaxw(Size p) const noexcept;
    [[nodiscard]] T deltaxe(Size p) const noexcept;

    [[nodiscard]] T xmin() const noexcept;
    [[nodiscard]] T xmax() const noexcept;
    [[nodiscard]] T length() const noexcept;

    [[nodiscard]] std::string_view pattern_name() const noexcept;

    template <class Pattern>
        requires requires {
            { Pattern::name() } -> std::convertible_to<std::string_view>;
        }
    [[nodiscard]] bool has_pattern() const noexcept {
        return pattern_name() == Pattern::name();
    }

private:
    std::vector<T> faces_;
    std::vector<T> centers_;
    std::vector<T> dx_faces_;
    std::vector<T> dx_centers_;

    std::string pattern_name_{"VolumeCentered1D"};

    void validate_geometry() const;
    void rebuild_metrics();
};

using Axis1D = BasicAxis1D<double>;
using Axis1DFloat = BasicAxis1D<float>;
using Axis1DLongDouble = BasicAxis1D<long double>;

template <std::floating_point T>
std::ostream& operator<<(std::ostream& os, const BasicAxis1D<T>& axis);

}  // namespace fvgrid

#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.tpp>
