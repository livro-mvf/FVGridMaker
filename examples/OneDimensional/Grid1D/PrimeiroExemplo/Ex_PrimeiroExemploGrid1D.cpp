// ----------------------------------------------------------------------------
// File: Ex_PrimeiroExemploGrid1D.cpp
//
// Primeiro exemplo de uso da FVGridMaker.
//
// Objetivo:
//   Mostrar, da forma mais simples possivel, como criar uma malha 1D uniforme
//   e como consultar as principais informacoes geometricas da malha.
//
// Este exemplo foi escrito para quem ainda nao conhece bem C++ ou a biblioteca.
// Por isso, ele evita atalhos e deixa explicito o que cada linha importante faz.
//
// Como executar pelo CMake, a partir da pasta build:
//
//   make -j2 run_Ex_PrimeiroExemploGrid1D
//
// Por padrao, a tabela mostra apenas a malha fisica. Isto significa que:
//
//   face_x[0] = xInicial
//
// Para imprimir tambem os volumes ficticios (ghost cells), execute o programa
// passando a opcao:
//
//   ./examples/Grid/Grid1D/PrimeiroExemplo/Ex_PrimeiroExemploGrid1D --com-ficticios
//
// Observacao importante:
//   Os vetores de centros e faces nao tem o mesmo tamanho.
//
//   Se existem N volumes:
//     - existem N centros;
//     - existem N + 1 faces.
//
//   Por isso, a ultima linha da tabela pode ter face_x e dcentro_x, mas nao
//   centro_x nem dface_x.
// ----------------------------------------------------------------------------

#include <iostream>

#include <FVGridMaker/Grid/Grid1D/Builders/Grid1DBuilder.hpp>
#include <FVGridMaker/Grid/Grid1D/Patterns/Distribution/Uniform1D.hpp>


