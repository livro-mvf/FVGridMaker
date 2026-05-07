// ----------------------------------------------------------------------------
// File: Ex_RandomGrid1D.cpp
//
// Exemplo de malha 1D aleatoria usando a FVGridMaker.
//
// Este exemplo e uma continuacao natural do primeiro exemplo uniforme. A ideia
// aqui e mostrar como construir uma malha nao uniforme, mas ainda controlada e
// reprodutivel.
//
// A malha aleatoria da FVGridMaker parte de uma malha uniforme de referencia.
// Depois, cada coordenada primaria e perturbada por uma fração do espacamento
// uniforme:
//
//   dx_uniforme = (xFinal - xInicial) / nVolumesFisicos
//
// Neste arquivo usamos:
//
//   percentual = 0.20
//
// isto e, uma perturbacao maxima de 20% de dx_uniforme.
//
// Importante:
//   Se um sorteio gerar pontos muito proximos, a biblioteca descarta essa
//   tentativa e sorteia novamente. Isso evita aceitar volumes degenerados sem
//   transformar uma amostra aleatoria ruim em erro do usuario.
//
// Regras gerais de construcao:
//   - CellCentered  (volume centrado): gera faces; calcula centros.
//   - FaceCentered  (face centrada)  : gera centros; calcula faces.
//
// Opcoes:
//   --volume-centrada  usa CellCentered (padrao deste exemplo)
//   --face-centrada    usa FaceCentered
//   --com-ficticios    imprime tambem os volumes ficticios
//   --seed-variado     gera uma malha diferente a cada execucao
//
// Sem --seed-variado, o seed fixo torna a malha reprodutivel. Isso e melhor
// para exemplos, testes e depuracao.
//
// Como executar a partir da pasta build:
//
//   cmake --build . --target run_Ex_RandomGrid1D --parallel 2
//
// Exemplos de execucao direta do binario gerado:
//
//   ./examples/Grid/Grid1D/RandomGrid1D/Ex_RandomGrid1D
//   ./examples/Grid/Grid1D/RandomGrid1D/Ex_RandomGrid1D --face-centrada
//   ./examples/Grid/Grid1D/RandomGrid1D/Ex_RandomGrid1D --com-ficticios
//   ./examples/Grid/Grid1D/RandomGrid1D/Ex_RandomGrid1D --seed-variado
// ----------------------------------------------------------------------------

#include <FVGridMaker/Grid/Common/Tags1D.hpp> // CenteringTag.
#include <FVGridMaker/Grid/Grid1D/Builders/Grid1DBuilder.hpp> // Grid1DBuilder.
#include <FVGridMaker/Grid/Grid1D/Patterns/Distribution/Random1D.hpp> // Random1D.
#include <FVGridMaker/Grid/Grid1D/Utils/Grid1DStats.hpp> // Grid1DStats.

#include <algorithm> // std::max.
#include <cstddef> // std::size_t.
#include <iomanip> // std::setw, std::setprecision.
#include <iostream> // std::cout.
#include <string_view> // std::string_view.

