// ============================================================================
// Arquivo: tst_Axis1DCSVWriter.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Testa o componente Axis1DCSVWriter no contexto de CSV.
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
#include <filesystem>
#include <sstream>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <FVGridMaker/ErrorHandling/FVGridException.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/Output/CSV/Axis1DCSVWriter.h>

namespace fvgrid {

TEST(Axis1DCSVWriter, WritesExpectedHeaderExactly) {
    const Axis1D axis{std::vector<Real>{0.0, 1.0, 3.0}};

    std::ostringstream output;
    Axis1DCSVWriter::write(axis, output);

    std::istringstream input{output.str()};
    std::string header;

    std::getline(input, header);

    EXPECT_EQ(header, "i,xF,xC,Dx,dx");
}

TEST(Axis1DCSVWriter, WritesLastFaceRowWithEmptyCenterAndCellLength) {
    const Axis1D axis{std::vector<Real>{0.0, 1.0, 3.0}};

    std::ostringstream output;
    Axis1DCSVWriter::write(axis, output);

    std::istringstream input{output.str()};
    std::string line;
    std::string last_line;

    while (std::getline(input, line)) {
        if (!line.empty()) {
            last_line = line;
        }
    }

    EXPECT_EQ(last_line, "2,3,,,1");
}

TEST(Axis1DCSVWriter, WritesAllCellRows) {
    const Axis1D axis{std::vector<Real>{0.0, 1.0, 3.0}};

    std::ostringstream output;
    Axis1DCSVWriter::write(axis, output);

    const std::string text = output.str();

    EXPECT_NE(text.find("0,0,0.5,1,0.5"), std::string::npos);
    EXPECT_NE(text.find("1,1,2,2,1.5"), std::string::npos);
}

TEST(Axis1DCSVWriter, CreatesParentDirectory) {
    const Axis1D axis{std::vector<Real>{0.0, 1.0}};

    const auto directory =
        std::filesystem::temp_directory_path() / "fvgrid_csv_test";
    const auto path = directory / "nested" / "axis.csv";

    std::filesystem::remove_all(directory);

    Axis1DCSVWriter::write(axis, path);

    EXPECT_TRUE(std::filesystem::exists(path));

    std::filesystem::remove_all(directory);
}

TEST(Axis1DCSVWriter, RejectsUnwritableStream) {
    const Axis1D axis{std::vector<Real>{0.0, 1.0}};

    std::ostringstream output;
    output.setstate(std::ios::badbit);

    EXPECT_THROW(
        Axis1DCSVWriter::write(axis, output),
        FVGridException
    );
}

TEST(Axis1DCSVWriter, RejectsDirectoryAsFile) {
    const Axis1D axis{std::vector<Real>{0.0, 1.0}};

    EXPECT_THROW(
        Axis1DCSVWriter::write(axis, std::filesystem::temp_directory_path()),
        FVGridException
    );
}

}  // namespace fvgrid