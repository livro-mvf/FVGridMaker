//==============================================================================
// Exercício Computacional 3.1
// Gerador de malhas aleatórias unidimensionais
//
// O programa gera uma malha não uniforme em uma dimensão.
//
// Dados do problema:
// xI, coordenada inicial do domínio.
// L, comprimento do domínio, com L > 0.
// N, número total de nós, com N > 2.
//
// A malha gerada contém o primeiro nó fixo em xI, o último nó fixo em xI + L,
// e N - 2 nós internos sorteados no intervalo aberto (xI, xI + L).
//
// O código é autocontido, não usa classes e não depende da biblioteca em
// desenvolvimento.
//==============================================================================

//==============================================================================
// Bibliotecas padrão do C++
//==============================================================================
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>
#include <span>
#include <vector>

//==============================================================================
// Tipo real usado no programa
//==============================================================================

using Real = double;

//==============================================================================
// Declarações das funções
//==============================================================================

// Imprime os dados iniciais da execução.
//
// Esta função mostra os parâmetros usados para construir a malha antes que os
// nós sejam gerados. Isso facilita conferir se a execução corresponde ao caso
// desejado.
//
// Parâmetros:
// xI, coordenada inicial do domínio.
// comprimento, comprimento L do domínio.
// numero_nos, número total de nós da malha.
// semente, semente usada no gerador pseudoaleatório.
inline void imprimir_dados_iniciais(
    Real xI,
    Real comprimento,
    std::size_t numero_nos,
    std::uint64_t semente
);

// Verifica se os dados básicos usados para construir a malha são válidos.
//
// Esta função não gera a malha. Ela apenas protege o programa contra entradas
// inconsistentes, como comprimento negativo, comprimento nulo, coordenadas não
// finitas ou número insuficiente de nós.
//
// Parâmetros:
// xI, coordenada inicial do domínio.
// comprimento, comprimento L do domínio.
// numero_nos, número total de nós da malha.
//
// Lança:
// std::invalid_argument se algum dado violar as condições do exercício.
inline void validar_dados_da_malha(
    Real xI,
    Real comprimento,
    std::size_t numero_nos
);

// Verifica se os nós da malha estão em ordem estritamente crescente.
//
// Esta função é usada depois da geração e ordenação dos nós. Ela confirma se a
// sequência satisfaz x[i - 1] < x[i] para todos os nós. Essa condição evita
// espaçamentos nulos ou negativos.
//
// Parâmetros:
// x, vetor, ou visão, com as coordenadas dos nós.
//
// Retorna:
// true se a malha estiver estritamente crescente.
// false caso contrário.
[[nodiscard]] inline bool malha_estritamente_crescente(
    std::span<const Real> x
);

// Gera uma malha aleatória unidimensional.
//
// A função recebe os dados do domínio, sorteia N - 2 nós internos no intervalo
// aberto (xI, xI + L), ordena esses nós e acrescenta os dois extremos fixos.
// O resultado é uma malha válida para testes com fórmulas de diferenças finitas
// em malha não uniforme.
//
// Parâmetros:
// xI, coordenada inicial do domínio.
// comprimento, comprimento L do domínio.
// numero_nos, número total de nós da malha.
// semente, semente do gerador pseudoaleatório.
//
// Retorna:
// vetor com as coordenadas ordenadas dos nós da malha.
//
// Lança:
// std::invalid_argument se os dados de entrada forem inválidos.
// std::runtime_error se a malha não puder ser gerada de forma consistente.
[[nodiscard]] inline std::vector<Real> gerar_malha_aleatoria(
    Real xI,
    Real comprimento,
    std::size_t numero_nos,
    std::uint64_t semente
);

// Calcula os espaçamentos entre nós consecutivos.
//
// Para uma malha com N nós, esta função calcula N - 1 espaçamentos:
//
// dx[i] = x[i + 1] - x[i]
//
// Esses valores são os passos locais da malha não uniforme e são necessários
// para aplicar as fórmulas de diferenças finitas em espaçamentos variáveis.
//
// Parâmetros:
// x, vetor, ou visão, com as coordenadas dos nós.
//
// Retorna:
// vetor com os espaçamentos entre nós consecutivos.
//
// Lança:
// std::invalid_argument se houver menos de dois nós.
[[nodiscard]] inline std::vector<Real> calcular_espacamentos(
    std::span<const Real> x
);

// Imprime a malha gerada.
//
// Para cada nó, a função imprime o índice i, a coordenada x_i, o espaçamento à
// esquerda, quando existir, e o espaçamento à direita, quando existir.
//
// Esta saída permite verificar visualmente se os nós foram ordenados e se os
// espaçamentos são de fato irregulares.
//
// Parâmetros:
// x, vetor, ou visão, com as coordenadas dos nós.
inline void imprimir_malha(
    std::span<const Real> x
);

