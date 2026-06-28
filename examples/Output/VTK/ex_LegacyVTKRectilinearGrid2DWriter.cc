// ============================================================================
// Arquivo: ex_LegacyVTKRectilinearGrid2DWriter.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Demonstra o uso de LegacyVTKRectilinearGrid2DWriter com a API pública do FVGridMaker.
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
//
// This example exports a StructuredGrid2D as a legacy ASCII VTK file.
//
// The exported VTK dataset is:
//
//   DATASET RECTILINEAR_GRID
//
// The grid coordinates are written from the face coordinates:
//
//   X_COORDINATES = x_axis.faces()
//   Y_COORDINATES = y_axis.faces()
//   Z_COORDINATES = {0.0}
//
// The example also writes one cell-data scalar:
//
//   cell_area
//
// The file can be opened directly in ParaView.
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <filesystem>
#include <iostream>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>
#include <FVGridMaker/Output/VTK/LegacyVTKRectilinearGrid2DWriter.h>
#include <FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h>

int main() {
    const fvgrid::Axis1D x_axis = fvgrid::Uniform1D::make(
        fvgrid::NVol{8},
        fvgrid::Length{4.0},
        fvgrid::XInit{0.0}
    );

    const fvgrid::Axis1D y_axis = fvgrid::Uniform1D::make(
        fvgrid::NVol{5},
        fvgrid::Length{2.5},
        fvgrid::XInit{-1.0}
    );

    const fvgrid::StructuredGrid2D grid{x_axis, y_axis};

    const std::filesystem::path output_path =
        std::filesystem::absolute(
            std::filesystem::current_path() /
            "examples_output" /
            "structured_grid_2d.vtk"
        );

    std::cout << "Current execution directory:\n"
              << "  " << std::filesystem::current_path() << '\n';

    std::cout << "Output file will be written to:\n"
              << "  " << output_path << '\n';

    fvgrid::LegacyVTKRectilinearGrid2DWriter::write(
        grid,
        output_path
    );

    if (!std::filesystem::exists(output_path)) {
        std::cerr << "ERROR: output file was not created.\n";
        std::cerr << "Expected output file:\n"
                  << "  " << output_path << '\n';
        return 1;
    }

    std::cout << "Output file created successfully:\n"
              << "  " << output_path << '\n';

    std::cout << "Output file size:\n"
              << "  " << std::filesystem::file_size(output_path)
              << " bytes\n";

    std::cout << "Open this file in ParaView as a RECTILINEAR_GRID dataset.\n";

    return 0;
}