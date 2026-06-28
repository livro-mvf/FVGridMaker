// ============================================================================
// Arquivo: man_quality1D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Programa de manual para avaliar métricas de qualidade de malhas 1D.
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
#include <limits>
#include <string>
#include <string_view>
#include <vector>

#include <FVGridMaker/FVGridMaker.h>

namespace {

using Scalar = fvgrid::Real;
using Axis = fvgrid::BasicAxis1D<Scalar>;

constexpr int id_width = 6;
constexpr int value_width = 16;

struct QualityMetrics final {
    Scalar sum_dx{};
    Scalar expected_length{};
    Scalar closure_error{};
    Scalar min_dx{};
    Scalar max_dx{};
    Scalar mean_dx{};
    Scalar relative_stddev{};
    Scalar concentration_ratio{};
    Scalar max_local_ratio{};
    fvgrid::Size min_index{};
    fvgrid::Size max_index{};
};

[[nodiscard]] QualityMetrics compute_quality_metrics(const Axis& axis) {
    QualityMetrics metrics;

    const fvgrid::Size n = axis.num_cells();

    metrics.expected_length = axis.length();
    metrics.min_dx = std::numeric_limits<Scalar>::max();
    metrics.max_dx = Scalar{0.0};
    metrics.max_local_ratio = Scalar{1.0};

    std::vector<Scalar> dx_values(n);

    for (fvgrid::Size p = 0; p < n; ++p) {
        const Scalar dx = axis.cell_length(p);

        dx_values[p] = dx;
        metrics.sum_dx += dx;

        if (dx < metrics.min_dx) {
            metrics.min_dx = dx;
            metrics.min_index = p;
        }

        if (dx > metrics.max_dx) {
            metrics.max_dx = dx;
            metrics.max_index = p;
        }
    }

    metrics.mean_dx = metrics.sum_dx / static_cast<Scalar>(n);
    metrics.closure_error = std::abs(metrics.sum_dx - metrics.expected_length);
    metrics.concentration_ratio = metrics.max_dx / metrics.min_dx;

    Scalar variance = Scalar{0.0};

    for (fvgrid::Size p = 0; p < n; ++p) {
        const Scalar diff = dx_values[p] - metrics.mean_dx;
        variance += diff * diff;
    }

    variance /= static_cast<Scalar>(n);
    metrics.relative_stddev = std::sqrt(variance) / metrics.mean_dx;

    for (fvgrid::Size p = 1; p < n; ++p) {
        const Scalar left = dx_values[p - static_cast<fvgrid::Size>(1)];
        const Scalar right = dx_values[p];

        const Scalar local_ratio = (left > right)
            ? left / right
            : right / left;

        if (local_ratio > metrics.max_local_ratio) {
            metrics.max_local_ratio = local_ratio;
        }
    }

    return metrics;
}

void print_scientific_value(std::string_view label, Scalar value) {
    const std::ios_base::fmtflags old_flags = std::cout.flags();
    const std::streamsize old_precision = std::cout.precision();

    std::cout << std::scientific << std::setprecision(6);
    std::cout << label << value << '\n';

    std::cout.flags(old_flags);
    std::cout.precision(old_precision);
}

void print_quality_report(
    const Axis& axis,
    std::string_view title,
    std::string_view explanation
) {
    const QualityMetrics metrics = compute_quality_metrics(axis);

    std::cout << std::fixed << std::setprecision(6);

    std::cout << "\n" << title << "\n";
    std::cout << std::string(title.size(), '=') << "\n";
    std::cout << explanation << "\n\n";

    std::cout << "Resumo compacto da malha\n";
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

    std::cout << "\nMétricas de qualidade\n";
    std::cout << "=====================\n";
    std::cout << "soma dos dx              : " << metrics.sum_dx << '\n';
    std::cout << "comprimento esperado     : " << metrics.expected_length << '\n';
    print_scientific_value("erro de fechamento       : ", metrics.closure_error);
    std::cout << "menor dx                 : " << metrics.min_dx
              << "   no volume " << metrics.min_index << '\n';
    std::cout << "maior dx                 : " << metrics.max_dx
              << "   no volume " << metrics.max_index << '\n';
    std::cout << "dx médio                 : " << metrics.mean_dx << '\n';
    std::cout << "razão maior/menor dx     : " << metrics.concentration_ratio << '\n';
    std::cout << "maior razão local        : " << metrics.max_local_ratio << '\n';
    std::cout << "desvio relativo dos dx   : " << metrics.relative_stddev << '\n';

    std::cout << "\nInformações geométricas dos volumes\n";
    std::cout << "===================================\n";
    std::cout << "O bloco abaixo imprime, para cada volume finito, a face oeste,\n";
    std::cout << "a face leste, o comprimento do volume e a razão local.\n";
    std::cout << "A razão local compara o volume atual com o volume anterior.\n";
    std::cout << "Para p = 0, não há volume anterior.\n\n";

    std::cout << std::right
              << std::setw(id_width) << "p"
              << std::setw(value_width) << "face_oeste"
              << std::setw(value_width) << "face_leste"
              << std::setw(value_width) << "dx"
              << std::setw(value_width) << "razão_local"
              << '\n';

    std::cout << std::string(id_width + 4 * value_width, '-') << '\n';

    for (fvgrid::Size p = 0; p < axis.num_cells(); ++p) {
        std::cout << std::right
                  << std::setw(id_width) << p
                  << std::setw(value_width) << axis.west_face(p)
                  << std::setw(value_width) << axis.east_face(p)
                  << std::setw(value_width) << axis.cell_length(p);

        if (p == 0) {
            std::cout << std::setw(value_width) << "-";
        } else {
            const Scalar previous =
                axis.cell_length(p - static_cast<fvgrid::Size>(1));
            const Scalar current = axis.cell_length(p);

            const Scalar local_ratio = (previous > current)
                ? previous / current
                : current / previous;

            std::cout << std::setw(value_width) << local_ratio;
        }

        std::cout << '\n';
    }
}

[[nodiscard]] bool check_geometric_consistency(
    const Axis& axis,
    std::string_view label
) {
    const QualityMetrics metrics = compute_quality_metrics(axis);
    const Scalar tolerance = Scalar{1.0e-12};

    std::cout << "\nTeste de consistência geométrica\n";
    std::cout << "================================\n";
    std::cout << "O bloco abaixo verifica se a soma dos comprimentos dos volumes\n";
    std::cout << "reproduz o comprimento total do domínio.\n\n";
    std::cout << "malha testada    : " << label << '\n';
    std::cout << "soma dos dx      : " << std::fixed << std::setprecision(6)
              << metrics.sum_dx << '\n';
    std::cout << "comprimento eixo : " << metrics.expected_length << '\n';
    print_scientific_value("erro absoluto    : ", metrics.closure_error);
    print_scientific_value("tolerância       : ", tolerance);

    if (metrics.closure_error <= tolerance) {
        std::cout << "resultado        : OK\n";
        return true;
    }

    std::cout << "resultado        : FALHOU\n";
    return false;
}

}  // namespace

