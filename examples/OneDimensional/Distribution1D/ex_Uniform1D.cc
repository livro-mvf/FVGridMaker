// ============================================================================
// Arquivo: ex_Uniform1D.cc
// Projeto: FVGridMaker
// Versão: consulte <FVGridMaker/Core/Version.h>
// Descrição: Demonstra o uso de Uniform1D com a API pública do FVGridMaker.
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
// SPDX-FileCopyrightText: 2026 FVGridMaker Team
// SPDX-License-Identifier: MIT
//
// Exercicio Computacional 4.4
// Titulo: Malha unidimensional simetrica com centros aleatorios.
//
// Objetivo:
//   Construir uma malha unidimensional simetrica a partir de centros gerados
//   pelo usuario, sem alterar a biblioteca FVGridMaker.
//
// Modelo numerico:
//   Os centros sao sorteados na metade esquerda do dominio, espelhados em
//   torno do ponto medio e usados por Custom1D para reconstruir a geometria
//   da malha. As faces internas obedecem a regra ponderada
//
//       face[i] = 0.25 * center[i - 1] + 0.75 * center[i].
//
// Verificacoes:
//   O programa confirma a simetria dos centros, a regra 25/75 para as faces
//   internas e emite aviso quando algum espacamento fica abaixo da tolerancia.

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/Core/Types.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Custom1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/AxisGeometry1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/CoordinateTags1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Coordinates1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Domain1D.h>

namespace {

struct FacesPonderadasDosCentros1D final {
    using primary_coordinates = fvgrid::CenterCoordinates1D;

    [[nodiscard]] static constexpr std::string_view name() noexcept {
        return "FacesPonderadasDosCentros1D";
    }

    [[nodiscard]] static fvgrid::AxisGeometry1D complete_geometry(
        std::vector<fvgrid::Real> centers,
        fvgrid::Domain1D domain
    ) {
        const fvgrid::Size nvol = centers.size();

        std::vector<fvgrid::Real> faces(nvol + 1);
        faces.front() = domain.xmin();
        faces.back() = domain.xmax();

        for (fvgrid::Size i = 1; i < nvol; ++i) {
            faces[i] = 0.25 * centers[i - 1] + 0.75 * centers[i];
        }

        return fvgrid::AxisGeometry1D{
            .faces = std::move(faces),
            .centers = std::move(centers),
            .pattern_name = std::string{name()},
        };
    }
};

class MalhaSimetrica1D final {
public:
    MalhaSimetrica1D(
        fvgrid::Length comprimento,
        fvgrid::NVol numero_de_volumes,
        fvgrid::XInit x_inicial,
        fvgrid::Seed semente
    )
        : comprimento_{comprimento},
          numero_de_volumes_{numero_de_volumes},
          x_inicial_{x_inicial},
          semente_{semente} {}

    [[nodiscard]] fvgrid::Axis1D gerar() const {
        std::vector<fvgrid::Real> centros = gerar_centros();

        return fvgrid::Custom1D::make(
            fvgrid::Coordinates1D::centers(std::move(centros)),
            FacesPonderadasDosCentros1D{},
            fvgrid::Domain1D::from_length(x_inicial_, comprimento_)
        );
    }

private:
    [[nodiscard]] std::vector<fvgrid::Real> gerar_centros() const {
        const fvgrid::Size n = numero_de_volumes_.value();
        const fvgrid::Real xmin = x_inicial_.value();
        const fvgrid::Real xmax = xmin + comprimento_.value();
        const fvgrid::Real xmeio = 0.5 * (xmin + xmax);

        const fvgrid::Size metade = n / 2;

        std::mt19937_64 gerador{semente_.value()};
        std::uniform_real_distribution<fvgrid::Real> distribuicao{xmin, xmeio};

        std::vector<fvgrid::Real> centros_esquerda(metade);

        for (fvgrid::Size i = 0; i < metade; ++i) {
            centros_esquerda[i] = distribuicao(gerador);
        }

        std::sort(centros_esquerda.begin(), centros_esquerda.end());

        std::vector<fvgrid::Real> centros(n);

        for (fvgrid::Size i = 0; i < metade; ++i) {
            centros[i] = centros_esquerda[i];
        }

        if (n % 2 != 0) {
            centros[metade] = xmeio;
        }

        for (fvgrid::Size i = 0; i < metade; ++i) {
            centros[n - 1 - i] = 2.0 * xmeio - centros_esquerda[i];
        }

        return centros;
    }

