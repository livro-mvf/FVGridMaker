// ============================================================================
// Arquivo: Random1D.h
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Declara o gerador pseudoaleatório reprodutível de eixos unidimensionais.
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
#include <random>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include <FVGridMaker/Core/ID.h>
#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/CoordinateTags1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Domain1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/GridPatternConcept1D.h>

namespace fvgrid {

template <std::floating_point T>
class BasicRandom1D final {
public:
    using value_type = T;

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "Random1D",
            "fvgrid.1d.distribution.Random1D"
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
        BasicXInit<T> xinit,
        Seed seed
    );

    [[nodiscard]] static BasicAxis1D<T> make(
        NVol nvol,
        BasicLength<T> length,
        BasicXInit<T> xinit,
        Seed seed,
        BasicMinSpacing<T> min_spacing
    );

    template <class Pattern>
        requires GridPattern1DFor<std::remove_cvref_t<Pattern>, T>
    [[nodiscard]] static BasicAxis1D<T> make(
        NVol nvol,
        BasicLength<T> length,
        BasicXInit<T> xinit,
        Seed seed,
        Pattern&& pattern
    ) {
        return make(
            nvol,
            length,
            xinit,
            seed,
            BasicMinSpacing<T>{T{}},
            std::forward<Pattern>(pattern)
        );
    }

    template <class Pattern>
        requires GridPattern1DFor<std::remove_cvref_t<Pattern>, T>
    [[nodiscard]] static BasicAxis1D<T> make(
        NVol nvol,
        BasicLength<T> length,
        BasicXInit<T> xinit,
        Seed seed,
        BasicMinSpacing<T> min_spacing,
        Pattern&& pattern
    ) {
        using PatternType = std::remove_cvref_t<Pattern>;

        validate_input(
            nvol,
            length,
            xinit,
            min_spacing,
            interval_count_for_pattern<PatternType>(nvol)
        );

        const BasicDomain1D<T> domain = BasicDomain1D<T>::from_length(
            xinit,
            length
        );

        std::mt19937_64 random_engine{seed.value()};

        std::vector<T> primary_coordinates =
            build_primary_coordinates<PatternType>(
                nvol,
                length,
                xinit,
                min_spacing,
                random_engine
            );

        const auto& pattern_ref = pattern;

        BasicAxisGeometry1D<T> geometry = pattern_ref.complete_geometry(
            std::move(primary_coordinates),
            domain
        );

        return BasicAxis1D<T>::from_geometry(std::move(geometry));
    }

private:
    static void validate_input(
        NVol nvol,
        BasicLength<T> length,
        BasicXInit<T> xinit,
        BasicMinSpacing<T> min_spacing,
        Size interval_count
    );

    [[nodiscard]] static std::vector<T> build_random_partition(
        Size interval_count,
        T total_length,
        T min_spacing,
        std::mt19937_64& random_engine
    );

    template <class PatternType>
    [[nodiscard]] static Size interval_count_for_pattern(NVol nvol) {
        using CoordinateTag = typename PatternType::primary_coordinates;

        if constexpr (std::same_as<CoordinateTag, FaceCoordinates1D>) {
            return nvol.value();
        } else {
            return nvol.value() + static_cast<Size>(1);
        }
    }

    template <class PatternType>
    [[nodiscard]] static std::vector<T> build_primary_coordinates(
        NVol nvol,
        BasicLength<T> length,
        BasicXInit<T> xinit,
        BasicMinSpacing<T> min_spacing,
        std::mt19937_64& random_engine
    ) {
        using CoordinateTag = typename PatternType::primary_coordinates;

        const Size cell_count = nvol.value();
        const T x0 = xinit.value();

        if constexpr (std::same_as<CoordinateTag, FaceCoordinates1D>) {
            const std::vector<T> widths = build_random_partition(
                cell_count,
                length.value(),
                min_spacing.value(),
                random_engine
            );

            std::vector<T> faces(cell_count + static_cast<Size>(1));
            faces[0] = x0;

            for (Size i = 0; i < cell_count; ++i) {
                faces[i + static_cast<Size>(1)] = faces[i] + widths[i];
            }

            faces[cell_count] = x0 + length.value();

            return faces;
        } else {
            const Size gap_count = cell_count + static_cast<Size>(1);

            const std::vector<T> gaps = build_random_partition(
                gap_count,
                length.value(),
                min_spacing.value(),
                random_engine
            );

            std::vector<T> centers(cell_count);
            T coordinate = x0 + gaps[0];
            centers[0] = coordinate;

            for (Size i = 1; i < cell_count; ++i) {
                coordinate += gaps[i];
                centers[i] = coordinate;
            }

            return centers;
        }
    }
};

using Random1D = BasicRandom1D<double>;
using Random1DFloat = BasicRandom1D<float>;
using Random1DLongDouble = BasicRandom1D<long double>;

template <std::floating_point T>
[[nodiscard]] BasicAxis1D<T> random_axis_1d(
    NVol nvol,
    BasicLength<T> length,
    BasicXInit<T> xinit,
    Seed seed
);

[[nodiscard]] Axis1D random_axis_1d(
    NVol nvol,
    Length length,
    XInit xinit,
    Seed seed
);

template <std::floating_point T>
[[nodiscard]] BasicAxis1D<T> random_axis_1d(
    NVol nvol,
    BasicLength<T> length,
    BasicXInit<T> xinit,
    Seed seed,
    BasicMinSpacing<T> min_spacing
);

[[nodiscard]] Axis1D random_axis_1d(
    NVol nvol,
    Length length,
    XInit xinit,
    Seed seed,
    MinSpacing min_spacing
);

template <class Pattern>
    requires GridPattern1DFor<std::remove_cvref_t<Pattern>, double>
[[nodiscard]] Axis1D random_axis_1d(
    NVol nvol,
    Length length,
    XInit xinit,
    Seed seed,
    Pattern&& pattern
) {
    return Random1D::make(nvol, length, xinit, seed, std::forward<Pattern>(pattern));
}

template <class Pattern>
    requires GridPattern1DFor<std::remove_cvref_t<Pattern>, double>
[[nodiscard]] Axis1D random_axis_1d(
    NVol nvol,
    Length length,
    XInit xinit,
    Seed seed,
    MinSpacing min_spacing,
    Pattern&& pattern
) {
    return Random1D::make(
        nvol,
        length,
        xinit,
        seed,
        min_spacing,
        std::forward<Pattern>(pattern)
    );
}

template <std::floating_point T>
[[nodiscard]] BasicAxis1D<T> random_axis_1d(
    Size nvol,
    T xinit,
    T xfinal,
    UInt64 seed
);

[[nodiscard]] Axis1D random_axis_1d(
    Size nvol,
    double xinit,
    double xfinal,
    UInt64 seed
);

}  // namespace fvgrid

#include <FVGridMaker/OneDimensional/Distribution1D/Random1D.tpp>