// Imprime estatísticas simples dos espaçamentos da malha.
//
// A função calcula e imprime o menor espaçamento, o maior espaçamento, o
// espaçamento médio e a razão entre maior e menor espaçamento.
//
// Essas estatísticas não são obrigatórias no enunciado, mas ajudam a mostrar
// numericamente o grau de irregularidade da malha aleatória.
//
// Parâmetros:
// dx, vetor, ou visão, com os espaçamentos entre nós consecutivos.
//
// Lança:
// std::invalid_argument se o vetor de espaçamentos estiver vazio.
inline void imprimir_resumo_dos_espacamentos(
    std::span<const Real> dx
);

//==============================================================================
// Programa principal
//==============================================================================

int main()
{
    try {
        //======================================================================
        // Dados do problema
        //======================================================================

        // Coordenada inicial do domínio.
        const Real xI = -1.0;

        // Comprimento do domínio.
        const Real comprimento = 3.0;

        // Número total de nós da malha.
        const std::size_t numero_nos = 10;

        // Semente do gerador pseudoaleatório.
        //
        // Mantendo a mesma semente, a malha gerada será sempre a mesma.
        // Isso facilita testes, depuração e comparação de resultados.
        const std::uint64_t semente = 20260301;

        //======================================================================
        // Impressão dos dados iniciais
        //======================================================================

        imprimir_dados_iniciais(
            xI,
            comprimento,
            numero_nos,
            semente
        );

        //======================================================================
        // Geração da malha
        //======================================================================

        const std::vector<Real> x = gerar_malha_aleatoria(
            xI,
            comprimento,
            numero_nos,
            semente
        );

        //======================================================================
        // Cálculo dos espaçamentos
        //======================================================================

        const std::vector<Real> dx = calcular_espacamentos(x);

        //======================================================================
        // Impressão dos resultados
        //======================================================================

        imprimir_malha(x);
        imprimir_resumo_dos_espacamentos(dx);

        return 0;
    } catch (const std::exception& erro) {
        std::cerr << "Erro: " << erro.what() << '\n';
        return 1;
    }
}

//==============================================================================
// Impressão dos dados iniciais
//==============================================================================

inline void imprimir_dados_iniciais(
    Real xI,
    Real comprimento,
    std::size_t numero_nos,
    std::uint64_t semente
)
{
    const Real x_final = xI + comprimento;

    std::cout << std::fixed << std::setprecision(12);

    std::cout << "Dados iniciais da execucao\n";
    std::cout << "==========================\n";
    std::cout << "xI            = " << xI << '\n';
    std::cout << "L             = " << comprimento << '\n';
    std::cout << "xI + L        = " << x_final << '\n';
    std::cout << "N             = " << numero_nos << '\n';
    std::cout << "nos internos  = " << numero_nos - 2 << '\n';
    std::cout << "semente       = " << semente << '\n';
    std::cout << '\n';
}

//==============================================================================
// Validação dos dados de entrada
//==============================================================================

inline void validar_dados_da_malha(
    Real xI,
    Real comprimento,
    std::size_t numero_nos
)
{
    if (!std::isfinite(xI)) {
        throw std::invalid_argument("A coordenada inicial xI deve ser finita.");
    }

    if (!std::isfinite(comprimento)) {
        throw std::invalid_argument("O comprimento L deve ser finito.");
    }

    if (comprimento <= Real{0.0}) {
        throw std::invalid_argument("O comprimento L deve ser positivo.");
    }

    if (numero_nos <= 2) {
        throw std::invalid_argument("O numero de nos N deve ser maior que 2.");
    }
}

//==============================================================================
// Verificação da ordenação da malha
//==============================================================================

[[nodiscard]] inline bool malha_estritamente_crescente(
    std::span<const Real> x
)
{
    for (std::size_t i = 1; i < x.size(); ++i) {
        if (!(x[i - 1] < x[i])) {
            return false;
        }
    }

    return true;
}

//==============================================================================
// Geração da malha aleatória
//==============================================================================

