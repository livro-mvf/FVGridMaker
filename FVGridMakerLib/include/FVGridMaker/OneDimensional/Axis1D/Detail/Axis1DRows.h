// ============================================================================
// Arquivo: Axis1DRows.h
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Define a travessia tabular compartilhada por impressão e saída CSV de eixos.
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

#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>

namespace fvgrid::detail {

template <std::floating_point T, class Callback>
void for_each_axis1d_row(
    const BasicAxis1D<T>& axis,
    Callback&& callback
) {
    const auto faces = axis.faces();
    const auto centers = axis.centers();
    const auto dx_faces = axis.dx_faces();
    const auto dx_centers = axis.dx_centers();

    const Size cell_count = axis.num_cells();

    for (Size i = 0; i < cell_count; ++i) {
        callback(
            i,
            faces[i],
            centers[i],
            dx_faces[i],
            dx_centers[i],
            true
        );
    }

    callback(
        cell_count,
        faces[cell_count],
        T{},
        T{},
        dx_centers[cell_count],
        false
    );
}

}  // namespace fvgrid::detail
