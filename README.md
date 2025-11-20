# FVGridMaker

Biblioteca em C++20 para geração de **malhas estruturadas 1D e 2D** voltadas
para o Método dos Volumes Finitos (FVM), com foco em:

- **Desempenho** (layout de dados em SoA, paralelização com `std::execution`);
- **Previsibilidade geométrica** (métricas pré-calculadas e somente leitura);
- **Extensibilidade** usando templates, traits e policies;
- **Integração simples** com códigos de CFD e outras aplicações científicas.

> Status: em desenvolvimento (API sujeita a mudanças).

---

## 1. Visão geral

A **FVGridMaker** é uma biblioteca de geração e gerenciamento de malhas
estruturadas que separa explicitamente:

- **Geração de coordenadas 1D** (malhas em linha);
- **Construção de malha 2D** como produto de duas malhas 1D;
- **Geometria** (sistema de coordenadas, mapeamentos);
- **Discretização** (espaçamento, layouts célula-centrado, face-centrado, etc.);
- **Campos** (escalares e vetoriais definidos sobre a malha);
- **Exportação** para pós-processamento (VTK, arquivos tabulares).

O objetivo é permitir que o usuário escreva **solvers em FVM** sem
reimplementar:

- indexação (1D ↔ (i,j));
- cálculo de áreas/volumes e distâncias;
- tratamento de ghost cells;
- exportação básica para ferramentas de visualização.

---

## 2. Motivação

Em muitos códigos de CFD e de simulação numérica:

- a lógica de malha fica misturada com a lógica do problema físico;
- diferentes trabalhos reimplementam a mesma infraestrutura geométrica;
- otimizações de cache e paralelismo são feitas de forma ad-hoc,
  frequentemente difíceis de manter.

A FVGridMaker tenta resolver isso ao fornecer um **núcleo de malha moderno**,
com:

- **C++20** e STL moderna;
- **data-oriented design (DOD)**;
- **princípios de código limpo e SOLID**, dentro do possível num cenário com
  templates;
- **extensibilidade** via traits/policies, sem precisar alterar o core.

---

## 3. Principais funcionalidades

### 3.1 Malhas 1D

A classe `Grid1D` representa uma malha unidimensional:

- domínios do tipo \( x \in [L_{\min}, L_{\max}] \);
- número de volumes internos \( N_{\text{cells}} \) definido pelo usuário;
- suporte a **ghost cells** em cada extremidade;
- diferentes políticas de espaçamento (discretização 1D):
  - espaçamento uniforme;
  - espaçamento estirado (funções de estiramento definidas pelo usuário);
  - malhas **aleatórias / jittered** (monótonas, mas com perturbações controladas);
  - possibilidade de usar tabelas de pontos já pré-definidas.

### 3.2 Malhas 2D como produto de duas 1D

A malha 2D (`Grid2D`) é construída como **produto cartesiano de duas `Grid1D`**:

- uma `Grid1D` em cada direção lógica (por exemplo, \(i\) e \(j\));
- `Grid2D` combina duas malhas 1D + uma política de geometria 2D:
  - cartesiana;
  - polar/cilíndrica;
  - elíptica.

Essa abordagem traz algumas vantagens:

- grande parte da lógica (espaçamento, aleatoriedade, ghost) é implementada
  apenas em 1D;
- manutenção e testes ficam mais simples;
- fica natural reutilizar e combinar diferentes padrões 1D em 2D.

### 3.3 Ghost cells como reflexão

Ghost cells são opcionais e sua espessura é configurável. Para 1D e 2D:

- ghost cells são preenchidas por **reflexão** em relação ao primeiro e ao
  último volume **interno**;
- isso garante continuidade geométrica na fronteira e facilita a implementação
  de condições de contorno no solver;
- a indexação inclui ghost cells de forma contígua em memória.

### 3.4 Geometria e métricas pré-calculadas

A FVGridMaker **pré-calcula todos os dados geométricos** necessários e os
mantém em vetores separados (SoA):

- coordenadas de centros de volumes:
  - 1D: \(x_P\);
  - 2D: \((x_P, y_P)\);
- coordenadas de faces:
  - 1D: \(x_w, x_e\);
  - 2D: \(x_w, x_e, y_s, y_n\), com otimização 1D quando possível.
