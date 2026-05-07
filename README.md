# FVGridMaker

Biblioteca C++20 para geração e inspeção de malhas estruturadas 1D e 2D,
pensada para códigos de volumes finitos, ensino e prototipagem numérica.

> Status: em desenvolvimento. A API ainda pode mudar enquanto os módulos de
> malha, I/O e utilitários amadurecem.

## Visão Geral

A FVGridMaker organiza a criação de malhas em componentes pequenos e
reutilizáveis:

- `Grid1D`: malhas unidimensionais com centros, faces e métricas pré-calculadas;
- `Grid2D`: malhas bidimensionais estruturadas;
- `Grid1DBuilder` e builders relacionados: construção fluente de malhas;
- distribuições: malhas uniformes, aleatórias e futuras políticas de espaçamento;
- volumes fictícios: suporte a ghost cells por lado;
- I/O: exportação tabular e VTK para visualização e depuração.

O foco inicial é deixar a geometria da malha clara, verificável e fácil de
usar em solvers, sem misturar a lógica geométrica com a lógica física do
problema.

## Recursos Atuais

- C++20, CMake e biblioteca padrão como base.
- Construção de malhas 1D uniformes e aleatórias.
- Suporte a volumes fictícios em malhas 1D.
- Acesso separado a centros, faces, distâncias centro-centro e distâncias
  face-face.
- Exemplos compiláveis descobertos automaticamente pelo CMake.
- Testes unitários opcionais.
- Saída dos executáveis dentro de `build/`, sem poluir as pastas de código-fonte.

## Estrutura do Projeto

```text
FVGridMaker/
  CMakeLists.txt
  README.md

  FVGridMakerLib/
    include/FVGridMaker/
      Core/
      ErrorHandling/
      Grid/
      IO/
      User/
    src/

  examples/
    CMakeLists.txt
    ErrorHandling/
    Grid/
    IO/

  tests/
    CMakeLists.txt

  cmake/
    ConfigOptions.cmake
    ConfigTargets.cmake
    ConfigExamples.cmake
    ConfigTests.cmake
    ConfigDocs.cmake
```

## Requisitos

- CMake 3.15 ou superior.
- Compilador com suporte a C++20.
- GCC, Clang ou MSVC recente.
- Google Test apenas se os testes forem habilitados.

As opções principais de build são:

```cmake
BUILD_EXAMPLES=ON
BUILD_TESTS=OFF
BUILD_DOCS=OFF
```

## Como Compilar

Build típico em modo Release:

```bash
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel 2
```

Para habilitar testes:

```bash
cmake .. -DBUILD_TESTS=ON
cmake --build . --parallel 2
ctest --output-on-failure
```

## Primeiro Exemplo

O ponto de entrada recomendado para quem está começando é:

```text
examples/Grid/Grid1D/PrimeiroExemplo/Ex_PrimeiroExemploGrid1D.cpp
```

Esse exemplo cria uma malha 1D uniforme no domínio físico `[0, 1]`, com:

- 5 volumes físicos;
- 1 volume fictício em cada lado;
- impressão tabular das coordenadas de centros, faces e espaçamentos.

Para compilar e executar a partir da pasta `build`:

```bash
cmake --build . --target run_Ex_PrimeiroExemploGrid1D --parallel 2
```

Por padrão, o exemplo imprime apenas a malha física. Assim, a primeira face
impressa é o início do domínio:

```text
face_x[0] = xInicial
```

Para imprimir também os volumes fictícios, execute o binário gerado com:

```bash
./examples/Grid/Grid1D/PrimeiroExemplo/Ex_PrimeiroExemploGrid1D --com-ficticios
```

A tabela usa a seguinte legenda:

```text
i   centro_x   face_x   dcentro_x   dface_x
```

Para `N` volumes físicos:

- `centro_x` e `dface_x` têm `N` valores;
- `face_x` e `dcentro_x` têm `N + 1` valores.

Por isso, algumas colunas podem ficar vazias na última linha da tabela.

## Exemplo Mínimo em C++

```cpp
#include <FVGridMaker/Grid/Grid1D/Builders/Grid1DBuilder.hpp>
#include <FVGridMaker/Grid/Grid1D/Patterns/Distribution/Uniform1D.hpp>

#include <iostream>

int main() {
    namespace grid1d = FVGridMaker::grid::grid1d;
    namespace dist   = FVGridMaker::grid::grid1d::patterns::distribution;

    using Grid1DBuilder = grid1d::builders::Grid1DBuilder;

    const auto malha =
        Grid1DBuilder{}
            .setN(5)
            .setDomain(0.0, 1.0)
            .setNFicticios(1)
            .setDistribution<dist::Uniform1D>()
            .build();

    std::cout << "Volumes fisicos: "
              << malha.nVolumesFisicos() << '\n';

    std::cout << "Primeira face fisica: "
              << malha.faceFisica(0) << '\n';

    return 0;
}
```

Este exemplo usa somente os headers necessários para os símbolos utilizados
diretamente no arquivo. Essa é uma diretriz do projeto: evitar includes
sobrando e não depender de includes transitivos sempre que possível.

## Onde os Executáveis Ficam

Os exemplos são compilados para dentro da árvore de build:

```text
build/examples/...
```

Eles não devem ser gerados dentro das pastas de código-fonte em `examples/...`.

Exemplo:

```text
build/examples/Grid/Grid1D/PrimeiroExemplo/Ex_PrimeiroExemploGrid1D
```

## Alvos Úteis

A partir da pasta `build`, alguns alvos úteis são:

```bash
cmake --build . --target examples --parallel 2
cmake --build . --target run_all_examples --parallel 2
cmake --build . --target run_Ex_PrimeiroExemploGrid1D --parallel 2
```

Para testes, quando `BUILD_TESTS=ON`:

```bash
ctest --output-on-failure
```

## Convenções de Desenvolvimento

- Usar C++20.
- Preferir APIs e padrões já existentes no projeto.
- Manter exemplos didáticos e comentados, especialmente os de entrada.
- Verificar se cada `#include` é realmente necessário.
- Evitar depender de includes transitivos em arquivos `.cpp`.
- Manter artefatos de build fora das pastas de código-fonte.
- Preservar nomes claros para grandezas geométricas:
  - `centro_x`, `face_x`, `dcentro_x`, `dface_x`;
  - futuramente, o mesmo padrão para `y`.

## Roadmap Próximo

- Consolidar o primeiro exemplo 1D como referência para novos usuários.
- Criar um primeiro exemplo 2D seguindo o mesmo padrão didático.
- Melhorar a documentação dos builders e dos acessores físicos.
- Expandir exemplos de exportação CSV e VTK.
- Revisar a API pública antes de estabilizar nomes e aliases.

## Licença

A licença do projeto ainda deve ser confirmada no repositório. Antes de uso,
distribuição ou publicação, adicione um arquivo `LICENSE` na raiz do projeto.
