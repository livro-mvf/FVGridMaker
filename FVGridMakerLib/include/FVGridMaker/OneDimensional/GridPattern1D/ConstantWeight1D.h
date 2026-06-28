// ============================================================================
// Arquivo: ConstantWeight1D.h
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Define regra de peso constante para padrões customizados de reconstrução.
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
#include <string_view>

#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/ErrorHandling/BuiltInErrors.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>

namespace fvgrid {

template <std::floating_point T>
class BasicConstantWeight1D final {
public:
    using value_type = T;

    explicit BasicConstantWeight1D(T value)
        : value_{value} {
        validate(value_);
    }

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "ConstantWeight1D",
            "fvgrid.1d.grid_pattern.ConstantWeight1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] T value() const noexcept {
        return value_;
    }

    [[nodiscard]] T operator()(Size) const noexcept {
        return value_;
    }

private:
    static void validate(T value) {
        require<errors::core::InvalidArgument>(
            std::isfinite(value) && value > T{0} && value < T{1},
            BasicConstantWeight1D::id()
        );
    }

    T value_;
};

template <std::floating_point T>
BasicConstantWeight1D(T) -> BasicConstantWeight1D<T>;

using ConstantWeight1D = BasicConstantWeight1D<double>;
using ConstantWeight1DFloat = BasicConstantWeight1D<float>;
using ConstantWeight1DLongDouble = BasicConstantWeight1D<long double>;

}  // namespace fvgrid
