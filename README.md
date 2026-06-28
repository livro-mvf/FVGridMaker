# FVGridMaker

FVGridMaker é uma biblioteca C++20 para construir geometria de malhas estruturadas de volumes finitos. Ela fornece eixos 1D, composição estruturada 2D, sistemas de coordenadas, medidas geométricas, validação de invariantes e saída CSV/VTK legacy.

A biblioteca tem foco acadêmico: a API tenta deixar explícitas as hipóteses geométricas usadas em uma malha antes de qualquer solver, campo físico ou operador discreto entrar no problema.

## Escopo

FVGridMaker trata de:

```text
malhas estruturadas de volumes finitos;
eixos unidimensionais;
malhas 2D por produto tensorial de dois eixos 1D;
faces, centros, distâncias e medidas geométricas;
padrões de reconstrução face centrada e volume centrada;
sistemas de coordenadas cartesiano, polar e axisimétricos;
validação de invariantes geométricos;
saída CSV para eixos 1D;
saída VTK legacy para malhas 2D.
```

FVGridMaker não trata de:

```text
equações diferenciais;
campos físicos;
condições de contorno;
operadores de difusão/advecção;
montagem de matriz;
solvers lineares;
integração temporal;
malhas não estruturadas;
YAML ou outros parsers de configuração no núcleo da biblioteca.
```

## Documentação

A documentação principal é gerada com Sphinx, Doxygen e Breathe.

```bash
cmake -S . -B build_docs -DBUILD_DOCUMENTATION=ON \
      -DBUILD_EXAMPLES=OFF -DBUILD_TESTS=OFF
cmake --build build_docs --target docs -j
```

Abra:

```text
build_docs/docs/html/index.html
```

A documentação Sphinx contém guias conceituais, páginas de arquitetura, exemplos de uso e referência completa da API C++ gerada a partir dos headers públicos.

## Uso rápido

```cpp
#include <FVGridMaker/FVGridMaker.h>

int main() {
    const auto x = fvgrid::uniform_axis_1d(100, 0.0, 1.0);
    const auto y = fvgrid::uniform_axis_1d(50, -0.5, 0.5);

    const fvgrid::StructuredGrid2D grid{x, y};

    const double area_logica = grid.cell_logical_area(0, 0);
    const double medida_fisica = grid.cell_measure(0, 0);

    fvgrid::Axis1DCSVWriter::write(x, "axis_x.csv");
    fvgrid::write_vtk(grid, "grid.vtk");

    (void)area_logica;
    (void)medida_fisica;
}
```

## API escalar

Os nomes simples usam `double`:

```cpp
fvgrid::Axis1D
fvgrid::Uniform1D
fvgrid::Random1D
fvgrid::Roberts1D
fvgrid::StructuredGrid2D
fvgrid::Length
fvgrid::XInit
fvgrid::Beta
```

A API avançada expõe aliases explícitos e templates:

```cpp
fvgrid::Axis1DFloat
fvgrid::Axis1DLongDouble
fvgrid::StructuredGrid2DFloat
fvgrid::StructuredGrid2DLongDouble

const auto xf = fvgrid::uniform_axis_1d<float>(100, 0.0f, 1.0f);
const auto xl = fvgrid::uniform_axis_1d<long double>(100, 0.0L, 1.0L);
```

Tipos que armazenam escalares expõem `value_type` para facilitar testes genéricos e integração com outras bibliotecas.

## Padrão das malhas

Quando o padrão não é especificado, os geradores `Uniform1D`, `Random1D` e `Roberts1D` usam `FaceCentered1D`.

```cpp
const auto axis = fvgrid::Uniform1D::make(
    fvgrid::NVol{10},
    fvgrid::Length{1.0},
    fvgrid::XInit{0.0}
);

// Equivalente a passar fvgrid::FaceCentered1D{} explicitamente.
```

Para construir uma malha volume centrada, peça isso explicitamente:

```cpp
const auto axis = fvgrid::Uniform1D::make(
    fvgrid::NVol{10},
    fvgrid::Length{1.0},
    fvgrid::XInit{0.0},
    fvgrid::VolumeCentered1D{}
);
```

A construção direta `Axis1D{{faces...}}` continua sendo volume centrada porque o vetor fornecido é interpretado como vetor de faces explícitas.

## Geometria 1D

Para `N` volumes finitos, um eixo possui:

```text
faces      : N + 1
centros    : N
dx_faces   : N
dx_centers : N + 1
```

A biblioteca valida que cada centro pertence à sua célula:

```text
faces[p] < centers[p] < faces[p + 1]
```

Essa validação é importante em contexto acadêmico: ela impede que uma lista de centros apenas globalmente crescente seja aceita como geometria localmente válida.

## Geometria 2D

`StructuredGrid2D` compõe dois `Axis1D` independentes. A indexação de células é row-major:

```text
k = j * num_cells_x + i
```

A API diferencia:

```text
cell_logical_area(i, j)  -> área computacional no espaço lógico;
cell_measure(i, j)       -> medida física dada pelo sistema de coordenadas.
```

Em coordenadas cartesianas esses valores coincidem. Em polar ou axisimétrico, eles representam grandezas diferentes.

## Erros

Falhas de validação geram `FVGridException` com código estável, categoria, origem e local de lançamento. Isso torna os testes e relatórios reprodutíveis.

## Build e testes

Build padrão:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

Testes:

```bash
cmake -S . -B build-tests -DBUILD_TESTS=ON -DBUILD_EXAMPLES=OFF
cmake --build build-tests -j
ctest --test-dir build-tests --output-on-failure
```

Exemplos:

```bash
cmake -S . -B build-examples -DBUILD_EXAMPLES=ON -DBUILD_TESTS=OFF
cmake --build build-examples -j
cmake --build build-examples --target run_all_examples
```

## Organização

```text
FVGridMakerLib/   biblioteca principal
examples/         exemplos executáveis e material didático
tests/            testes unitários e de invariantes
docs/             documentação Sphinx/Doxygen
scripts/          material auxiliar e snapshots de planejamento
```

## Princípios de projeto

```text
C++20;
valores sempre válidos;
armazenamento contíguo;
sem polimorfismo virtual no caminho geométrico;
conceitos e traits para extensão;
sem dependência de FVMaker;
sem dependência real na biblioteca VTK;
separação entre geometria de malha e solver numérico.
```