- distâncias entre centros (úteis para termos difusivos);
- larguras de célula (distâncias entre faces);
- medida da célula:
  - comprimento em 1D;
  - área em 2D (incluindo fatores geométricos em coordenadas curvilíneas).

Esses vetores são expostos para o solver como **somente leitura** (por exemplo,
via `std::span<const T>`), para evitar alterações acidentais na geometria.

### 3.5 Iteradores e ranges

A biblioteca define ranges/iteradores especializados para:

- **volumes internos** (sem ghost, sem borda);
- **volumes de fronteira**, por lado:
  - norte (North),
  - sul (South),
  - leste (East),
  - oeste (West);
- **todos os volumes físicos** (internos + fronteiras, excluindo ghosts).

As propriedades desejadas:

- iteradores pelo menos de **Random Access** (compatíveis com STL e PSTL);
- ranges compatíveis com algoritmos padrão, incluindo execução paralela:

```cpp
std::for_each(std::execution::par,
              grid.interior_begin(),
              grid.interior_end(),
              [&](auto cell_index) {
                  // Atualizar campo em cada volume interno
              });
```

### 3.6 Layouts de discretização (cell-centered, face-centered e além)

O arranjo **célula-centrado** é o padrão da biblioteca:

- variáveis primárias associadas aos centros dos volumes de controle;
- métricas geométricas calculadas assumindo este arranjo.

Outros arranjos são implementados via **políticas de layout**:

- `CellCentered` (padrão);
- `FaceCentered` (face-centrado).

Se o usuário quiser um layout diferente (por exemplo, híbrido ou dual), basta:

1. Definir uma nova policy de layout (sem alterar o core);
2. Ajustar traits/concepts de discretização para reconhecer esse layout;
3. Usar essa policy ao declarar o campo.

A ideia é **não precisar alterar `Grid1D`/`Grid2D`** para adicionar novos layouts.

### 3.7 Campos escalares e vetoriais

O módulo `Fields` cuida de campos definidos sobre a malha:

- `Field<Grid, LayoutPolicy, T>`: tipo genérico parametrizado;
- aliases/utilitários:
  - `ScalarField<Grid, LayoutPolicy>` – campos escalares;
  - `VectorField<Grid, LayoutPolicy>` – campos vetoriais em 1D ou 2D.

O objetivo é separar:

- “onde” o dado é armazenado (definido pelo layout/policy);
- “que tipo de dado” é armazenado (escalar, vetor, etc.);
- “sobre qual malha” o dado vive (1D, 2D, com ghost, etc.).

### 3.8 Paralelismo e adaptação ao tamanho da malha

A FVGridMaker faz uso explícito de **paralelização padrão C++20**:

- algoritmos em `<algorithm>` + políticas em `<execution>`;
- operações naturalmente paralelizáveis:
  - inicialização de coordenadas;
  - cálculo de métricas;
  - transformações geométricas (rotação, translação).

Uma diretriz importante é:

- para malhas pequenas, **execução sequencial** (`std::execution::seq`) pode
  ser mais rápida;
- para malhas acima de um certo número de volumes (definido em configuração),
  usar `std::execution::par` ou `std::execution::par_unseq`.

A decisão pode ser tomada em função de:

- número total de volumes (1D ou 2D);
- disponibilidade de threads do ambiente.

### 3.9 Exportação e pós-processamento

O módulo `IO` oferece:

- exportação em **VTK Legacy ASCII** (`.vtk`) para visualização em ParaView,
  VisIt e similares;
- formatos **tabulares** simples (`.csv`, `.txt`), úteis para análises
  rápidas ou integração com outras ferramentas.

A exportação reconstrói a topologia visual a partir dos vetores geométricos,
sem exigir que o solver reconstrua conectividade.

Toda a escrita é feita com a **STL**, sem dependências externas (sem VTK, etc.).

---

## 4. Arquitetura e organização do código

### 4.1 Princípios de design

A arquitetura da FVGridMaker segue as diretrizes:

- **C++20 puro** no núcleo da biblioteca (apenas STL);
- **data-oriented design**:
  - foco em como os dados são armazenados e percorridos;
- **SOLID e código limpo**, na medida do possível com templates:
  - responsabilidades bem definidas,
  - baixo acoplamento entre módulos;