    fvgrid::Length comprimento_;
    fvgrid::NVol numero_de_volumes_;
    fvgrid::XInit x_inicial_;
    fvgrid::Seed semente_;
};

[[nodiscard]] bool registrar_teste(
    const std::string& descricao,
    bool passou
) {
    std::cout << (passou ? "[PASSOU] " : "[FALHOU] ")
              << descricao << '\n';
    return passou;
}

[[nodiscard]] bool centros_simetricos(
    const fvgrid::Axis1D& malha,
    fvgrid::Real tolerancia
) {
    const auto centros = malha.centers();
    const fvgrid::Real xmeio = 0.5 * (malha.xmin() + malha.xmax());
    bool passou = true;

    for (fvgrid::Size i = 0; i < centros.size(); ++i) {
        const fvgrid::Size j = centros.size() - 1 - i;

        if (i > j) {
            break;
        }

        const fvgrid::Real residual =
            std::abs((centros[i] + centros[j]) - 2.0 * xmeio);

        if (residual > tolerancia) {
            std::cout << "  centros[" << i << "] + centros[" << j
                      << "] difere de 2*xmeio por "
                      << residual << '\n';
            passou = false;
        }
    }

    return passou;
}

[[nodiscard]] bool faces_ponderadas_corretas(
    const fvgrid::Axis1D& malha,
    fvgrid::Real tolerancia
) {
    const auto faces = malha.faces();
    const auto centros = malha.centers();
    bool passou = true;

    for (fvgrid::Size i = 1; i + 1 < faces.size(); ++i) {
        const fvgrid::Real face_esperada =
            0.25 * centros[i - 1] + 0.75 * centros[i];

        const fvgrid::Real residual = std::abs(faces[i] - face_esperada);

        if (residual > tolerancia) {
            std::cout << "  face[" << i
                      << "] nao obedece a regra 25/75; residuo = "
                      << residual << '\n';
            passou = false;
        }
    }

    return passou;
}

void avisar_espacamentos_pequenos(
    const fvgrid::Axis1D& malha,
    fvgrid::Real tolerancia
) {
    const auto dx_faces = malha.dx_faces();

    for (fvgrid::Size i = 0; i < dx_faces.size(); ++i) {
        if (dx_faces[i] < tolerancia) {
            std::cout << "[AVISO] dx_faces[" << i << "] = "
                      << dx_faces[i]
                      << " e menor que a tolerancia "
                      << tolerancia << '\n';
        }
    }
}

void imprimir_razao_de_espacamentos(const fvgrid::Axis1D& malha) {
    const auto dx = malha.dx_faces();
    const auto [min_it, max_it] = std::minmax_element(dx.begin(), dx.end());

    std::cout << "dx_max/dx_min = " << (*max_it / *min_it) << "\n\n";
}

[[nodiscard]] unsigned verificar_malha(
    const fvgrid::Axis1D& malha,
    fvgrid::Real tolerancia
) {
    unsigned aprovados{};
    unsigned total{};

    std::cout << "\nVerificacoes automaticas\n";
    std::cout << "========================\n";

    ++total;
    aprovados += registrar_teste(
        "centros simetricos em relacao ao ponto medio",
        centros_simetricos(malha, tolerancia)
    );

    ++total;
    aprovados += registrar_teste(
        "faces internas obedecem a regra 25/75",
        faces_ponderadas_corretas(malha, tolerancia)
    );

    std::cout << "\nResumo\n";
    std::cout << "======\n";
    std::cout << "Verificacoes aprovadas: "
              << aprovados << " de " << total << '\n';

    return (aprovados == total) ? 0u : 1u;
}

void imprimir_mensagem_final() {
    std::cout << "\nAplicacoes e recomendacoes\n";
    std::cout << "--------------------------\n";
    std::cout << "1. A classe MalhaSimetrica1D permanece no exercicio; ";
    std::cout << "a biblioteca recebe apenas coordenadas e um padrao valido.\n";
    std::cout << "2. Alterar a semente muda a distribuicao dos centros, ";
    std::cout << "mas deve preservar a simetria imposta pelo algoritmo.\n";
    std::cout << "3. A regra 25/75 ilustra como testar um padrao de faces ";
    std::cout << "definido pelo usuario antes de reutiliza-lo em outros casos.\n";
}

}  // namespace

int main() {
    try {
        const fvgrid::Length comprimento{1.0};
        const fvgrid::NVol numero_de_volumes{9};
        const fvgrid::XInit x_inicial{0.0};
        const fvgrid::Seed semente{1234};

        constexpr fvgrid::Real tolerancia = 1.0e-10;
        constexpr fvgrid::Real tolerancia_espacamento_pequeno = 1.0e-2;

        const MalhaSimetrica1D gerador{
            comprimento,
            numero_de_volumes,
            x_inicial,
            semente
        };

        const fvgrid::Axis1D malha = gerador.gerar();

        std::cout << std::fixed << std::setprecision(6);
        std::cout << "Exercicio Computacional 4.4\n";
        std::cout << "Malha simetrica com centros aleatorios\n";
        std::cout << "======================================\n\n";

        std::cout << malha << '\n';
        imprimir_razao_de_espacamentos(malha);
        avisar_espacamentos_pequenos(
            malha,
            tolerancia_espacamento_pequeno
        );

        const unsigned status = verificar_malha(malha, tolerancia);
        imprimir_mensagem_final();

        return static_cast<int>(status);
    } catch (const std::exception& erro) {
        std::cerr << "Erro: " << erro.what() << '\n';
        return 1;
    }
}