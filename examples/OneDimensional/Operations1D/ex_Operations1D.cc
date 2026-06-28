// ============================================================================
// Arquivo: ex_Operations1D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Demonstra o uso de Operations1D com a API pública do FVGridMaker.
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
// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <iomanip>
#include <iostream>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>
#include <FVGridMaker/OneDimensional/Operations1D/AxisInterval1D.h>
#include <FVGridMaker/OneDimensional/Operations1D/Operations1D.h>

namespace {

void print_interval(
    const fvgrid::AxisInterval1D& interval
) {
    if (interval.is_empty()) {
        std::cout << "intersection: empty\n";
        return;
    }

    if (interval.is_point()) {
        std::cout << "intersection: point at x = "
                  << interval.lower()
                  << '\n';
        return;
    }

    std::cout << "intersection: interval ["
              << interval.lower()
              << ", "
              << interval.upper()
              << "]\n";
}

}  // namespace

int main() {
    const fvgrid::Axis1D left = fvgrid::Uniform1D::make(
        fvgrid::NVol{6},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0}
    );

    const fvgrid::Axis1D right = fvgrid::Uniform1D::make(
        fvgrid::NVol{6},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.35}
    );

    std::cout << std::fixed << std::setprecision(6);

    std::cout << "Left axis\n";
    std::cout << left << "\n\n";

    std::cout << "Right axis\n";
    std::cout << right << "\n\n";

    const fvgrid::AxisInterval1D intersection =
        fvgrid::Operations1D::intersection(left, right);

    print_interval(intersection);
    std::cout << '\n';

    if (intersection.is_interval()) {
        const fvgrid::Axis1D clipped =
            fvgrid::Operations1D::clip_faces_to_interval(
                left,
                intersection,
                1.0e-12
            );

        std::cout << "Left axis clipped to the intersection interval\n";
        std::cout << clipped << '\n';
    }

    return 0;
}