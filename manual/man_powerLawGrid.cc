// ============================================================================
// Arquivo: man_powerLawGrid.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Programa de manual para gerar e imprimir uma malha 1D por lei de potência.
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
#include <utility>
#include <vector>

#include <FVGridMaker/FVGridMaker.h>

int main() {
    //
    // Este exemplo mostra como gerar uma nova malha unidimensional a partir de
    // uma função de mapeamento.
    //
    // A ideia central é simples:
    //
    // 1. definir uma função x(eta), com 0 <= eta <= 1;
    // 2. usar essa função para gerar as faces da malha;
    // 3. informar à biblioteca que essas faces são as coordenadas primárias;
    // 4. usar VolumeCentered1D para reconstruir os centros dos volumes.
    //
    // No padrão volume centrado, as faces são conhecidas primeiro. Os centros
    // são calculados depois a partir das faces.
    //

    using Scalar = fvgrid::Real;
    using Axis = fvgrid::BasicAxis1D<Scalar>;

    const fvgrid::Size nvol = 10;
    const Scalar xinit = Scalar{0.0};
    const Scalar xfinal = Scalar{1.0};
    const Scalar gamma = Scalar{2.0};

    const Scalar length = xfinal - xinit;

    //
    // Aqui definimos o novo padrão de distribuição das faces.
    //
    // A variável eta pertence ao intervalo lógico [0, 1]. A função abaixo
    // transforma eta na coordenada física x.
    //
    // Para gamma = 1, a malha é uniforme.
    // Para gamma > 1, a malha fica mais refinada perto de xinit.
    // Para 0 < gamma < 1, a malha fica mais refinada perto de xfinal.
    //

    const auto power_law_map = [xinit, length, gamma](Scalar eta) {
        return xinit + length * std::pow(eta, gamma);
    };

    //
    // VolumeCentered1D::primary_coordinates_from_map gera as coordenadas
    // primárias do padrão volume centrado.
    //
    // Como o padrão é volume centrado, essas coordenadas primárias são as faces.
    // Para nvol volumes, são geradas nvol + 1 faces.
    //
    // Neste exemplo:
    //
    // eta = 0/10, 1/10, 2/10, ..., 10/10.
    //

    std::vector<Scalar> faces =
        fvgrid::VolumeCentered1D::primary_coordinates_from_map<Scalar>(
            nvol,
            power_law_map
        );

    //
    // Agora construímos o eixo unidimensional.
    //
    // BasicCoordinates1D<Scalar>::faces informa que o vetor contém faces.
    // VolumeCentered1D informa que os centros devem ser reconstruídos a partir
    // dessas faces.
    //

    const Axis axis = fvgrid::custom_axis_1d(
        fvgrid::BasicCoordinates1D<Scalar>::faces(std::move(faces)),
        fvgrid::VolumeCentered1D{}
    );

    //
    // Usamos um formato numérico fixo para tornar a saída estável e fácil de
    // comparar com o resultado impresso no manual.
    //

    std::cout << std::fixed << std::setprecision(6);

    //
    // Primeiro, imprimimos a representação textual padrão fornecida pela
    // biblioteca para o objeto Axis1D.
    //

    std::cout << "\nResumo automático gerado pelo operador <<\n";
    std::cout << "========================================\n";
    std::cout << "O bloco abaixo mostra a representação textual padrão de Axis1D.\n";
    std::cout << "Essa impressão é fornecida diretamente pela biblioteca e serve\n";
    std::cout << "para uma inspeção rápida da malha criada.\n";
    std::cout << "O padrão deve aparecer como VolumeCentered1D.\n\n";

    std::cout << axis << '\n';

    //
    // Em seguida, imprimimos manualmente algumas propriedades globais da malha.
    //

    std::cout << "\nResumo manual da malha gerada\n";
    std::cout << "=============================\n";
    std::cout << "O bloco abaixo imprime algumas propriedades globais da malha:\n";
    std::cout << "número de volumes, número de faces, limite inferior, limite\n";
    std::cout << "superior, comprimento total do domínio e expoente usado.\n\n";

    std::cout << "número de volumes : " << axis.num_cells() << '\n';
    std::cout << "número de faces   : " << axis.num_faces() << '\n';
    std::cout << "xmin              : " << axis.xmin() << '\n';
    std::cout << "xmax              : " << axis.xmax() << '\n';
    std::cout << "comprimento       : " << axis.length() << '\n';
    std::cout << "expoente gamma    : " << gamma << '\n';
    std::cout << "padrão            : " << axis.pattern_name() << '\n';

    //
    // Agora imprimimos as coordenadas das faces.
    //

    std::cout << "\nCoordenadas das faces\n";
    std::cout << "=====================\n";
    std::cout << "O bloco abaixo imprime a coordenada de cada face da malha.\n";
    std::cout << "Como gamma = 2, as primeiras faces ficam mais próximas entre si.\n\n";

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

    //
    // Por fim, imprimimos as informações geométricas associadas aos volumes.
    //

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
