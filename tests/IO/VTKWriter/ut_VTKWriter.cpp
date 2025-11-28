// ----------------------------------------------------------------------------
// File: ut_VTKWriter.cpp
// Project: FVGridMaker
// Description: Teste de integração para a classe VTKWriter.
//              Verifica a geração de arquivos VTK a partir de uma malha.
// ----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <fstream>
#include <cstdio> // Para std::remove

// Includes necessários da biblioteca
// Certifique-se de que os caminhos correspondem à estrutura do seu projeto
#include <FVGridMaker/Grid/Mesh.hpp>
#include <FVGridMaker/Grid/Geometry/Cartesian.hpp>
#include <FVGridMaker/Grid/Discretization/CellCentered.hpp>
#include <FVGridMaker/IO/VTKWriter.hpp>

// Facilitadores de Namespace
namespace FVG = FVGridMaker;

// Definição da malha concreta para o teste
// Mesh está no namespace FVGridMaker::core (conforme Mesh.hpp enviado)
using TestMeshType = FVG::core::Mesh<
    FVG::grid::geometry::Cartesian, 
    FVG::grid::discretization::CellCentered
>;

TEST(VTKWriterTest, ExportSimpleCartesianMesh) {
    // 1. Configuração da Malha (10x10)
    FVG::core::Size ni = 10;
    FVG::core::Size nj = 10;
    
    // Instanciação (CORREÇÃO: Usando TestMeshType consistentemente)
    TestMeshType mesh(ni, nj);
    
    // 2. Cálculo da Geometria (Preenche o Storage com dados para exportar)
    mesh.computeGeometry();
    
    // 3. Exportação
    std::string filename = "test_cartesian_10x10.vtk";
    
    // Chama o método estático de escrita
    FVG::io::VTKWriter::write(mesh, filename);
    
    // 4. Validação (Caixa Preta)
    // Verifica se o arquivo foi criado e pode ser lido
    std::ifstream file(filename);
    ASSERT_TRUE(file.is_open()) << "Falha ao abrir o ficheiro gerado: " << filename;
    
    // Verifica o cabeçalho mágico do VTK para garantir que é um ficheiro válido
    std::string header;
    if (std::getline(file, header)) {
        // O header padrão escrito pelo VTKWriter é "# vtk DataFile Version 3.0"
        EXPECT_EQ(header, "# vtk DataFile Version 3.0");
    } else {
        FAIL() << "Ficheiro gerado está vazio.";
    }
    
    // Verifica se declarou o tipo de dataset correto
    std::string line;
    bool found_structured_grid = false;
    while(std::getline(file, line)) {
        if (line.find("DATASET STRUCTURED_GRID") != std::string::npos) {
            found_structured_grid = true;
            break;
        }
    }
    EXPECT_TRUE(found_structured_grid) << "Ficheiro não contém a tag DATASET STRUCTURED_GRID";
    
    file.close();
    
    // Limpeza (opcional): Remove o arquivo gerado após o teste
    // Comente esta linha se quiser inspecionar o arquivo no ParaView/VisIt
    // std::remove(filename.c_str());
}