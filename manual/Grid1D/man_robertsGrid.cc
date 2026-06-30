// ============================================================================
// Arquivo: man_robertsGrid.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Programa de manual para gerar e imprimir uma malha 1D de Roberts.
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

#include <FVGridMaker/FVGridMaker.h>

int main() {
    //
    // Este exemplo gera uma malha 1D pela transformação de Roberts.
    //
    // A malha de Roberts é uma malha não uniforme suave. Ela é útil quando se
    // deseja concentrar volumes próximos às fronteiras sem introduzir saltos
    // aleatórios entre volumes vizinhos.
    //
    // O parâmetro beta controla a intensidade da transformação. A biblioteca
    // exige beta > 1.
    //
    // Neste exemplo, usamos explicitamente o padrão VolumeCentered1D. Assim, as
    // faces são geradas pela transformação de Roberts e os centros ficam nos
    // pontos médios dos volumes.
    //

    using Scalar = fvgrid::Real;
    using Axis = fvgrid::BasicAxis1D<Scalar>;

    const fvgrid::Size nvol = 10;
    const Scalar xinit = Scalar{0.0};
    const Scalar xfinal = Scalar{1.0};
    const Scalar beta = Scalar{2.0};

    if (!(std::isfinite(beta) && beta > Scalar{1.0})) {
        throw std::invalid_argument(
            "O parâmetro beta deve ser finito e maior que 1."
        );
    }

    const Axis axis = fvgrid::roberts_axis_1d(
        fvgrid::NVol{nvol},
        fvgrid::Length{xfinal - xinit},
        fvgrid::XInit{xinit},
        fvgrid::Beta{beta},
        fvgrid::VolumeCentered1D{}
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
    std::cout << "O bloco abaixo imprime algumas propriedades globais da malha:\n";
    std::cout << "número de volumes, número de faces, limite inferior, limite\n";
    std::cout << "superior, comprimento total do domínio e parâmetro beta.\n\n";

    std::cout << "número de volumes : " << axis.num_cells() << '\n';
    std::cout << "número de faces   : " << axis.num_faces() << '\n';
    std::cout << "xmin              : " << axis.xmin() << '\n';
    std::cout << "xmax              : " << axis.xmax() << '\n';
    std::cout << "comprimento       : " << axis.length() << '\n';
    std::cout << "beta              : " << beta << '\n';
    std::cout << "padrão            : " << axis.pattern_name() << '\n';

    constexpr int id_width = 6;
    constexpr int value_width = 14;

    std::cout << "\nCoordenadas das faces\n";
    std::cout << "=====================\n";
    std::cout << "O bloco abaixo imprime a coordenada de cada face da malha.\n";
    std::cout << "Na malha de Roberts, as faces não ficam igualmente espaçadas.\n\n";

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
    std::cout << "O bloco abaixo imprime, para cada volume finito, a coordenada\n";
    std::cout << "da face oeste, a coordenada do centro, a coordenada da face leste\n";
    std::cout << "e o comprimento do volume.\n\n";

    std::cout << std::right
              << std::setw(id_width) << "p"
              << std::setw(value_width) << "face_oeste"
              << std::setw(value_width) << "centro"
              << std::setw(value_width) << "face_leste"
              << std::setw(value_width) << "dx"
              << '\n';

    std::cout << std::string(id_width + 4 * value_width, '-') << '\n';

    for (fvgrid::Size p = 0; p < axis.num_cells(); ++p) {
        std::cout << std::right
                  << std::setw(id_width) << p
                  << std::setw(value_width) << axis.west_face(p)
                  << std::setw(value_width) << axis.center(p)
                  << std::setw(value_width) << axis.east_face(p)
                  << std::setw(value_width) << axis.cell_length(p)
                  << '\n';
    }

    //
    // Indicadores simples de concentração.
    //
    // A razão entre o maior e o menor volume dá uma primeira medida da
    // intensidade da não uniformidade produzida pela transformação.
    //

    Scalar min_dx = axis.cell_length(0);
    Scalar max_dx = axis.cell_length(0);

    for (fvgrid::Size p = 1; p < axis.num_cells(); ++p) {
        const Scalar dx = axis.cell_length(p);

        if (dx < min_dx) {
            min_dx = dx;
        }

        if (dx > max_dx) {
            max_dx = dx;
        }
    }

    std::cout << "\nIndicadores simples de concentração\n";
    std::cout << "===================================\n";
    std::cout << "O bloco abaixo resume a variação dos comprimentos dos volumes.\n\n";

    std::cout << "menor dx          : " << min_dx << '\n';
    std::cout << "maior dx          : " << max_dx << '\n';
    std::cout << "razão max/min     : " << max_dx / min_dx << '\n';

    //
    // Teste simples de consistência geométrica.
    //
    // Em uma malha 1D, o somatório dos comprimentos dos volumes deve reproduzir
    // o comprimento total do domínio.
    //
    // Isto é,
    //
    //     soma(dx[p]) = xmax - xmin.
    //
    // Na malha de Roberts, os volumes não têm o mesmo comprimento. Ainda assim,
    // a malha deve preencher exatamente o intervalo geométrico definido por
    // xmin e xmax.
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

    std::cout << std::scientific << std::setprecision(6);
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