[[nodiscard]] inline std::vector<Real> gerar_malha_aleatoria(
    Real xI,
    Real comprimento,
    std::size_t numero_nos,
    std::uint64_t semente
)
{
    validar_dados_da_malha(xI, comprimento, numero_nos);

    const Real x_final = xI + comprimento;

    if (!std::isfinite(x_final)) {
        throw std::invalid_argument("O extremo final xI + L deve ser finito.");
    }

    // Define limites internos ao domínio.
    //
    // std::nextafter desloca os extremos ligeiramente para dentro do intervalo.
    // Assim, os pontos sorteados pertencem ao intervalo aberto (xI, xI + L).
    const Real limite_inferior = std::nextafter(xI, x_final);
    const Real limite_superior = std::nextafter(x_final, xI);

    if (!(limite_inferior < limite_superior)) {
        throw std::runtime_error(
            "O intervalo e pequeno demais para conter nos internos distintos."
        );
    }

    // Configura o gerador pseudoaleatório.
    std::mt19937_64 gerador(semente);

    // Configura a distribuição uniforme para os nós internos.
    std::uniform_real_distribution<Real> distribuicao(
        limite_inferior,
        limite_superior
    );

    std::vector<Real> nos_internos;
    nos_internos.reserve(numero_nos - 2);

    const std::size_t numero_nos_internos = numero_nos - 2;

    // Define um limite defensivo para o número de tentativas.
    //
    // Em ponto flutuante, a chance de sortear exatamente o mesmo valor duas
    // vezes é pequena. Mesmo assim, a proteção evita aceitar nós coincidentes.
    const std::size_t maximo_tentativas =
        1000 + 100 * numero_nos_internos;

    std::size_t tentativas = 0;

    // Sorteia os nós internos.
    while (nos_internos.size() < numero_nos_internos) {
        if (tentativas >= maximo_tentativas) {
            throw std::runtime_error(
                "Nao foi possivel gerar todos os nos internos distintos."
            );
        }

        const Real candidato = distribuicao(gerador);

        const bool candidato_repetido =
            std::ranges::find(nos_internos, candidato) != nos_internos.end();

        if (!candidato_repetido) {
            nos_internos.push_back(candidato);
        }

        ++tentativas;
    }

    // Ordena os nós internos.
    //
    // A ordenação garante uma sequência crescente, mas preserva a irregularidade
    // dos espaçamentos, que é o objetivo da malha aleatória.
    std::ranges::sort(nos_internos);

    std::vector<Real> x;
    x.reserve(numero_nos);

    // Insere o extremo esquerdo do domínio.
    x.push_back(xI);

    // Insere os nós internos já ordenados.
    x.insert(x.end(), nos_internos.begin(), nos_internos.end());

    // Insere o extremo direito do domínio.
    x.push_back(x_final);

    if (!malha_estritamente_crescente(x)) {
        throw std::runtime_error("A malha gerada nao ficou crescente.");
    }

    return x;
}

//==============================================================================
// Cálculo dos espaçamentos
//==============================================================================

[[nodiscard]] inline std::vector<Real> calcular_espacamentos(
    std::span<const Real> x
)
{
    if (x.size() < 2) {
        throw std::invalid_argument(
            "Sao necessarios ao menos dois nos para calcular espacamentos."
        );
    }

    std::vector<Real> dx(x.size() - 1);

    // Calcula cada espaçamento local da malha.
    for (std::size_t i = 0; i + 1 < x.size(); ++i) {
        dx[i] = x[i + 1] - x[i];
    }

    return dx;
}

//==============================================================================
// Impressão da malha
//==============================================================================

inline void imprimir_malha(
    std::span<const Real> x
)
{
    std::cout << std::fixed << std::setprecision(12);

    std::cout << "Malha gerada\n";
    std::cout << "============\n";

    std::cout << std::setw(6)  << "i"
              << std::setw(20) << "x_i"
              << std::setw(20) << "dx_esquerda"
              << std::setw(20) << "dx_direita"
              << '\n';

    // Imprime cada nó e os espaçamentos vizinhos.
    for (std::size_t i = 0; i < x.size(); ++i) {
        std::cout << std::setw(6) << i
                  << std::setw(20) << x[i];

        if (i == 0) {
            std::cout << std::setw(20) << "-";
        } else {
            std::cout << std::setw(20) << x[i] - x[i - 1];
        }

        if (i + 1 == x.size()) {
            std::cout << std::setw(20) << "-";
        } else {
            std::cout << std::setw(20) << x[i + 1] - x[i];
        }

        std::cout << '\n';
    }
}

//==============================================================================
// Impressão de estatísticas simples
//==============================================================================

inline void imprimir_resumo_dos_espacamentos(
    std::span<const Real> dx
)
{
    if (dx.empty()) {
        throw std::invalid_argument("O vetor de espacamentos nao pode estar vazio.");
    }

    const auto [menor, maior] = std::ranges::minmax_element(dx);

    Real soma = Real{0.0};

    // Soma todos os espaçamentos para calcular o valor médio.
    for (const Real valor : dx) {
        soma += valor;
    }

    const Real media = soma / static_cast<Real>(dx.size());
    const Real razao = *maior / *menor;

    std::cout << "\nResumo dos espacamentos\n";
    std::cout << "=======================\n";
    std::cout << "dx_min        = " << *menor << '\n';
    std::cout << "dx_max        = " << *maior << '\n';
    std::cout << "dx_medio      = " << media << '\n';
    std::cout << "dx_max/dx_min = " << razao << '\n';
}