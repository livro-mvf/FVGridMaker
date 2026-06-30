// ============================================================================
// Arquivo: man_uniformGrid.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Programa de manual para gerar e imprimir uma malha 1D uniforme.
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
#include <string>

#include <FVGridMaker/FVGridMaker.h>

int main() {
    //
    // A malha é intencionalmente simples.
    //
    // Definimos o domínio cartesiano unidimensional [0, 1] e o dividimos em
    // 10 volumes finitos de mesmo comprimento.
    //

    using Scalar = fvgrid::Real;
    using Axis = fvgrid::BasicAxis1D<Scalar>;

    const fvgrid::Size nvol = 10;
    const Scalar xinit = Scalar{0.0};
    const Scalar xfinal = Scalar{1.0};

    // uniform_axis_1d<Scalar> cria as coordenadas das faces e constrói o objeto
    // BasicAxis1D<Scalar> correspondente. Neste exemplo, Scalar é fvgrid::Real,
    // que atualmente é um alias para double.

    const Axis axis = fvgrid::uniform_axis_1d<Scalar>(nvol, xinit, xfinal);

    // Usamos um formato numérico fixo para tornar a saída estável e fácil de
    // comparar com o resultado impresso no manual.

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

    // Em seguida, imprimimos manualmente algumas propriedades globais da malha.

    std::cout << "\nResumo manual da malha gerada\n";
    std::cout << "=============================\n";
    std::cout << "O bloco abaixo imprime algumas propriedades globais da malha:\n";
    std::cout << "número de volumes, número de faces, limite inferior, limite\n";
    std::cout << "superior e comprimento total do domínio.\n\n";

    std::cout << "número de volumes : " << axis.num_cells() << '\n';
    std::cout << "número de faces   : " << axis.num_faces() << '\n';
    std::cout << "xmin              : " << axis.xmin() << '\n';
    std::cout << "xmax              : " << axis.xmax() << '\n';
    std::cout << "comprimento       : " << axis.length() << '\n';

    // Agora imprimimos as coordenadas das faces.
    //
    // Como foram gerados 10 volumes, a malha possui 11 faces. Os índices das
    // faces variam de 0 até 10.

    std::cout << "\nCoordenadas das faces\n";
    std::cout << "=====================\n";
    std::cout << "O bloco abaixo imprime a coordenada de cada face da malha.\n";
    std::cout << "Como foram gerados 10 volumes, a malha possui 11 faces.\n";
    std::cout << "Os índices das faces variam de 0 até 10.\n\n";

    constexpr int id_width = 6;
    constexpr int value_width = 14;

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

    // Por fim, imprimimos as informações geométricas associadas aos volumes.
    //
    // Para cada volume finito, mostramos a face oeste, o centro, a face leste
    // e o comprimento do volume.

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
    // Teste simples de consistência geométrica.
    //
    // Em uma malha 1D, o somatório dos comprimentos dos volumes deve reproduzir
    // o comprimento total do domínio.
    //
    // Isto é,
    //
    //     soma(dx[p]) = xmax - xmin.
    //
    // Na malha uniforme, todos os volumes têm o mesmo comprimento, mas a
    // verificação continua sendo importante: ela confirma que a discretização
    // preenche o domínio sem lacunas e sem sobreposição de volumes.
    //
    // Como estamos usando números de ponto flutuante, a comparação é feita com
    // uma pequena tolerância.
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
    std::cout << "reproduz o comprimento total do domínio.\n";
    std::cout << "Na malha uniforme, isso equivale a verificar que nvol * dx\n";
    std::cout << "fecha exatamente o intervalo geométrico [xmin, xmax].\n\n";

    std::cout << "soma dos dx      : " << sum_dx << "\n";
    std::cout << "comprimento eixo : " << expected_length << "\n";
    std::cout << "erro absoluto    : " << error << "\n";
    std::cout << "tolerância       : " << tolerance << "\n";

    if (error <= tolerance) {
        std::cout << "resultado        : OK\n";
    } else {
        std::cout << "resultado        : FALHOU\n";
        return 1;
    }

    return 0;
}
