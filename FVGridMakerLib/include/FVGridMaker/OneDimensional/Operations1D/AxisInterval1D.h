// ============================================================================
// Arquivo: AxisInterval1D.h
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Define intervalos unidimensionais usados em interseções e recortes.
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

namespace fvgrid {

template <std::floating_point T>
class BasicAxisInterval1D final {
public:
    using value_type = T;

    [[nodiscard]] static constexpr BasicAxisInterval1D empty() noexcept {
        return BasicAxisInterval1D{T{}, T{}, true};
    }

    [[nodiscard]] static constexpr BasicAxisInterval1D point(
        T coordinate
    ) noexcept {
        return BasicAxisInterval1D{coordinate, coordinate, false};
    }

    [[nodiscard]] static constexpr BasicAxisInterval1D from_bounds(
        T lower,
        T upper
    ) noexcept {
        if (upper < lower) {
            return BasicAxisInterval1D::empty();
        }

        return BasicAxisInterval1D{lower, upper, false};
    }

    [[nodiscard]] constexpr bool is_empty() const noexcept {
        return is_empty_;
    }

    [[nodiscard]] constexpr bool is_point() const noexcept {
        return !is_empty_ && lower_ == upper_;
    }

    [[nodiscard]] constexpr bool is_interval() const noexcept {
        return !is_empty_ && upper_ > lower_;
    }

    [[nodiscard]] constexpr T lower() const noexcept {
        return lower_;
    }

    [[nodiscard]] constexpr T upper() const noexcept {
        return upper_;
    }

    [[nodiscard]] constexpr T length() const noexcept {
        if (is_empty_) {
            return T{};
        }

        return upper_ - lower_;
    }

private:
    constexpr BasicAxisInterval1D(
        T lower,
        T upper,
        bool is_empty
    ) noexcept
        : lower_(lower),
          upper_(upper),
          is_empty_(is_empty) {}

    T lower_{};
    T upper_{};
    bool is_empty_{true};
};

using AxisInterval1D = BasicAxisInterval1D<double>;
using AxisInterval1DFloat = BasicAxisInterval1D<float>;
using AxisInterval1DLongDouble = BasicAxisInterval1D<long double>;

}  // namespace fvgrid
