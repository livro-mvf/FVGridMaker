// ============================================================================
// Arquivo: ex_Roberts1D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Demonstra o uso de Roberts1D com a API pública do FVGridMaker.
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
#include <iostream>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/OneDimensional/Distribution1D/Roberts1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/FaceCentered1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>

int main() {
    const fvgrid::Axis1D malha = fvgrid::Roberts1D::make(
        fvgrid::NVol{12},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0},
        fvgrid::Beta{1.5}
    );

    const fvgrid::Axis1D volume_centered_axis = fvgrid::Roberts1D::make(
        fvgrid::NVol{12},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0},
        fvgrid::Beta{1.5},
        fvgrid::VolumeCentered1D{}
    );

    const fvgrid::Axis1D face_centered_axis = fvgrid::Roberts1D::make(
        fvgrid::NVol{12},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0},
        fvgrid::Beta{1.5},
        fvgrid::FaceCentered1D{}
    );

    std::cout << "Roberts1D default face-centred axis\n";
    std::cout << malha << "\n";

    std::cout << "Roberts1D explicit volume-centred axis\n";
    std::cout << volume_centered_axis << "\n\n";

    std::cout << "Roberts1D explicit face-centred axis\n";
    std::cout << face_centered_axis << '\n';

    return 0;
}
