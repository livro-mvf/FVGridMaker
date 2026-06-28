// ============================================================================
// Arquivo: StrongTypes.h
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Define wrappers fortemente tipados para parâmetros da API de malhas.
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
#include <compare>
#include <concepts>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/Types.h>

namespace fvgrid {

template <std::floating_point T, class Tag>
class StrongReal final {
public:
    using value_type = T;
    using tag_type = Tag;

    constexpr explicit StrongReal(T value) noexcept : value_(value) {}

    [[nodiscard]] constexpr T value() const noexcept {
        return value_;
    }

    friend constexpr bool operator==(
        const StrongReal&,
        const StrongReal&
    ) noexcept = default;

    friend constexpr auto operator<=> (
        const StrongReal&,
        const StrongReal&
    ) noexcept = default;

private:
    T value_;
};

template <std::integral T, class Tag>
class StrongInteger final {
public:
    using value_type = T;
    using tag_type = Tag;

    constexpr explicit StrongInteger(T value) noexcept : value_(value) {}

    [[nodiscard]] constexpr T value() const noexcept {
        return value_;
    }

    friend constexpr bool operator==(
        const StrongInteger&,
        const StrongInteger&
    ) noexcept = default;

    friend constexpr auto operator<=> (
        const StrongInteger&,
        const StrongInteger&
    ) noexcept = default;

private:
    T value_;
};

struct NVolTag final {};
struct LengthTag final {};
struct XInitTag final {};
struct XFinalTag final {};
struct MinSpacingTag final {};
struct BetaTag final {};
struct SeedTag final {};

using NVol = StrongInteger<Size, NVolTag>;
using Seed = StrongInteger<UInt64, SeedTag>;

template <std::floating_point T>
using BasicLength = StrongReal<T, LengthTag>;

template <std::floating_point T>
using BasicXInit = StrongReal<T, XInitTag>;

template <std::floating_point T>
using BasicXFinal = StrongReal<T, XFinalTag>;

template <std::floating_point T>
using BasicMinSpacing = StrongReal<T, MinSpacingTag>;

template <std::floating_point T>
using BasicBeta = StrongReal<T, BetaTag>;

using Length = BasicLength<double>;
using XInit = BasicXInit<double>;
using XFinal = BasicXFinal<double>;
using MinSpacing = BasicMinSpacing<double>;
using Beta = BasicBeta<double>;

using LengthFloat = BasicLength<float>;
using XInitFloat = BasicXInit<float>;
using XFinalFloat = BasicXFinal<float>;
using MinSpacingFloat = BasicMinSpacing<float>;
using BetaFloat = BasicBeta<float>;

using LengthLongDouble = BasicLength<long double>;
using XInitLongDouble = BasicXInit<long double>;
using XFinalLongDouble = BasicXFinal<long double>;
using MinSpacingLongDouble = BasicMinSpacing<long double>;
using BetaLongDouble = BasicBeta<long double>;

}  // namespace fvgrid
