// File: ex_UniformCartesian1D.cc
// Project: FVGridMaker
// Description: Gera e imprime uma malha cartesiana uniforme unidimensional.

#include <iomanip>
#include <iostream>

#include <FVGridMaker/FVGridMaker.h>

int main() {
    // A primeira malha do manual é intencionalmente simples.
    //
    // Definimos o domínio cartesiano unidimensional [0, 1] e o dividimos em
    // 10 volumes finitos de mesmo comprimento. Em uma dimensão, uma malha com
    // nvol volumes finitos possui nvol + 1 faces.

    using Scalar = fvgrid::Real;
    using Axis = fvgrid::BasicAxis1D<Scalar>;

    const fvgrid::Size nvol = 10;
    const Scalar xinit = Scalar{0.0};
    const Scalar xfinal = Scalar{1.0};

    // uniform_axis_1d<Scalar> cria as coordenadas das faces e constrói o objeto
    // BasicAxis1D<Scalar> correspondente. Neste exemplo, Scalar é fvgrid::Real,
    // que atualmente é um alias para double.

    const Axis axis = fvgrid::uniform_axis_1d<Scalar>(nvol, xinit, xfinal);

    // O operador << fornece uma impressão resumida da malha. Ele é útil para
    // inspeção rápida e para conferir se o objeto foi construído corretamente.

    std::cout << axis << "\n";

    // Usamos um formato numérico fixo para tornar a saída estável e fácil de
    // comparar com o resultado impresso no manual.

    std::cout << std::fixed << std::setprecision(6);

    // Imprime um resumo curto da malha gerada.

    std::cout << "Malha cartesiana uniforme 1D\n";
    std::cout << "numero de volumes : " << axis.num_cells() << '\n';
    std::cout << "numero de faces   : " << axis.num_faces() << '\n';
    std::cout << "xmin              : " << axis.xmin() << '\n';
    std::cout << "xmax              : " << axis.xmax() << '\n';
    std::cout << "comprimento       : " << axis.length() << '\n';

    // Imprime todas as coordenadas das faces.
    //
    // Os indices das faces variam de 0 ate nvol. Para nvol = 10, a malha possui
    // 11 faces.

    std::cout << "\nFaces\n";
    std::cout << "i        x_face\n";

    for (fvgrid::Size i = 0; i < axis.num_faces(); ++i) {
        std::cout << std::setw(2) << i << "    "
                  << std::setw(10) << axis.face(i) << '\n';
    }

    // Imprime as informações geométricas associadas a cada volume finito.
    //
    // O indice p identifica o volume finito. Para cada volume, o programa
    // imprime sua face oeste, seu centro, sua face leste e seu comprimento.

    std::cout << "\nVolumes\n";
    std::cout << "p        face_oeste      centro      face_leste          dx\n";

    for (fvgrid::Size p = 0; p < axis.num_cells(); ++p) {
        std::cout << std::setw(2) << p << "    "
                  << std::setw(10) << axis.west_face(p) << "  "
                  << std::setw(10) << axis.center(p) << "  "
                  << std::setw(10) << axis.east_face(p) << "  "
                  << std::setw(10) << axis.cell_length(p) << '\n';
    }

    return 0;
}