int main(int argc, char** argv) {
    // ------------------------------------------------------------------------
    // 1. Apelidos de namespace e tipos
    // ------------------------------------------------------------------------
    //
    // Estes apelidos reduzem o tamanho dos nomes no restante do arquivo.
    //
    namespace grid1d = FVGridMaker::grid::grid1d;
    namespace dist   = FVGridMaker::grid::grid1d::patterns::distribution;

    using FVGridMaker::grid::CenteringTag;
    using Grid1DBuilder = grid1d::builders::Grid1DBuilder;
    using Index         = Grid1DBuilder::Index;
    using Real          = Grid1DBuilder::Real;
    using Grid1DStats   = grid1d::utils::Grid1DStats;

    // ------------------------------------------------------------------------
    // 2. Parametros principais da malha
    // ------------------------------------------------------------------------
    //
    // nVolumesFisicos:
    //   Numero de volumes dentro do dominio fisico.
    //
    // xInicial e xFinal:
    //   Primeira e ultima face fisica da malha.
    //
    // percentual:
    //   Fração do espacamento uniforme usada como limite da perturbacao.
    //
    // seedFixo:
    //   Garante que a mesma malha seja gerada em execucoes repetidas.
    //
    const Index nVolumesFisicos = 10;
    const Real  xInicial        = -10.0;
    const Real  xFinal          = 10.0;
    const Real  percentual      = 0.8;
    const auto  seedFixo        = 12345u;

    // ------------------------------------------------------------------------
    // 3. Opcoes alteradas pela linha de comando
    // ------------------------------------------------------------------------
    //
    // O exemplo comeca com uma configuracao simples:
    //   - sem volumes ficticios;
    //   - seed fixo;
    //   - malha volume-centrada.
    //
    Index nFicticios = 0;
    bool imprimirFicticios = false;
    bool seedVariado = true;
    CenteringTag centering = CenteringTag::CellCentered;

    // ------------------------------------------------------------------------
    // 4. Leitura simples dos argumentos
    // ------------------------------------------------------------------------
    //
    // Este parser e propositalmente pequeno. Ele reconhece apenas as opcoes do
    // cabecalho do arquivo e ignora argumentos desconhecidos.
    //
    for (int i = 1; i < argc; ++i) {
        const std::string_view arg{argv[i]};

        if (arg == "--face-centrada") {
            centering = CenteringTag::FaceCentered;
        } else if (arg == "--volume-centrada") {
            centering = CenteringTag::CellCentered;
        } else if (arg == "--com-ficticios") {
            nFicticios = 2;
            imprimirFicticios = true;
        } else if (arg == "--seed-variado") {
            seedVariado = true;
        }
    }

    // ------------------------------------------------------------------------
    // 5. Escolha do seed
    // ------------------------------------------------------------------------
    //
    // Seed fixo:
    //   a mesma entrada gera sempre a mesma malha.
    //
    // Seed variado:
    //   cada execucao gera uma malha diferente. A geracao do seed fica dentro
    //   da distribuicao Random1D, nao no programa de exemplo.
    //

    // ------------------------------------------------------------------------
    // 6. Construcao da malha
    // ------------------------------------------------------------------------
    //
    // setRandomPercentual(percentual):
    //   controla a intensidade da perturbacao aleatoria.
    //
    // setRandomSeed(seedFixo):
    //   controla a sequencia pseudoaleatoria usada por Random1D quando o modo
    //   de seed fixo esta ativo.
    //
    // A regra de CellCentered/FaceCentered e resolvida dentro do builder:
    //   - CellCentered: Random1D gera faces;
    //   - FaceCentered: Random1D gera centros.
    //
    Grid1DBuilder builder = Grid1DBuilder{}
        .setN(nVolumesFisicos)
        .setDomain(xInicial, xFinal)
        .setNVolumesFicticios(nFicticios)
        .setCentering(centering)
        .setDistribution<dist::Random1D>()
        .setRandomPercentual(percentual)
        .setRandomSeed(seedFixo);

    if (seedVariado) {
        builder.setRandomSeedVariado();
    }

    const auto malha = builder.build();
    const auto stats = Grid1DStats::basicPhysicalFaces(malha);
    const auto smooth = Grid1DStats::smoothnessPhysicalFaces(malha);
    const auto uniformidade =
        Grid1DStats::relativeUniformityPhysicalFaces(malha);

    // ------------------------------------------------------------------------
    // 7. Resumo da configuracao
    // ------------------------------------------------------------------------
    //
    std::cout << "FVGridMaker - exemplo Random1D\n";
    std::cout << "Dominio fisico: [" << xInicial << ", " << xFinal << "]\n";
    std::cout << "Volumes fisicos: " << malha.nVolumesFisicos() << '\n';
    std::cout << "Volumes ficticios por lado: " << malha.nVolumesFicticios() << '\n';
    std::cout << "Centralizacao: " << malha.nomePadraoArmazenamento() << '\n';
    std::cout << "Percentual aleatorio: " << percentual * Real(100) << "%\n";
    std::cout << "Seed: "
              << (seedVariado ? "variado" : "fixo")
              << (seedVariado ? "" : " (12345)") << "\n\n";

    // ------------------------------------------------------------------------
    // 8. Estatisticas simples da malha fisica
    // ------------------------------------------------------------------------
    //
    // As estatisticas abaixo usam apenas os volumes fisicos. Se a opcao
    // --com-ficticios estiver ativa, os volumes ficticios aparecem na tabela
    // completa, mas nao entram neste resumo de qualidade.
    //
    std::cout << "Estatisticas de dface_x fisico\n";
    std::cout << "min      : " << stats.min << '\n';
    std::cout << "max      : " << stats.max << '\n';
    std::cout << "mean     : " << stats.mean << '\n';
    std::cout << "aspect   : " << stats.aspect << '\n';
    std::cout << "cv       : " << stats.cv << '\n';
    std::cout << "uniform. : " << uniformidade << '\n';
    std::cout << "smoothMax: " << smooth.max_grad << "\n\n";

    // ------------------------------------------------------------------------
    // 9. Legenda da tabela
    // ------------------------------------------------------------------------
    //
    // centro_x:
    //   Coordenada do centro do volume.
    //
    // face_x:
    //   Coordenada da face.
    //
    // dcentro_x:
    //   Distancia associada aos centros.
    //
    // dface_x:
    //   Distancia entre faces consecutivas, isto e, largura do volume.
    //
    // Para N volumes:
    //   centro_x e dface_x tem N valores;
    //   face_x e dcentro_x tem N + 1 valores.
    //
    std::cout << std::fixed << std::setprecision(6);
    std::cout << std::setw(4) << "i"
              << std::setw(14) << "centro_x"
              << std::setw(14) << "face_x"
              << std::setw(14) << "dcentro_x"
              << std::setw(14) << "dface_x" << '\n';

    if (!imprimirFicticios) {
        // --------------------------------------------------------------------
        // 10A. Impressao padrao: somente grandezas fisicas
        // --------------------------------------------------------------------
        //
        // Este modo e o mais indicado para entender a malha principal. Mesmo
        // que a malha tenha volumes ficticios, eles nao aparecem aqui.
        //
        for (Index i = 0; i <= malha.nVolumesFisicos(); ++i) {
            std::cout << std::setw(4) << i;

            if (i < malha.nVolumesFisicos()) {
                std::cout << std::setw(14) << malha.centroFisico(i);
            } else {
                std::cout << std::setw(14) << "";
            }

            std::cout << std::setw(14) << malha.faceFisica(i);
            std::cout << std::setw(14) << malha.deltaCentroFisico(i);

            if (i < malha.nVolumesFisicos()) {
                std::cout << std::setw(14) << malha.deltaFaceFisica(i);
            } else {
                std::cout << std::setw(14) << "";
            }

            std::cout << '\n';
        }
    } else {
        // --------------------------------------------------------------------
        // 10B. Impressao completa: inclui volumes ficticios
        // --------------------------------------------------------------------
        //
        // Aqui imprimimos os vetores completos armazenados pela malha. Como
        // eles tem tamanhos diferentes, percorremos ate o maior tamanho e
        // deixamos uma coluna em branco quando o vetor nao possui aquele indice.
        //
        const auto xCentro  = malha.centers();
        const auto xFace    = malha.faces();
        const auto dxCentro = malha.deltasCenters();
        const auto dxFace   = malha.deltasFaces();
        const auto nLinhas  = std::max({xCentro.size(),
                                        xFace.size(),
                                        dxCentro.size(),
                                        dxFace.size()});

        for (std::size_t i = 0; i < nLinhas; ++i) {
            std::cout << std::setw(4) << i;

            if (i < xCentro.size()) {
                std::cout << std::setw(14) << xCentro[i];
            } else {
                std::cout << std::setw(14) << "";
            }

            if (i < xFace.size()) {
                std::cout << std::setw(14) << xFace[i];
            } else {
                std::cout << std::setw(14) << "";
            }

            if (i < dxCentro.size()) {
                std::cout << std::setw(14) << dxCentro[i];
            } else {
                std::cout << std::setw(14) << "";
            }

            if (i < dxFace.size()) {
                std::cout << std::setw(14) << dxFace[i];
            } else {
                std::cout << std::setw(14) << "";
            }

            std::cout << '\n';
        }
    }

    return 0;
}
