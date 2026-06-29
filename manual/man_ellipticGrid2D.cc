// ============================================================================
// Arquivo: man_ellipticGrid2D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Programa de manual para gerar uma malha elíptica confocal.
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

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <numbers>
#include <string>

#include <FVGridMaker/FVGridMaker.h>

namespace {

using Scalar = fvgrid::Real;

Scalar elliptic_scale_factor(Scalar a, Scalar mu, Scalar nu) {
    const Scalar sinh_mu = std::sinh(mu);
    const Scalar sin_nu = std::sin(nu);

    return a * std::sqrt(sinh_mu * sinh_mu + sin_nu * sin_nu);
}

Scalar elliptic_cell_measure(
    Scalar a,
    fvgrid::BasicCoordinateCell2D<Scalar> cell
) {
    const Scalar delta_mu = cell.first_max - cell.first_min;
    const Scalar delta_nu = cell.second_max - cell.second_min;

    return (a * a / Scalar{4.0})
        * (
            delta_nu
            * (
                std::sinh(Scalar{2.0} * cell.first_max)
                - std::sinh(Scalar{2.0} * cell.first_min)
            )
            - delta_mu
            * (
                std::sin(Scalar{2.0} * cell.second_max)
                - std::sin(Scalar{2.0} * cell.second_min)
            )
        );
}

template <class Function>
Scalar composite_simpson(
    Scalar lower,
    Scalar upper,
    std::size_t panels,
    Function function
) {
    if (upper == lower) {
        return Scalar{0.0};
    }

    if (panels < 2) {
        panels = 2;
    }

    if (panels % 2 != 0) {
        ++panels;
    }

    const Scalar h = (upper - lower) / static_cast<Scalar>(panels);

    Scalar sum = function(lower) + function(upper);

    for (std::size_t n = 1; n < panels; ++n) {
        const Scalar coordinate = lower + static_cast<Scalar>(n) * h;
        const Scalar weight = (n % 2 == 0) ? Scalar{2.0} : Scalar{4.0};

        sum += weight * function(coordinate);
    }

    return (h / Scalar{3.0}) * sum;
}

bool is_sample_cell(
    fvgrid::Size i,
    fvgrid::Size j,
    fvgrid::Size nx,
    fvgrid::Size ny
) {
    const bool first_direction_sample = (i == 0) || (i + 1 == nx);
    const bool second_direction_sample = (j == 0) || (j == ny / 2) ||
        (j + 1 == ny);

    return first_direction_sample && second_direction_sample;
}

}  // namespace

