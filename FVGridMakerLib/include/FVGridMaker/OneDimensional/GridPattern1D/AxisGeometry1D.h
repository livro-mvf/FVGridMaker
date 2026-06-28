// ============================================================================
// Arquivo: AxisGeometry1D.h
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Define o pacote temporário de faces, centros e padrão de um eixo 1D.
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
#include <string>
#include <vector>

namespace fvgrid {

template <std::floating_point T>
struct BasicAxisGeometry1D final {
    using value_type = T;

    std::vector<T> faces;
    std::vector<T> centers;
    std::string pattern_name;
};

using AxisGeometry1D = BasicAxisGeometry1D<double>;
using AxisGeometry1DFloat = BasicAxisGeometry1D<float>;
using AxisGeometry1DLongDouble = BasicAxisGeometry1D<long double>;

}  // namespace fvgrid
