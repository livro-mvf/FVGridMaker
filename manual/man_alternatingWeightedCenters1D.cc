// ============================================================================
// Arquivo: man_alternatingWeightedCenters1D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Programa de manual para gerar e imprimir uma malha 1D com centros ponderados alternados.
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

#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <FVGridMaker/FVGridMaker.h>

int main() {
    //
    // Este exemplo cria uma malha 1D que não usa nem o padrão face centrada
    // nem o padrão volume centrado usual.
    //
    // As faces são obtidas de uma malha uniforme criada pela própria biblioteca.
    // Depois, os centros são reconstruídos a partir dessas faces por uma regra
    // de pesos alternados.
    //
    // Nos volumes pares:
    //
    //     x_P = x_W + alpha * (x_E - x_W).
    //
    // Nos volumes ímpares:
    //
    //     x_P = x_W + (1 - alpha) * (x_E - x_W).
    //
    // Quando alpha = 0.5, o padrão volta ao caso volume centrado usual.
    //

    using Scalar = fvgrid::Real;
    using Axis = fvgrid::BasicAxis1D<Scalar>;

    const fvgrid::Size nvol = 10;
    const Scalar xinit = Scalar{0.0};
    const Scalar xfinal = Scalar{1.0};
    const Scalar alpha = Scalar{0.20};

    if (!(std::isfinite(alpha) && alpha > Scalar{0.0} && alpha < Scalar{1.0})) {
        throw std::invalid_argument(
            "O parâmetro alpha deve pertencer ao intervalo aberto (0, 1)."
        );
    }

    //
    // Primeiro, criamos uma malha uniforme com o gerador da biblioteca.
    //
    // Aqui, essa malha uniforme é usada apenas como fonte das faces. Os centros
    // dela não serão usados no eixo final.
    //

    const Axis uniform_axis =
        fvgrid::uniform_axis_1d<Scalar>(nvol, xinit, xfinal);

    //
    // custom_axis_1d recebe as coordenadas primárias em um std::vector.
    // Por isso, copiamos as faces da malha uniforme auxiliar para um std::vector.
    //

    std::vector<Scalar> faces(
        uniform_axis.faces().begin(),
        uniform_axis.faces().end()
    );

    //
    // Esta é a regra de pesos.
    //
    // Para volumes pares, ela retorna alpha.
    // Para volumes ímpares, ela retorna 1 - alpha.
    //

    const auto alternating_weight = [alpha](fvgrid::Size p) {
        if (p % static_cast<fvgrid::Size>(2) == 0) {
            return alpha;
        }

        return Scalar{1.0} - alpha;
    };

    //
    // CentersFromFaces1D é o reconstrutor parametrizado da biblioteca.
    //
    // Ele recebe faces como coordenadas primárias e calcula os centros com a
    // regra de pesos fornecida.
    //

    const Axis axis = fvgrid::custom_axis_1d(
        fvgrid::BasicCoordinates1D<Scalar>::faces(std::move(faces)),
        fvgrid::CentersFromFaces1D{alternating_weight}
    );

    std::cout << std::fixed << std::setprecision(6);

    //
    // Primeiro, imprimimos uma visão compacta da malha.
    //
    // Esta seção substitui a saída bruta do operador << por rótulos em
    // português, mantendo o foco didático do programa de manual.
    //

    std::cout << "\nResumo compacto da malha\n";
    std::cout << "========================\n";
    std::cout << "O bloco abaixo apresenta uma visão compacta da malha gerada:\n";
    std::cout << "padrão de centralização, quantidade de faces e volumes,\n";
    std::cout << "limites físicos e comprimento total do domínio.\n\n";

    std::cout << "padrão            : " << axis.pattern_name() << '\n';
    std::cout << "número de faces   : " << axis.num_faces() << '\n';
    std::cout << "número de volumes : " << axis.num_cells() << '\n';
    std::cout << "xmin              : " << axis.xmin() << '\n';
    std::cout << "xmax              : " << axis.xmax() << '\n';
    std::cout << "comprimento       : " << axis.length() << '\n';

    std::cout << "\nResumo manual da malha gerada\n";
    std::cout << "=============================\n";
    std::cout << "O bloco abaixo imprime algumas propriedades globais da malha.\n\n";

    std::cout << "número de volumes : " << axis.num_cells() << '\n';
    std::cout << "número de faces   : " << axis.num_faces() << '\n';
    std::cout << "xmin              : " << axis.xmin() << '\n';
    std::cout << "xmax              : " << axis.xmax() << '\n';
    std::cout << "comprimento       : " << axis.length() << '\n';
    std::cout << "alpha             : " << alpha << '\n';
    std::cout << "padrão            : " << axis.pattern_name() << '\n';

    constexpr int id_width = 6;
    constexpr int value_width = 14;

    std::cout << "\nCoordenadas das faces\n";
    std::cout << "=====================\n";
    std::cout << "O bloco abaixo imprime a coordenada de cada face da malha.\n";
    std::cout << "As faces vieram de uma malha uniforme gerada pela biblioteca.\n\n";

    std::cout << std::right
              << std::setw(id_width) << "i"
              << std::setw(value_width) << "x_face"
              << '\n';

    std::cout << std::string(id_width + value_width, '-') << '\n';

    for (fvgrid::Size i = 0; i < axis.num_faces(); ++i) {
        std::cout << std::right
                  << std::setw(id_width) << i
                  << std::setw(value_width) << axis.face(i)
                  << '\n';
    }

    std::cout << "\nInformações geométricas dos volumes\n";
    std::cout << "===================================\n";
    std::cout << "O bloco abaixo mostra a alternância da posição dos centros.\n";
    std::cout << "Volumes pares usam alpha. Volumes ímpares usam 1 - alpha.\n\n";

    std::cout << std::right
              << std::setw(id_width) << "p"
              << std::setw(value_width) << "peso"
              << std::setw(value_width) << "face_oeste"
              << std::setw(value_width) << "centro"
              << std::setw(value_width) << "face_leste"
              << std::setw(value_width) << "dx"
              << '\n';

    std::cout << std::string(id_width + 5 * value_width, '-') << '\n';

    for (fvgrid::Size p = 0; p < axis.num_cells(); ++p) {
        const Scalar weight = alternating_weight(p);

        std::cout << std::right
                  << std::setw(id_width) << p
                  << std::setw(value_width) << weight
                  << std::setw(value_width) << axis.west_face(p)
                  << std::setw(value_width) << axis.center(p)
                  << std::setw(value_width) << axis.east_face(p)
                  << std::setw(value_width) << axis.cell_length(p)
                  << '\n';
    }

    //
    // Teste simples de consistência geométrica.
    //
    // O posicionamento dos centros não deve alterar os comprimentos dos
    // volumes. Portanto, a soma dos dx deve continuar reproduzindo o
    // comprimento total do domínio.
    //

    Scalar sum_dx = Scalar{0.0};

    for (fvgrid::Size p = 0; p < axis.num_cells(); ++p) {
        sum_dx += axis.cell_length(p);
    }

    const Scalar expected_length = axis.length();
    const Scalar error = std::abs(sum_dx - expected_length);
    const Scalar tolerance = Scalar{1.0e-12};

    std::cout << "\nTeste de consistência geométrica\n";
    std::cout << "================================\n";
    std::cout << "O bloco abaixo verifica se a soma dos comprimentos dos volumes\n";
    std::cout << "reproduz o comprimento total do domínio.\n\n";

    std::cout << "soma dos dx      : " << sum_dx << '\n';
    std::cout << "comprimento eixo : " << expected_length << '\n';
    std::cout << "erro absoluto    : " << error << '\n';
    std::cout << "tolerância       : " << tolerance << '\n';

    if (error <= tolerance) {
        std::cout << "resultado        : OK\n";
    } else {
        std::cout << "resultado        : FALHOU\n";
        return 1;
    }

    return 0;
}
