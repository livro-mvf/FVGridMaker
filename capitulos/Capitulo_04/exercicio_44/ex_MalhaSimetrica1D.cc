// SPDX-FileCopyrightText: 2026 FVGridMaker Team
// SPDX-License-Identifier: MIT
//
// Exercicio Computacional 4.4
// Titulo: Malha simetrica definida pelo usuario.
//
// Objetivo:
//   Criar uma malha unidimensional simetrica fora da FVGridMakerLib e usar
//   Custom1D para construir a Axis1D final validada pela biblioteca.
//
// Modelo numerico:
//   Centros aleatorios sao sorteados na metade esquerda do dominio e
//   espelhados na metade direita. As faces internas sao reconstruidas pela
//   regra ponderada 25/75 entre centros vizinhos.
//
// Verificacoes:
//   O programa confirma a simetria dos centros e a regra das faces ponderadas,
//   imprime a razao dx_max/dx_min e resume as verificacoes automaticas.

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
#include <FVGridMaker/OneDimensional/GridPattern1D/CoordinateKind1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Coordinates1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Domain1D.h>

namespace {

struct FacesPonderadasDosCentros1D final {
    [[nodiscard]] static constexpr std::string_view name() noexcept {
        return "FacesPonderadasDosCentros1D";
    }

    [[nodiscard]] static constexpr fvgrid::CoordinateKind1D input_kind()
        noexcept {
        return fvgrid::CoordinateKind1D::Centers;
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

        const fvgrid::Real residuo =
            std::abs((centros[i] + centros[j]) - 2.0 * xmeio);

        if (residuo > tolerancia) {
            std::cout << "  centros[" << i << "] + centros[" << j
                      << "] difere de 2*xmeio por "
                      << residuo << '\n';
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

        const fvgrid::Real residuo = std::abs(faces[i] - face_esperada);

        if (residuo > tolerancia) {
            std::cout << "  face[" << i
                      << "] nao obedece a regra 25/75; residuo = "
                      << residuo << '\n';
            passou = false;
        }
    }

    return passou;
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

void imprimir_mensagem_final()
{
    std::cout << "\nAplicacoes e recomendacoes\n";
    std::cout << "--------------------------\n";
    std::cout << "1. A construcao especializada fica no exercicio, ";
    std::cout << "mantendo a biblioteca focada em componentes reutilizaveis.\n";
    std::cout << "2. A simetria dos centros pode ser usada para testar ";
    std::cout << "malhas nao uniformes com propriedades geometricas impostas.\n";
    std::cout << "3. A razao dx_max/dx_min ajuda a avaliar o grau de ";
    std::cout << "nao uniformidade antes de resolver uma equacao numerica.\n";
}

} // namespace

int main()
{
    try {
        const fvgrid::Length comprimento{1.0};
        const fvgrid::NVol numero_de_volumes{21};
        const fvgrid::XInit x_inicial{0.0};
        const fvgrid::Seed semente{2026};

        constexpr fvgrid::Real tolerancia = 1.0e-12;

        const MalhaSimetrica1D gerador{
            comprimento,
            numero_de_volumes,
            x_inicial,
            semente
        };

        const fvgrid::Axis1D malha = gerador.gerar();

        std::cout << std::fixed << std::setprecision(8);
        std::cout << "Exercicio Computacional 4.4\n";
        std::cout << "Malha simetrica definida pelo usuario\n";
        std::cout << "=====================================\n\n";

        std::cout << malha << '\n';
        imprimir_razao_de_espacamentos(malha);

        const unsigned status = verificar_malha(malha, tolerancia);
        imprimir_mensagem_final();

        return static_cast<int>(status);
    } catch (const std::exception& erro) {
        std::cerr << "Erro: " << erro.what() << '\n';
        return 1;
    }
}
