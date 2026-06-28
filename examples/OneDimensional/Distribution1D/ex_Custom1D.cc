// ============================================================================
// Arquivo: ex_Custom1D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Demonstra o uso de Custom1D com a API pública do FVGridMaker.
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
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/OneDimensional/Distribution1D/Custom1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Coordinates1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Domain1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/FaceCentered1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>

int main() {
    const fvgrid::Axis1D volume_centered_axis = fvgrid::Custom1D::make(
        fvgrid::Coordinates1D::faces(
            std::vector<fvgrid::Real>{0.0, 0.2, 0.7, 1.0}
        ),
        fvgrid::VolumeCentered1D{}
    );

    const fvgrid::Axis1D face_centered_axis = fvgrid::Custom1D::make(
        fvgrid::Coordinates1D::centers(
            std::vector<fvgrid::Real>{0.1, 0.45, 0.85}
        ),
        fvgrid::FaceCentered1D{},
        fvgrid::Domain1D::from_length(
            fvgrid::XInit{0.0},
            fvgrid::Length{1.0}
        )
    );

    std::cout << "Custom volume-centred axis\n";
    std::cout << volume_centered_axis << "\n\n";

    std::cout << "Custom face-centred axis\n";
    std::cout << face_centered_axis << '\n';

    return 0;
}