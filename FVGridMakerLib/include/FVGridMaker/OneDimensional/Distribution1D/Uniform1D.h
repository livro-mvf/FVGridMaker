// ============================================================================
// Arquivo: Uniform1D.h
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Declara o gerador de eixos uniformes e funções auxiliares de conveniência.
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
#include <string_view>

#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>

namespace fvgrid {

template <std::floating_point T>
class BasicUniform1D final {
public:
    using value_type = T;

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "Uniform1D",
            "fvgrid.1d.distribution.Uniform1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] static BasicAxis1D<T> make(
        NVol nvol,
        BasicLength<T> length,
        BasicXInit<T> xinit
    );

private:
    static void validate_input(
        NVol nvol,
        BasicLength<T> length,
        BasicXInit<T> xinit
    );
};

using Uniform1D = BasicUniform1D<double>;
using Uniform1DFloat = BasicUniform1D<float>;
using Uniform1DLongDouble = BasicUniform1D<long double>;

template <std::floating_point T>
[[nodiscard]] BasicAxis1D<T> uniform_axis_1d(
    NVol nvol,
    BasicLength<T> length,
    BasicXInit<T> xinit
);

[[nodiscard]] Axis1D uniform_axis_1d(
    NVol nvol,
    Length length,
    XInit xinit
);

template <std::floating_point T>
[[nodiscard]] BasicAxis1D<T> uniform_axis_1d(
    Size nvol,
    T xinit,
    T xfinal
);

[[nodiscard]] Axis1D uniform_axis_1d(
    Size nvol,
    double xinit,
    double xfinal
);

}  // namespace fvgrid

#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.tpp>