int main() {
    //
    // Este exemplo compara métricas simples de qualidade em três malhas 1D:
    //
    // 1. malha uniforme;
    // 2. malha de Roberts;
    // 3. malha aleatória.
    //
    // As métricas usadas aqui não são um veredito universal sobre a malha. Elas
    // servem como alertas práticos:
    //
    // - erro de fechamento geométrico;
    // - menor e maior comprimento de volume;
    // - razão entre maior e menor volume;
    // - maior razão local entre volumes vizinhos;
    // - desvio relativo dos comprimentos dos volumes.
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

    print_quality_report(
        uniform_axis,
        "Malha uniforme",
        "A malha uniforme é o caso de referência. Todos os volumes têm o "
        "mesmo comprimento e as razões de qualidade devem ficar próximas de 1."
    );

    if (!check_geometric_consistency(uniform_axis, "uniforme")) {
        return 1;
    }

    print_quality_report(
        roberts_axis,
        "Malha de Roberts",
        "A malha de Roberts concentra volumes de maneira suave. A razão entre "
        "maior e menor volume cresce, mas a variação local permanece controlada."
    );

    if (!check_geometric_consistency(roberts_axis, "Roberts")) {
        return 1;
    }

    print_quality_report(
        random_axis,
        "Malha aleatória",
        "A malha aleatória é útil como teste de robustez. Ela tende a produzir "
        "saltos mais fortes entre volumes vizinhos."
    );

    if (!check_geometric_consistency(random_axis, "aleatória")) {
        return 1;
    }

    return 0;
}