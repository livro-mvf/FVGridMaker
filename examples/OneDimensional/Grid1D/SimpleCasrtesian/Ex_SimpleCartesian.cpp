// ----------------------------------------------------------------------------
// File: Ex01_SimpleCartesian.cpp
// Description: Exemplo de uso da biblioteca FVGridMaker.
//              Gera uma malha cartesiana, calcula um campo escalar e exporta.
// ----------------------------------------------------------------------------

#include <iostream>
#include <vector>
#include <cmath>

// Includes da Biblioteca
#include <FVGridMaker/Grid/Mesh.hpp>
#include <FVGridMaker/Grid/Geometry/Cartesian.hpp>
#include <FVGridMaker/Grid/Discretization/CellCentered.hpp>
#include <FVGridMaker/Grid/Iterators/GridIterators.hpp>
#include <FVGridMaker/IO/VTKWriter.hpp>

// Namespaces para facilitar
namespace FVG = FVGridMaker;
using namespace FVG::grid;

// Define o tipo da malha
using MyMesh = FVG::core::Mesh<
    geometry::Cartesian, 
    discretization::CellCentered
>;

int main() {
    std::cout << "=== FVGridMaker: Exemplo 01 - Malha Cartesiana ===\n";

    // 1. Configuração
    FVG::core::Size ni = 20;
    FVG::core::Size nj = 20;
    
    std::cout << "Gerando malha " << ni << "x" << nj << "...\n";

    // 2. Instanciação
    MyMesh mesh(ni, nj);

    // 3. Cálculo da Geometria (Preenche coordenadas, volumes, etc.)
    mesh.computeGeometry();

    // 4. (Opcional) Criar um campo de dados para visualização
    // Vamos simular uma distribuição de Temperatura: T = x^2 + y^2
    std::cout << "Calculando campo escalar (Temperatura)...\n";
    
    // Como a biblioteca foca na geometria, campos de física geralmente
    // ficam em vetores externos no solver. Vamos criar um aqui.
    std::vector<double> temperature(mesh.size(), 0.0);

    // Usamos os iteradores para percorrer apenas as células reais
    for (auto idx : iterators::InternalCells(mesh)) {
        double x = mesh.centers_x[idx];
        double y = mesh.centers_y[idx];
        
        temperature[idx] = x*x + y*y;
    }

    // 5. Exportação
    std::string filename = "ex01_cartesian.vtk";
    std::cout << "Exportando para " << filename << "...\n";
    
    // O VTKWriter padrão exporta geometria + volumes.
    // Para exportar o campo de temperatura, precisaríamos de uma função
    // mais genérica no VTKWriter ou adicionar manualmente (fora do escopo deste exemplo simples).
    // Por enquanto, exportamos a geometria base para validação.
    FVG::io::VTKWriter::write(mesh, filename);

    std::cout << "Concluido com sucesso!\n";
    return 0;
}