int main(int argc, char** argv) {
    // ------------------------------------------------------------------------
    // 1. Apelidos de namespace e tipos
    // ------------------------------------------------------------------------
    //
    // A biblioteca usa namespaces para organizar os modulos. O nome completo
    // de alguns tipos e longo, entao criamos apelidos locais apenas para deixar
    // este exemplo mais legivel.
    //
    // Index: tipo inteiro usado para indices e quantidades de volumes.
    // Real : tipo real usado para coordenadas e distancias.
    //
    namespace grid1d = FVGridMaker::grid::grid1d;
    namespace dist   = FVGridMaker::grid::grid1d::patterns::distribution;

    using Grid1DBuilder = grid1d::builders::Grid1DBuilder;
    using Index         = Grid1DBuilder::Index;
    using Real          = Grid1DBuilder::Real;

    // ------------------------------------------------------------------------
    // 2. Parametros da malha
    // ------------------------------------------------------------------------
    //
    // Vamos construir uma malha 1D no intervalo [0, 1].
    //
    // nVolumesFisicos:
    //   Numero de volumes dentro do dominio fisico.
    //
    // nFicticios:
    //   Numero de volumes ficticios em cada lado do dominio. Estes volumes sao
    //   uteis em metodos numericos para aplicar condicoes de contorno, mas
    //   normalmente o usuario iniciante quer olhar primeiro apenas para a malha
    //   fisica.
    //
    // xInicial e xFinal:
    //   Limites do dominio fisico. No modo padrao de impressao, face_x[0] deve
    //   ser exatamente xInicial.
    //
    const Index nVolumesFisicos = 5;
    const Index nFicticios      = 1;
    const Real  xInicial        = 0.0;
    const Real  xFinal          = 1.0;

    // Por padrao, nao imprimimos os volumes ficticios.
    // O usuario pode mudar isso passando "--com-ficticios" na linha de comando.
    bool imprimirFicticios      = false;

    // ------------------------------------------------------------------------
    // 3. Leitura simples dos argumentos da linha de comando
    // ------------------------------------------------------------------------
    //
    // argc informa quantos argumentos foram passados.
    // argv contem os textos desses argumentos.
    //
    // Exemplo:
    //
    //   programa --com-ficticios
    //
    // Neste caso, argv[1] sera o texto "--com-ficticios".
    //
    for (int i = 1; i < argc; ++i) {
        if (std::string_view{argv[i]} == "--com-ficticios") {
            imprimirFicticios = true;
        }
    }

    // ------------------------------------------------------------------------
    // 4. Construcao da malha
    // ------------------------------------------------------------------------
    //
    // A FVGridMaker usa um "builder" para criar a malha. O builder permite
    // configurar a malha passo a passo:
    //
    //   setN(...)
    //     Define quantos volumes fisicos serao usados.
    //
    //   setDomain(...)
    //     Define o intervalo fisico [xInicial, xFinal].
    //
    //   setNFicticios(...)
    //     Define quantos volumes ficticios existem em cada lado.
    //
    //   setDistribution<dist::Uniform1D>()
    //     Define que a distribuicao das faces sera uniforme.
    //
    //   build()
    //     Cria o objeto Grid1D pronto para ser consultado.
    //
    const auto malha =
        Grid1DBuilder{}
            .setN(nVolumesFisicos)
            .setDomain(xInicial, xFinal)
            .setNFicticios(nFicticios)
            .setDistribution<dist::Uniform1D>()
            .build();

    // ------------------------------------------------------------------------
    // 5. Informacoes gerais
    // ------------------------------------------------------------------------
    //
    // Aqui imprimimos um resumo antes da tabela para o usuario conferir se a
    // malha foi criada com os parametros esperados.
    //
    std::cout << "FVGridMaker - primeiro exemplo 1D\n";
    std::cout << "Dominio fisico: [" << xInicial << ", " << xFinal << "]\n";
    std::cout << "Volumes fisicos: " << malha.nVolumesFisicos() << '\n';
    std::cout << "Volumes ficticios por lado: " << malha.nVolumesFicticios() << '\n';
    std::cout << "Volumes armazenados: " << malha.nVolumesTotais() << "\n\n";

    // ------------------------------------------------------------------------
    // 6. Legenda da tabela
    // ------------------------------------------------------------------------
    //
    // i:
    //   Indice usado apenas para imprimir a tabela.
    //
    // centro_x:
    //   Coordenada x do centro do volume.
    //
    // face_x:
    //   Coordenada x da face.
    //
    // dcentro_x:
    //   Distancia associada aos centros:
    //     - na primeira linha, centro_x[0] - face_x[0];
    //     - no interior, centro_x[i] - centro_x[i - 1];
    //     - na ultima linha, face_x[N] - centro_x[N - 1].
    //
    // dface_x:
    //   Largura do volume, isto e, face_x[i + 1] - face_x[i].
    //
    // Repare que centro_x e dface_x tem N valores, enquanto face_x e dcentro_x
    // tem N + 1 valores.
    //
    std::cout << std::fixed << std::setprecision(6);
    std::cout << std::setw(4) << "i"
              << std::setw(14) << "centro_x"
              << std::setw(14) << "face_x"
              << std::setw(14) << "dcentro_x"
              << std::setw(14) << "dface_x" << '\n';

    if (!imprimirFicticios) {
        // --------------------------------------------------------------------
        // 7A. Impressao padrao: somente a malha fisica
        // --------------------------------------------------------------------
        //
        // Este e o modo recomendado para o primeiro contato com a biblioteca.
        //
        // Usamos os metodos com sufixo "Fisico" para ignorar os volumes
        // ficticios. Assim:
        //
        //   faceFisica(0) == xInicial
        //   faceFisica(N) == xFinal
        //
        // O loop vai ate i == N porque existem N + 1 faces para N volumes.
        // Na ultima linha nao ha centro_x nem dface_x, pois estes vetores tem
        // apenas N entradas.
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
        // 7B. Impressao completa: inclui volumes ficticios
        // --------------------------------------------------------------------
        //
        // Este modo e util para depurar a geometria completa armazenada pela
        // malha. Aqui usamos os vetores internos completos:
        //
        //   centers()
        //   faces()
        //   deltasCenters()
        //   deltasFaces()
        //
        // Como estes vetores nao tem todos o mesmo tamanho, o loop vai ate o
        // maior tamanho encontrado. Quando um vetor nao possui o indice i, a
        // coluna correspondente fica em branco.
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