int main() {
    //
    // Este programa cria uma malha elíptica confocal como sistema de coordenadas
    // personalizado.
    //
    // As coordenadas lógicas são (mu, nu), e o mapeamento físico é:
    //
    //     x = a cosh(mu) cos(nu),
    //     y = a sinh(mu) sin(nu).
    //
    // A biblioteca não precisa conhecer "coordenadas elípticas" como um tipo
    // nativo. O usuário fornece o mapeamento, a medida da célula e as medidas
    // das faces por meio da fábrica make_basic_coordinate_mapping_2d.
    //

    using Axis = fvgrid::BasicAxis1D<Scalar>;
    using Grid = fvgrid::BasicStructuredGrid2D<Scalar>;

    const Scalar pi = std::numbers::pi_v<Scalar>;

    const Scalar a = Scalar{1.0};

    const Scalar mu_inner = Scalar{0.40};
    const Scalar mu_outer = Scalar{1.10};

    const Scalar nu_begin = Scalar{0.0};
    const Scalar nu_end = Scalar{2.0} * pi;

    const fvgrid::Size nvol_mu = 4;
    const fvgrid::Size nvol_nu = 16;

    const Axis mu_axis = fvgrid::uniform_axis_1d<Scalar>(
        nvol_mu,
        mu_inner,
        mu_outer
    );

    const Axis nu_axis = fvgrid::uniform_axis_1d<Scalar>(
        nvol_nu,
        nu_begin,
        nu_end
    );

    const auto elliptic_mapping = fvgrid::make_basic_coordinate_mapping_2d<
        Scalar
    >(
        "EllipticConfocal",
        "mu",
        "nu",
        [a](Scalar mu, Scalar nu) {
            return fvgrid::BasicPhysicalPoint2D<Scalar>{
                a * std::cosh(mu) * std::cos(nu),
                a * std::sinh(mu) * std::sin(nu),
                Scalar{0.0}
            };
        },
        [a](fvgrid::BasicCoordinateCell2D<Scalar> cell) {
            return elliptic_cell_measure(a, cell);
        },
        [a](Scalar mu, Scalar nu_min, Scalar nu_max) {
            return composite_simpson(
                nu_min,
                nu_max,
                96,
                [a, mu](Scalar nu) {
                    return elliptic_scale_factor(a, mu, nu);
                }
            );
        },
        [a](Scalar nu, Scalar mu_min, Scalar mu_max) {
            return composite_simpson(
                mu_min,
                mu_max,
                96,
                [a, nu](Scalar mu) {
                    return elliptic_scale_factor(a, mu, nu);
                }
            );
        },
        false
    );

    const Grid grid{mu_axis, nu_axis, elliptic_mapping};

    std::cout << std::fixed << std::setprecision(6);

    std::cout << "\nMalha elíptica confocal\n";
    std::cout << "=======================\n";
    std::cout << "O bloco abaixo apresenta uma malha estruturada em coordenadas\n";
    std::cout << "elípticas confocais. A malha lógica é retangular em (mu, nu),\n";
    std::cout << "mas as linhas coordenadas mapeadas formam elipses e hipérboles.\n\n";

    std::cout << "sistema de coordenadas : "
              << grid.coordinate_system_name() << '\n';
    std::cout << "coordenadas lógicas    : "
              << grid.first_coordinate_name() << ", "
              << grid.second_coordinate_name() << '\n';
    std::cout << "vtk rectilinear        : "
              << (grid.vtk_rectilinear() ? "sim" : "nao") << '\n';
    std::cout << "número de células em mu: " << grid.num_cells_x() << '\n';
    std::cout << "número de células em nu: " << grid.num_cells_y() << '\n';
    std::cout << "número total de células: " << grid.num_cells() << '\n';
    std::cout << "a                      : " << a << '\n';
    std::cout << "mu mínimo              : " << grid.xmin() << '\n';
    std::cout << "mu máximo              : " << grid.xmax() << '\n';
    std::cout << "nu mínimo              : " << grid.ymin() << '\n';
    std::cout << "nu máximo              : " << grid.ymax() << '\n';

    constexpr int id_width = 6;
    constexpr int value_width = 14;

    Scalar sum_logical_area = Scalar{0.0};
    Scalar sum_measure = Scalar{0.0};
    Scalar max_difference = Scalar{0.0};

    std::cout << "\nAmostra de células\n";
    std::cout << "==================\n";
    std::cout << "A tabela abaixo imprime apenas algumas células. O teste de\n";
    std::cout << "consistência usa todas as células da malha.\n\n";

    std::cout << std::right
              << std::setw(id_width) << "i"
              << std::setw(id_width) << "j"
              << std::setw(id_width) << "k"
              << std::setw(value_width) << "mu"
              << std::setw(value_width) << "nu"
              << std::setw(value_width) << "x_map"
              << std::setw(value_width) << "y_map"
              << std::setw(value_width) << "area_logica"
              << std::setw(value_width) << "medida"
              << '\n';

    std::cout << std::string(3 * id_width + 6 * value_width, '-') << '\n';

    for (fvgrid::Size j = 0; j < grid.num_cells_y(); ++j) {
        for (fvgrid::Size i = 0; i < grid.num_cells_x(); ++i) {
            const fvgrid::Size k = grid.linear_cell_index(i, j);

            const Scalar logical_area = grid.cell_logical_area(i, j);
            const Scalar measure = grid.cell_measure(i, j);

            const fvgrid::BasicCoordinateCell2D<Scalar> cell{
                grid.first_face(i),
                grid.first_face(i + 1),
                grid.second_face(j),
                grid.second_face(j + 1)
            };

            const Scalar expected_cell_measure =
                elliptic_cell_measure(a, cell);

            sum_logical_area += logical_area;
            sum_measure += measure;

            max_difference = std::max(
                max_difference,
                std::abs(measure - expected_cell_measure)
            );

            if (is_sample_cell(i, j, grid.num_cells_x(), grid.num_cells_y())) {
                const Scalar mu = grid.first_center(i);
                const Scalar nu = grid.second_center(j);
                const auto mapped_point = grid.physical_cell_center(i, j);

                std::cout << std::right
                          << std::setw(id_width) << i
                          << std::setw(id_width) << j
                          << std::setw(id_width) << k
                          << std::setw(value_width) << mu
                          << std::setw(value_width) << nu
                          << std::setw(value_width) << mapped_point.x
                          << std::setw(value_width) << mapped_point.y
                          << std::setw(value_width) << logical_area
                          << std::setw(value_width) << measure
                          << '\n';
            }
        }
    }

    const fvgrid::BasicCoordinateCell2D<Scalar> whole_domain{
        mu_inner,
        mu_outer,
        nu_begin,
        nu_end
    };

    const Scalar expected_measure = elliptic_cell_measure(a, whole_domain);
    const Scalar expected_logical_area =
        (mu_outer - mu_inner) * (nu_end - nu_begin);

    const Scalar measure_error = std::abs(sum_measure - expected_measure);
    const Scalar logical_error =
        std::abs(sum_logical_area - expected_logical_area);

    const Scalar tolerance = Scalar{1.0e-12};

    std::cout << "\nTeste de consistência geométrica\n";
    std::cout << "================================\n";
    std::cout << "O bloco abaixo compara a soma das medidas físicas das células\n";
    std::cout << "com a integral analítica do jacobiano elíptico no domínio.\n\n";

    std::cout << "soma das áreas lógicas       : " << sum_logical_area << '\n';
    std::cout << "área lógica esperada         : " << expected_logical_area << '\n';
    std::cout << "soma das medidas físicas     : " << sum_measure << '\n';
    std::cout << "medida física esperada       : " << expected_measure << '\n';

    std::cout << std::scientific;
    std::cout << "erro na área lógica          : " << logical_error << '\n';
    std::cout << "erro na medida física        : " << measure_error << '\n';
    std::cout << "maior diferença local        : " << max_difference << '\n';
    std::cout << "tolerância                   : " << tolerance << '\n';

    fvgrid::write_vtk(grid, "malha_eliptica.vtk");

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "arquivo VTK                  : malha_eliptica.vtk\n";

    const bool passed =
        logical_error <= tolerance &&
        measure_error <= tolerance &&
        max_difference <= tolerance;

    if (passed) {
        std::cout << "resultado                    : OK\n";
    } else {
        std::cout << "resultado                    : FALHOU\n";
        return 1;
    }

    return 0;
}