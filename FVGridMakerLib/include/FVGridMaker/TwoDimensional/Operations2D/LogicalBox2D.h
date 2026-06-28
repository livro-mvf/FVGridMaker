// ============================================================================
// Arquivo: LogicalBox2D.h
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Define caixas lógicas bidimensionais usadas em interseções e recortes.
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

#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/OneDimensional/Operations1D/AxisInterval1D.h>

namespace fvgrid {

template <std::floating_point T>
class BasicLogicalBox2D final {
public:
    using value_type = T;

    [[nodiscard]] static constexpr BasicLogicalBox2D empty() noexcept {
        return BasicLogicalBox2D{
            BasicAxisInterval1D<T>::empty(),
            BasicAxisInterval1D<T>::empty()
        };
    }

    [[nodiscard]] static constexpr BasicLogicalBox2D from_intervals(
        BasicAxisInterval1D<T> first,
        BasicAxisInterval1D<T> second
    ) noexcept {
        return BasicLogicalBox2D{first, second};
    }

    [[nodiscard]] constexpr BasicAxisInterval1D<T> first_interval()
        const noexcept {
        return first_;
    }

    [[nodiscard]] constexpr BasicAxisInterval1D<T> second_interval()
        const noexcept {
        return second_;
    }

    [[nodiscard]] constexpr bool is_empty() const noexcept {
        return first_.is_empty() || second_.is_empty();
    }

    [[nodiscard]] constexpr bool is_area() const noexcept {
        return first_.is_interval() && second_.is_interval();
    }

    [[nodiscard]] constexpr T computational_area() const noexcept {
        if (!is_area()) {
            return T{};
        }

        return first_.length() * second_.length();
    }

private:
    constexpr BasicLogicalBox2D(
        BasicAxisInterval1D<T> first,
        BasicAxisInterval1D<T> second
    ) noexcept
        : first_(first),
          second_(second) {}

    BasicAxisInterval1D<T> first_ = BasicAxisInterval1D<T>::empty();
    BasicAxisInterval1D<T> second_ = BasicAxisInterval1D<T>::empty();
};

using LogicalBox2D = BasicLogicalBox2D<double>;
using LogicalBox2DFloat = BasicLogicalBox2D<float>;
using LogicalBox2DLongDouble = BasicLogicalBox2D<long double>;

}  // namespace fvgrid
