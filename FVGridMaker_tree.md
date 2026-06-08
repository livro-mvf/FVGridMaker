FVGridMaker — Tree do Projeto
Este documento define a árvore de referência do FVGridMaker no estado atual do desenvolvimento. Ele substitui versões anteriores que ainda usavam `AxisGrid1D` como nome principal. A unidade geométrica 1D atualmente adotada é `Axis1D`.
1. Princípios da árvore
A organização do projeto deve refletir estas decisões:
```text
1. O projeto trabalha com grids estruturados, não meshes não estruturadas.
2. A unidade fundamental 1D é Axis1D.
3. Axis1D armazena geometria completa: faces, centers, dx_faces e dx_centers.
4. Uma grade 2D será composição de dois Axis1D.
5. Distribution1D gera coordenadas primárias segundo uma distribuição.
6. GridPattern1D define como coordenadas secundárias são reconstruídas.
7. Axis1D calcula métricas geométricas gerais depois que faces e centers existem.
8. Operations1D contém operações entre eixos/grades já construídos.
9. CoordinateSystem interpreta os eixos e calcula medidas geométricas físicas.
10. Output1D e Output2D são periféricos.
11. YAML é periférico.
12. GoogleTest aparece apenas nos testes.
13. Nenhum módulo periférico deve criar dependência reversa no núcleo.
14. Nenhuma categoria extensível deve ser implementada com enum.
```
A palavra `Mesh` deve ser evitada nos nomes principais. O vocabulário preferencial é:
```text
Grid
Axis
StructuredGrid
Distribution
GridPattern
CoordinateSystem
Operations
Output
```
2. Tree atual/recomendada
```text
FVGridMaker/
├── CMakeLists.txt
├── README.md
├── LICENSE.md
├── .gitignore
├── .clang-format
│
├── FVGridMakerLib/
│   ├── include/
│   │   └── FVGridMaker/
│   │       ├── FVGridMaker.h
│   │       │
│   │       ├── Core/
│   │       │   ├── ID.h
│   │       │   ├── StrongTypes.h
│   │       │   ├── Types.h
│   │       │   └── Version.h
│   │       │
│   │       ├── ErrorHandling/
│   │       │   ├── ErrorCatalog.h
│   │       │   ├── ErrorCodes.h
│   │       │   ├── ErrorDescriptor.h
│   │       │   ├── ErrorRecord.h
│   │       │   ├── FVGridException.h
│   │       │   └── ThrowError.h
│   │       │
│   │       ├── OneDimensional/
│   │       │   ├── Axis1D/
│   │       │   │   └── Axis1D.h
│   │       │   │
│   │       │   ├── Distribution1D/
│   │       │   │   └── Uniform1D.h
│   │       │   │
│   │       │   ├── GridPattern1D/
│   │       │   │   ├── FaceCentered1D.h
│   │       │   │   └── VolumeCentered1D.h
│   │       │   │
│   │       │   ├── Operations1D/
│   │       │   │   └── <future files>
│   │       │   │
│   │       │   └── Output1D/
│   │       │       └── <future files>
│   │       │
│   │       ├── CoordinateSystem/
│   │       │   ├── Concept/
│   │       │   ├── Measures/
│   │       │   └── Builtin/
│   │       │
│   │       ├── TwoDimensional/
│   │       │   ├── Grid2D/
│   │       │   └── Output2D/
│   │       │
│   │       └── YAML/
│   │           └── <future files>
│   │
│   └── src/
│       └── FVGridMaker/
│           ├── FVGridMaker.cc
│           │
│           ├── Core/
│           │   ├── ID.cc
│           │   └── Version.cc
│           │
│           ├── ErrorHandling/
│           │   ├── FVGridException.cc
│           │   └── ThrowError.cc
│           │
│           └── OneDimensional/
│               ├── Axis1D/
│               │   └── Axis1D.cpp
│               │
│               ├── Distribution1D/
│               │   └── Uniform1D.cc
│               │
│               └── GridPattern1D/
│                   ├── FaceCentered1D.cpp
│                   └── VolumeCentered1D.cpp
│
├── examples/
│   ├── Ex_Minimal/
│   │   └── ex_Minimal.cc
│   │
│   ├── ErrorHandling/
│   │   └── ex_ErrorHandling.cpp
│   │
│   └── OneDimensional/
│       ├── Axis1D/
│       │   └── ex_Axis1D.cc
│       │
│       └── Distribution1D/
│           └── ex_Uniform1D.cc
│
├── tests/
│   ├── Core/
│   │   ├── tst_ID.cc
│   │   ├── tst_StrongTypes.cc
│   │   ├── tst_Types.cc
│   │   └── tst_Version.cc
│   │
│   ├── ErrorHandling/
│   │   ├── tst_ErrorCatalog.cc
│   │   ├── tst_ErrorCodes.cc
│   │   ├── tst_ErrorRecord.cc
│   │   ├── tst_FVGridException.cc
│   │   └── tst_ThrowError.cc
│   │
│   └── OneDimensional/
│       ├── Axis1D/
│       │   └── tst_Axis1D.cc
│       │
│       ├── Distribution1D/
│       │   └── tst_Uniform1D.cc
│       │
│       └── GridPattern1D/
│           └── tst_GridPattern1D.cc
│
└── FVGridMakerLibOld/
    └── <legacy implementation, excluded from the current build>
```
3. Core
`Core/Types.h`
Define aliases fundamentais:
```text
Real
Index
Size
Int8/16/32/64
UInt8/16/32/64
```
`Core/StrongTypes.h`
Define tipos fortes para parâmetros de geração:
```text
NVol
Length
XInit
Seed
```
Esses tipos evitam troca acidental de argumentos em APIs como `Uniform1D::make(NVol, Length, XInit)`.
`Core/ID.h`
Define identidade imutável de classe/componente:
```text
module
class_name
class_id
```
Regra atual:
```text
Toda classe do FVGridMaker deve ter identidade própria.
Nem toda função precisa de identidade própria.
Função/método específico é rastreado por std::source_location.
```
4. ErrorHandling
O sistema de erros usa códigos e descritores textuais, sem `enum`.
Componentes:
```text
ErrorCodes.h        -> códigos textuais estáveis
ErrorDescriptor.h   -> code + message + category
ErrorCatalog.h      -> catálogo interno de descritores FVGridMaker
ErrorRecord.h       -> registro completo do erro
FVGridException.h   -> exceção com ErrorRecord
ThrowError.h        -> throw_error() e require()
```
A forma preferencial de uso interno é:
```cpp
require(
    condition,
    error_catalog::kInvalidNVol,
    Uniform1D::id()
);
```
A forma explícita permanece disponível para erros externos ou mensagens contextuais:
```cpp
require(
    condition,
    "USER.CODE.INVALID_VALUE",
    "Context-specific diagnostic message.",
    "UserCategory",
    UserClass::id()
);
```
5. OneDimensional
`Axis1D`
`Axis1D` é a unidade geométrica 1D fundamental.
Armazena:
```text
faces       -> xface[i],    tamanho nvol + 1
centers     -> xcenter[i],  tamanho nvol
dx_faces    -> dxface[i],   tamanho nvol
dx_centers  -> dxcenter[i], tamanho nvol + 1
```
Define métricas gerais:
```text
dxface[i]       = xface[i + 1] - xface[i]
dxcenter[0]     = xcenter[0] - xface[0]
dxcenter[i]     = xcenter[i] - xcenter[i - 1], 1 <= i < nvol
dxcenter[nvol]  = xface[nvol] - xcenter[nvol - 1]
```
Essas definições são geométricas e valem para qualquer padrão de malha desde que `faces` e `centers` estejam consistentes.
`GridPattern1D`
`GridPattern1D` define como reconstruir coordenadas secundárias a partir de coordenadas primárias.
Atualmente:
```text
VolumeCentered1D:
  coordenadas primárias: faces
  coordenadas secundárias: centers
  regra: centers_from_faces(faces)

FaceCentered1D:
  coordenadas primárias: centers
  coordenadas secundárias: faces
  regra prevista: faces_from_centers(centers, x_min, x_max)
```
`FaceCentered1D` existe como descritor e pode conter regra de reconstrução, mas o `Uniform1D` atual foi deliberadamente mantido restrito ao caminho volume-centred.
`Distribution1D/Uniform1D`
`Uniform1D` atual é volume-centred.
Regra implementada:
```text
1. Uniform1D calcula faces uniformes a partir de NVol, Length e XInit.
2. VolumeCentered1D calcula centers a partir das faces.
3. Axis1D recebe faces + centers e calcula dx_faces + dx_centers.
```
A versão face-centred de `Uniform1D` foi discutida, mas deve entrar em bloco próprio para evitar mistura prematura de responsabilidades.
6. Exemplos públicos atuais
```text
run_ex_Minimal
run_ex_ErrorHandling
run_ex_Axis1D
run_ex_Uniform1D
```
`Ex_Uniform1D` imprime uma tabela com cinco colunas:
```text
i
xface[i]
xcenter[i]
dxface[i]
dxcenter[i]
```
A última linha corresponde à face direita, portanto não possui `xcenter[i]` nem `dxface[i]`, mas possui `dxcenter[i]`.
7. Regras de implementação
```text
1. Código, comentários e API em inglês.
2. Respostas e discussão podem permanecer em português.
3. Evitar enum em categorias extensíveis.
4. Cada classe deve expor ID próprio quando puder ser origem de erro.
5. require() deve receber ErrorDescriptor + ID sempre que possível.
6. Axis1D não deve decidir regras de reconstrução específicas de padrões.
7. GridPattern1D deve conter regras de reconstrução próprias do padrão.
8. Axis1D deve armazenar geometria completa e calcular métricas gerais.
9. Exemplos públicos devem ter alvo run_ex_*.
10. Testes devem ter alvo run_tst_*.
```