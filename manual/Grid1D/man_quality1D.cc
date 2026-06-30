// ============================================================================
// Arquivo: man_quality1D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Programa de manual para avaliar métricas simples de qualidade de
//            malhas 1D usando o módulo Quality1D da própria biblioteca.
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
#include <string_view>

#include <FVGridMaker/FVGridMaker.h>

namespace {

using Scalar = fvgrid::Real;
using Axis = fvgrid::BasicAxis1D<Scalar>;

constexpr int label_width = 26;
constexpr int value_width = 16;

//
// Imprime as métricas de qualidade de um eixo.
//
// O ponto central deste exemplo está aqui: as métricas NÃO são recalculadas à
// mão. Elas vêm do módulo Quality1D da própria biblioteca, por meio da função
// quality_metrics_1d, que devolve um QualityMetrics1D já preenchido.
//
// A única conta feita manualmente é o desvio relativo dos comprimentos, que a
// biblioteca não fornece. Ele serve para ilustrar que o usuário pode compor
// suas próprias medidas a partir dos dados geométricos do eixo, quando precisar
// de algo que ainda não existe na biblioteca.
//
void print_quality_report(const Axis& axis, std::string_view title) {
    std::cout << std::fixed << std::setprecision(6);

    std::cout << "\n" << title << "\n";
    std::cout << std::string(title.size(), '=') << "\n";
    std::cout << "padrão            : " << axis.pattern_name() << '\n';
    std::cout << "número de volumes : " << axis.num_cells() << '\n';
    std::cout << "comprimento       : " << axis.length() << "\n\n";

    // Fonte oficial das métricas: a biblioteca.
    const fvgrid::QualityMetrics1D metrics = fvgrid::quality_metrics_1d(axis);

    std::cout << std::left << std::setw(label_width) << "menor dx"
              << std::right << std::setw(value_width)
              << metrics.min_cell_length << '\n';

    std::cout << std::left << std::setw(label_width) << "maior dx"
              << std::right << std::setw(value_width)
              << metrics.max_cell_length << '\n';

    std::cout << std::left << std::setw(label_width) << "razão maior/menor dx"
              << std::right << std::setw(value_width)
              << metrics.cell_length_ratio << '\n';

    std::cout << std::left << std::setw(label_width)
              << "maior razão entre vizinhos"
              << std::right << std::setw(value_width)
              << metrics.max_adjacent_cell_ratio << '\n';

    //
    // Conferência manual de uma única métrica.
    //
    // Recalculamos a razão maior/menor diretamente a partir de cell_lengths()
    // e comparamos com o valor devolvido pela biblioteca. Os dois devem
    // coincidir. Este passo existe só para mostrar, uma vez, que o número da
    // biblioteca é o mesmo que a definição da métrica produz.
    //
    const std::span<const Scalar> dx = axis.cell_lengths();

    Scalar manual_min = dx[0];
    Scalar manual_max = dx[0];

    for (const Scalar value : dx) {
        if (value < manual_min) {
            manual_min = value;
        }

        if (value > manual_max) {
            manual_max = value;
        }
    }

    const Scalar manual_ratio = manual_max / manual_min;
    const Scalar ratio_diff =
        std::abs(manual_ratio - metrics.cell_length_ratio);

    std::cout << std::left << std::setw(label_width)
              << "conferência (manual)"
              << std::right << std::setw(value_width) << manual_ratio << '\n';

    const std::ios_base::fmtflags old_flags = std::cout.flags();
    const std::streamsize old_precision = std::cout.precision();

    std::cout << std::scientific << std::setprecision(6);
    std::cout << std::left << std::setw(label_width)
              << "diferença biblioteca/manual"
              << std::right << std::setw(value_width) << ratio_diff << '\n';

    std::cout.flags(old_flags);
    std::cout.precision(old_precision);

    //
    // Desvio relativo dos comprimentos.
    //
    // Esta métrica não faz parte de Quality1D. Ela é composta aqui a partir dos
    // próprios comprimentos: mede o quanto os dx se afastam, em média, do
    // comprimento médio. Para a malha uniforme, o resultado é próximo de zero.
    //
    Scalar sum_dx = Scalar{0.0};

    for (const Scalar value : dx) {
        sum_dx += value;
    }

    const Scalar mean_dx =
        sum_dx / static_cast<Scalar>(axis.num_cells());

    Scalar sum_abs_dev = Scalar{0.0};

    for (const Scalar value : dx) {
        sum_abs_dev += std::abs(value - mean_dx);
    }

    const Scalar relative_deviation =
        (mean_dx > Scalar{0.0})
            ? (sum_abs_dev / static_cast<Scalar>(axis.num_cells())) / mean_dx
            : Scalar{0.0};

    std::cout << std::left << std::setw(label_width)
              << "desvio relativo dos dx"
              << std::right << std::setw(value_width)
              << relative_deviation << '\n';
}

}  // namespace

int main() {
    //
    // Este exemplo compara métricas simples de qualidade em três malhas com o
    // mesmo domínio e o mesmo número de volumes: uniforme, Roberts e aleatória.
    //
    // As métricas vêm do módulo Quality1D da biblioteca. A intenção é dupla:
    //
    // 1. mostrar que a FVGridMaker já fornece essas medidas prontas, sem que o
    //    usuário precise reimplementá-las;
    // 2. lembrar que essas métricas são uma inspeção inicial. Elas ajudam a
    //    detectar problemas grosseiros de construção, mas não decidem, sozinhas,
    //    se uma malha é adequada para um problema numérico específico.
    //

    const fvgrid::Size nvol = 10;
    const Scalar xinit = Scalar{0.0};
    const Scalar xfinal = Scalar{1.0};
    const Scalar beta = Scalar{2.0};
    const fvgrid::UInt64 seed = 20260627;

    const Axis uniform_axis =
        fvgrid::uniform_axis_1d<Scalar>(nvol, xinit, xfinal);

    const Axis roberts_axis = fvgrid::roberts_axis_1d(
        fvgrid::NVol{nvol},
        fvgrid::Length{xfinal - xinit},
        fvgrid::XInit{xinit},
        fvgrid::Beta{beta},
        fvgrid::VolumeCentered1D{}
    );

    const Axis random_axis =
        fvgrid::random_axis_1d<Scalar>(nvol, xinit, xfinal, seed);

    print_quality_report(uniform_axis, "Malha uniforme");
    print_quality_report(roberts_axis, "Malha de Roberts");
    print_quality_report(random_axis, "Malha aleatória");

    //
    // Leitura esperada dos resultados:
    //
    // - a malha uniforme deve ter razão maior/menor próxima de 1 e desvio
    //   relativo próximo de zero;
    // - a malha de Roberts deve mostrar variação controlada, com razão entre
    //   vizinhos suave;
    // - a malha aleatória pode apresentar a maior razão maior/menor e os saltos
    //   locais mais fortes entre volumes vizinhos.
    //

    std::cout << "\nObservação\n";
    std::cout << "==========\n";
    std::cout << "As métricas acima são alertas, não vereditos. Uma malha pode\n";
    std::cout << "passar em todas elas e ainda assim não ser adequada para uma\n";
    std::cout << "equação específica. Elas servem para encontrar irregularidades\n";
    std::cout << "e erros de construção antes do uso numérico.\n";

    return 0;
}