// ============================================================================
// Arquivo: Domain1D.h
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Define o domínio físico unidimensional usado em reconstruções por centros.
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
#include <string_view>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/ErrorHandling/BuiltInErrors.h>
#include <FVGridMaker/ErrorHandling/ThrowError.h>

namespace fvgrid {

template <std::floating_point T>
class BasicDomain1D final {
public:
    using value_type = T;

    [[nodiscard]] static constexpr BasicDomain1D none() noexcept {
        return BasicDomain1D{};
    }

    [[nodiscard]] static constexpr BasicDomain1D from_length(
        BasicXInit<T> xinit,
        BasicLength<T> length
    ) noexcept {
        return BasicDomain1D{
            xinit.value(),
            xinit.value() + length.value(),
            true
        };
    }

    [[nodiscard]] static constexpr BasicDomain1D from_bounds(
        BasicXInit<T> xinit,
        BasicXFinal<T> xfinal
    ) noexcept {
        return BasicDomain1D{xinit.value(), xfinal.value(), true};
    }

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "Domain1D",
            "fvgrid.1d.pattern.Domain1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] constexpr bool has_bounds() const noexcept {
        return has_bounds_;
    }

    [[nodiscard]] T xmin() const {
        require<errors::core::InvalidArgument>(
            has_bounds_,
            BasicDomain1D::id()
        );

        return xmin_;
    }

    [[nodiscard]] T xmax() const {
        require<errors::core::InvalidArgument>(
            has_bounds_,
            BasicDomain1D::id()
        );

        return xmax_;
    }

    [[nodiscard]] T length() const {
        require<errors::core::InvalidArgument>(
            has_bounds_,
            BasicDomain1D::id()
        );

        return xmax_ - xmin_;
    }

private:
    constexpr BasicDomain1D() noexcept = default;

    constexpr BasicDomain1D(
        T xmin,
        T xmax,
        bool has_bounds
    ) noexcept
        : xmin_(xmin),
          xmax_(xmax),
          has_bounds_(has_bounds) {}

    T xmin_{};
    T xmax_{};
    bool has_bounds_{false};
};

using Domain1D = BasicDomain1D<double>;
using Domain1DFloat = BasicDomain1D<float>;
using Domain1DLongDouble = BasicDomain1D<long double>;

}  // namespace fvgrid
