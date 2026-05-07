// ----------------------------------------------------------------------------
// File: Ex_UniformGrid1D.cpp
// Author: FVMGridMaker Team
// Version: 2.3
// Date: 2025-10-26
// Description: Criação de uma malha uniforme
// License: GNU GPL v3
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// includes FVMGridMaker 
// ----------------------------------------------------------------------------
#include <FVGridMaker/Grid/Grid1D/Builders/Grid1DBuilder.hpp>
#include <FVGridMaker/Grid/Grid1D/Patterns/Distribution/Uniform1D.hpp>
#include <FVGridMaker/Grid/Grid1D/Grid1D.h>

// ----------------------------------------------------------------------------
// includes C++ 
// ----------------------------------------------------------------------------
#include <iomanip>
#include <iostream>

// ----------------------------------------------------------------------------
// Definição dos using 
// ----------------------------------------------------------------------------

using FVGridMaker::core::Index;
using FVGridMaker::core::Real;
using FVGridMaker::grid::CenteringTag;
using FVGridMaker::grid::DistributionTag;
using FVGridMaker::grid::grid1d::Grid1D;
using FVGridMaker::grid::grid1d::builders::Grid1DBuilder;
using FVGridMaker::grid::grid1d::patterns::distribution::Uniform1D;

int main() {


// ------------------------------------------------------------------------
// 1. Dados básicos
// ------------------------------------------------------------------------

const Index N      = 20;   // número de volumes na malha
const Real  A      = 0.0;  // coordenada inicial do domínio
const Real  B      = 1.0;  // coordenada final do domínio
const Index NGhost = 0;    // número de volumes fictícios (por lado)

// ------------------------------------------------------------------------
// 2. Construção da malha
// ------------------------------------------------------------------------

    Grid1D grid =   Grid1DBuilder{}
                    .setN(N)
                    .setDomain(A, B)
                    .setNGhost(NGhost)
                    .setDistribution<Uniform1D>()
                    .build();

// ------------------------------------------------------------------------
// 3. Impressão das coordenadas e distâncias da malha
// ------------------------------------------------------------------------

auto coutFlags = std::cout.flags();

    std::cout << "---- Informações sobre a malha 1D (uniforme) -----\n";
    std::cout << "Número de volumes                           : " << N << '\n';
    if(NGhost > 0) std::cout << "Número de volumes fictícios                 : " << NGhost << '\n';

    std::cout << std::setprecision(4) << std::fixed;

    std::cout << "Coordenadas inicial e final do domínio      : [" << A << ", " << B << "]\n";
    std::cout << "Comprimento do domínio                      : " << (B - A) << "\n\n";

    std::cout.flags(coutFlags);


    std::cout << grid << '\n';

    std::cout << "\n\nMalha1D uniforme foi criada corretamente.\n\n";

// ------------------------------------------------------------------------
// 4. Acesso as coordenadas e distâncias da malha
// ------------------------------------------------------------------------

auto    Delta_x  = grid.deltasFaces();      // Vetor com as distancias entre faces consecutivas
auto    delta_x  = grid.deltasCenters();    // Vetor com as distancias entre centros consecutivos
auto    xc       = grid.centers();          // Vetor com as coordenadas dos centros dos volumes
auto    xf       = grid.faces();            // Vetir com as coordenadas das faces dos volumes  
    return 0;
}