- **templates, traits e policies** para extensibilidade:
  - evitar `enum` e `switch` sempre que possível;
  - preferir “tag types” e traits em compile-time;
- **separação clara** entre:
  - Core (Grid 1D/2D e traits);
  - Geometry (sistemas de coordenadas);
  - Discretization (espaçamentos e layouts);
  - Fields (armazenamento de campos);
  - IO (exportação);
  - FVException (tratamento de erros).

### 4.2 Layout de diretórios (resumo)

```text
FVGridMaker/
  CMakeLists.txt
  README.md
  LICENSE
  .gitignore

  include/
    FVGridMaker/
      FVGridMaker.hpp          # Header principal (facade)

      Core/
        Common/
          Types.hpp
          Config.hpp
          GridTraits.hpp
          ID.hpp
        1D/
          Grid1D.hpp
        2D/
          Grid2D.hpp

      Geometry/
        Traits/
          GeometryTraits.hpp
        1D/
          Cartesian1D.hpp
        2D/
          Cartesian2D.hpp
          Polar2D.hpp
          Elliptic2D.hpp

      Discretization/
        Traits/
          DiscretizationTraits.hpp
        1D/
          UniformSpacing.hpp
          StretchedSpacing.hpp
          RandomSpacing.hpp
        2D/
          CellCentered2D.hpp
          FaceCentered2D.hpp

      Fields/
        Field.hpp
        ScalarField.hpp
        VectorField.hpp

      FVException/
        FVException.hpp

      IO/
        VTKWriter.hpp
        Endianness.hpp

  src/
    ...                        # Implementações .cpp (se necessário)

  tests/
    CMakeLists.txt
    main_test.cpp
    Unit/
      TestGrid1D.cpp
      TestGrid2D_Cartesian.cpp
      TestGrid2D_Polar.cpp
      TestGrid2D_Elliptic.cpp
      TestScalarField.cpp
      TestVectorField.cpp
    Validation/
      ConservationTest.cpp

  examples/
    CMakeLists.txt
    01_BasicGrid1D.cpp
    02_BasicGrid2D.cpp
    03_RotatedDomain.cpp

  docs/
    manual/
      Manual_FVGridMaker_Completo.tex
      images/
```

---

## 5. Sequência sugerida de implementação

Esta seção serve como um “roteiro interno” para desenvolvimento da biblioteca.

1. **Módulo Core/Common**
   - `Types.hpp` – aliases básicos (`Real`, índices, dimensões).
   - `Config.hpp` – constantes globais, macros de namespace, parâmetros padrão.
   - `GridTraits.hpp` – traits estáticos para malhas (1D/2D).
   - `ID.hpp` – identificação de instâncias para fins de logging.

2. **Infraestrutura de exceções**
   - Integrar a classe `FVException` com a infraestrutura de exceções já
     existente no projeto (derivando de `std::exception` ou encapsulando).

3. **Grid1D**
   - Representação mínima de uma malha 1D com:
     - domínios físicos;
     - ghost cells;
     - índices lógicos.
   - Cálculo de coordenadas e métricas em 1D.

4. **Discretização 1D**
   - `UniformSpacing.hpp` – malha uniforme.
   - `StretchedSpacing.hpp` – malha estirada.
   - `RandomSpacing.hpp` – malhas aleatórias monotônicas.
   - Integração dessas policies com `Grid1D` via traits.

5. **Geometry 1D**
   - `Cartesian1D.hpp` – geometria cartesiana básica (suporte para futura
     extensão se necessário).

6. **Grid2D**
   - Implementar `Grid2D` como produto de duas `Grid1D`:
     - construção a partir de dois objetos `Grid1D` e de um mapeamento
       geométrico 2D (geometry policy);
     - cálculo de métricas 2D a partir das métricas 1D + geometria.

7. **Geometry 2D**
   - `Cartesian2D.hpp` – sistema cartesiano 2D (prioridade alta).
   - `Polar2D.hpp` – coordenadas polares/cilíndricas.
   - `Elliptic2D.hpp` – coordenadas elípticas.

8. **Discretização 2D / Layouts**
   - `CellCentered2D.hpp` – layout célula-centrado (padrão).
   - `FaceCentered2D.hpp` – layout face-centrado (exemplo canônico).
   - Traits de discretização para permitir novos layouts no futuro sem
     alterar `Grid2D`.

