// ============================================================================
// Arquivo: ex_Axis1D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Demonstra o uso de Axis1D com a API pública do FVGridMaker.
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
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>

int main() {
    const fvgrid::Axis1D axis{
        std::vector<fvgrid::Real>{0.0, 0.25, 0.5, 1.0}
    };

    std::cout << "Axis1D example\n";
    std::cout << "pattern         : " << axis.pattern_name() << '\n';
    std::cout << "number of faces : " << axis.num_faces() << '\n';
    std::cout << "number of cells : " << axis.num_cells() << '\n';
    std::cout << "xmin            : " << axis.xmin() << '\n';
    std::cout << "xmax            : " << axis.xmax() << '\n';
    std::cout << "length          : " << axis.length() << '\n';

    std::cout << "faces           : ";
    for (const fvgrid::Real face : axis.faces()) {
        std::cout << face << ' ';
    }
    std::cout << '\n';

    std::cout << "centers         : ";
    for (const fvgrid::Real center : axis.centers()) {
        std::cout << center << ' ';
    }
    std::cout << '\n';

    std::cout << "cell lengths    : ";
    for (const fvgrid::Real cell_length : axis.cell_lengths()) {
        std::cout << cell_length << ' ';
    }
    std::cout << '\n';

    std::cout << "finite-volume distances\n";

    for (fvgrid::Size p = 0; p < axis.num_cells(); ++p) {
        std::cout << "cell " << p
                  << " west_face=" << axis.west_face(p)
                  << " center=" << axis.center(p)
                  << " east_face=" << axis.east_face(p)
                  << " DxP=" << axis.DxP(p);

        if (p > 0) {
            std::cout << " DxW=" << axis.DxW(p)
                      << " deltaxw=" << axis.deltaxw(p);
        }

        if (p + static_cast<fvgrid::Size>(1) < axis.num_cells()) {
            std::cout << " DxE=" << axis.DxE(p)
                      << " deltaxe=" << axis.deltaxe(p);
        }

        std::cout << '\n';
    }

    return 0;
}