9. **Fields**
   - `Field.hpp`, `ScalarField.hpp`, `VectorField.hpp`.
   - Integração com os iteradores/ranges da malha.

10. **Iteradores e ranges**
    - Implementar iteradores para:
      - interior,
      - fronteiras (N/S/E/W),
      - todos os volumes físicos.
    - Garantir compatibilidade com algoritmos paralelos da STL.

11. **IO / exportação**
    - `VTKWriter.hpp` – exportação VTK Legacy ASCII.
    - `Endianness.hpp` – utilitários para escrita binária (se necessário).

12. **Testes e exemplos**
    - Testes unitários (Google Test):
      - Grid1D, Grid2D, geometria, discretização, fields, IO.
    - Exemplos:
      - malha 1D básica;
      - malha 2D cartesiana;
      - malha 2D rotacionada com exportação para VTK.

---

## 6. Requisitos de compilação

- **Linguagem:** C++20 (ou superior).
- **Compilador:**
  - GCC 12+ ou equivalente,
  - Clang 15+ ou equivalente,
  - MSVC com suporte completo a C++20.
- **Build system:** CMake (out-of-source build recomendado).
- **Dependências externas:**
  - **núcleo:** nenhuma (apenas biblioteca padrão);
  - **tests:** Google Test (opcional, mas recomendado).

---

## 7. Como compilar

Exemplo genérico (ajuste caminhos conforme seu projeto):

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

Para compilar testes (se habilitados no `CMakeLists.txt`):

```bash
ctest --output-on-failure
```

---

## 8. Uso básico (esqueleto)

A API ainda está em evolução, mas um uso típico pode ter a seguinte forma:

```cpp
#include <FVGridMaker/FVGridMaker.hpp>

using namespace fvgridmaker; // conforme definido em Config.hpp

int main() {
    // 1D: criar malha com 100 células internas em [0, 1]
    auto grid1d_x = make_uniform_grid1d(/*Lmin=*/0.0, /*Lmax=*/1.0,
                                        /*Ncells=*/100,
                                        /*Nghost=*/2);

    auto grid1d_y = make_uniform_grid1d(0.0, 0.5, 50, 2);

    // 2D: produto de duas Grid1D, com geometria cartesiana 2D
    auto grid2d = make_cartesian_grid2d(grid1d_x, grid1d_y);

    // Campo escalar célula-centrado
    ScalarField<decltype(grid2d), CellCentered, double> phi(grid2d);

    // Percorrer volumes internos em paralelo
    std::for_each(std::execution::par,
                  grid2d.interior_begin(),
                  grid2d.interior_end(),
                  [&](auto cell) {
                      // inicializar phi(cell) de forma qualquer
                  });

    // Exportar malha para VTK
    VTKWriter writer("malha.vtk");
    writer.write(grid2d);

    return 0;
}
```

(O código acima é apenas ilustrativo; os nomes finais de funções e tipos podem
mudar conforme a implementação avançar.)

---

## 9. Roadmap

Alguns pontos planejados para versões futuras:

- Suporte a mais políticas de espaçamento (p.ex., baseado em funções analíticas
  fornecidas pelo usuário).
- Heurísticas mais sofisticadas para decidir entre execução sequencial e
  paralela com base no tamanho da malha e na arquitetura da máquina.
- Exportação adicional em VTK XML (quando for compatível com o escopo de
  “somente STL”).
- Ferramentas de validação adicional (conservação em malhas curvilíneas,
  testes de qualidade de malha).
- Scripts/arquivos de exemplo integrados à documentação (manual em LaTeX,
  Sphinx, etc.).

---

## 10. Licença

A FVGridMaker é distribuída sob uma licença de código aberto (por exemplo,
MIT ou BSD – consulte o arquivo `LICENSE` no repositório para detalhes).

---

## 11. Contato e contribuições

No momento o foco principal é o uso em pesquisa e ensino, mas:

- issues, sugestões e relatórios de bug são bem-vindos;
- contribuições de código podem ser discutidas via issues antes de serem
  submetidas como pull requests.

Para dúvidas técnicas mais específicas sobre a biblioteca ou sobre o método
dos volumes finitos, recomenda-se abrir uma issue detalhando:

- versão do compilador e flags de compilação;
- sistema operacional;
- trecho mínimo de código que reproduz o problema.
