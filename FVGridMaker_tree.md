FVGridMaker - Tree do Projeto

Este documento define a árvore de referência do FVGridMaker no estado atual do desenvolvimento. Ele substitui versões anteriores que ainda usavam `AxisGrid1D` como nome principal. A unidade geométrica 1D atualmente adotada é `Axis1D`.

## 1. Princípios da árvore

A organização do projeto deve refletir estas decisões:

```text
1. O projeto trabalha com grids estruturados, não meshes não estruturadas.
2. A unidade fundamental 1D é Axis1D.
3. Axis1D armazena geometria completa: faces, centers, dx_faces e dx_centers.
4. Uma grade 2D será composição de dois Axis1D.
5. Distribution1D gera ou organiza coordenadas primárias segundo uma distribuição.
6. GridPattern1D define como coordenadas secundárias são reconstruídas.
7. Cada GridPattern1D declara exatamente um tipo de coordenada primária.
8. Axis1D calcula métricas geométricas gerais depois que faces e centers existem.
9. Operations1D contém operações entre eixos/grades já construídos.
10. CoordinateSystem interpreta os eixos e calcula medidas geométricas físicas.
11. Output1D e Output2D são periféricos.
12. YAML é permitido apenas em exemplos ou aplicações externas.
13. GoogleTest aparece apenas nos testes.
14. Nenhum módulo periférico deve criar dependência reversa no núcleo.
15. Nenhuma categoria extensível deve ser implementada com enum.
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

## 2. Tree atual/recomendada

```text
FVGridMaker/
├── CMakeLists.txt
├── README.md
├── LICENSE.md
├── FVGridMaker_cronograma_por_gates.md
├── FVGridMaker_requisitos.md
├── FVGridMaker_tree.md
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
│   │       │   │   ├── Custom1D.h
│   │       │   │   └── Uniform1D.h
│   │       │   │
│   │       │   ├── GridPattern1D/
│   │       │   │   ├── AxisGeometry1D.h
│   │       │   │   ├── CoordinateKind1D.h
│   │       │   │   ├── Coordinates1D.h
│   │       │   │   ├── Domain1D.h
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
│   │       └── TwoDimensional/
│   │           ├── Grid2D/
│   │           └── Output2D/
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
│               │   ├── Custom1D.cc
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
│   ├── OneDimensional/
│   │   ├── Axis1D/
│   │   │   └── ex_Axis1D.cc
│   │   │
│   │   └── Distribution1D/
│   │       ├── ex_Custom1D.cc
│   │       └── ex_Uniform1D.cc
│   │
│   └── YAML/
│       └── <future external examples only>
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
│       │   ├── tst_Custom1D.cc
│       │   └── tst_Uniform1D.cc
│       │
│       └── GridPattern1D/
│           ├── tst_Coordinates1D.cc
│           ├── tst_Domain1D.cc
│           └── tst_GridPattern1D.cc
│
└── FVGridMakerLibOld/
    └── <legacy implementation, excluded from the current build>
```

## 3. Core

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

Define tipos fortes para parâmetros de geração e domínio:

```text
NVol
Length
XInit
XFinal
Seed
```

Esses tipos evitam troca acidental de argumentos em APIs como:

```cpp
Uniform1D::make(NVol, Length, XInit)
```

e em descrições de domínio como:

```cpp
Domain1D::from_bounds(XInit, XFinal)
```

`Core/ID.h`

Define identidade imutável de classe/componente:

```text
module
class_name
class_id
```

Regra atual:

```text
Toda classe do FVGridMaker deve ter identidade própria quando puder ser
origem de erro.
Nem toda função precisa de identidade própria.
Função/método específico é rastreado por std::source_location.
```

## 4. ErrorHandling

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

Erro adicionado para validação de `Custom1D`:

```text
FVGRID.GRID.INVALID_COORDINATE_KIND
```

Esse erro é usado quando o tipo de coordenada primária fornecido não coincide com o `input_kind()` declarado pelo padrão.

Observação sobre YAML:

```text
Códigos de erro YAML não devem induzir a criação de módulo YAML dentro da biblioteca.
Se mantidos, devem ser tratados como suporte a exemplos ou candidatos à remoção antes da estabilização da API.
```

## 5. OneDimensional

`Axis1D`

`Axis1D` é a unidade geométrica 1D fundamental.

Armazena:

```text
faces       -> xface[i],    tamanho nvol + 1
centers     -> xcenter[i],  tamanho nvol
dx_faces    -> dxface[i],   tamanho nvol
dx_centers  -> dxcenter[i], tamanho nvol + 1
pattern_name
```

Define métricas gerais:

```text
dxface[i]       = xface[i + 1] - xface[i]
dxcenter[0]     = xcenter[0] - xface[0]
dxcenter[i]     = xcenter[i] - xcenter[i - 1], 1 <= i < nvol
dxcenter[nvol]  = xface[nvol] - xcenter[nvol - 1]
```

Essas definições são geométricas e valem para qualquer padrão de grid desde que `faces` e `centers` estejam consistentes.

`GridPattern1D`

`GridPattern1D` define como reconstruir coordenadas secundárias a partir de coordenadas primárias.

Arquivos auxiliares:

```text
CoordinateKind1D.h  -> Faces ou Centers
Coordinates1D.h     -> entrada tipada do usuário
Domain1D.h          -> domínio físico opcional/obrigatório conforme padrão
AxisGeometry1D.h    -> geometria completa intermediária
```

Atualmente:

```text
VolumeCentered1D:
  coordenadas primárias: faces
  coordenadas secundárias: centers
  input_kind: Faces
  regra: centers_from_faces(faces)

FaceCentered1D:
  coordenadas primárias: centers
  coordenadas secundárias: faces
  input_kind: Centers
  regra: faces_from_centers(centers, x_min, x_max)
```

Cada padrão deve declarar:

```text
input_kind();
complete_geometry(...);
```

`FaceCentered1D` já contém a regra de reconstrução por centers e domínio físico. O `Uniform1D` atual permanece deliberadamente restrito ao caminho volume-centred.

`Distribution1D/Uniform1D`

`Uniform1D` atual é volume-centred.

Regra implementada:

```text
1. Uniform1D calcula faces uniformes a partir de NVol, Length e XInit.
2. VolumeCentered1D calcula centers a partir das faces.
3. Axis1D recebe faces + centers e calcula dx_faces + dx_centers.
```

A versão pattern-aware de `Uniform1D` deve entrar no próximo bloco.

`Distribution1D/Custom1D`

`Custom1D` constrói `Axis1D` a partir de coordenadas primárias fornecidas pelo usuário.

Regras:

```text
1. O usuário informa se os dados são faces ou centers via Coordinates1D.
2. O padrão declara qual tipo de entrada aceita via input_kind().
3. Custom1D valida a compatibilidade.
4. O padrão completa a geometria via complete_geometry().
5. Axis1D recebe faces + centers e calcula as métricas.
```

Casos implementados:

```text
Coordinates1D::faces(...)   + VolumeCentered1D
Coordinates1D::centers(...) + FaceCentered1D + Domain1D
```

Casos rejeitados:

```text
Coordinates1D::centers(...) + VolumeCentered1D
Coordinates1D::faces(...)   + FaceCentered1D
```

## 6. Examples

Exemplos públicos atuais:

```text
run_ex_Minimal
run_ex_ErrorHandling
run_ex_Axis1D
run_ex_Uniform1D
run_ex_Custom1D
```

`Ex_Uniform1D` imprime uma tabela com cinco colunas:

```text
i
xface[i]
xcenter[i]
dxface[i]
dxcenter[i]
```

`Ex_Custom1D` demonstra:

```text
construção volume-centred a partir de faces;
construção face-centred a partir de centers e domínio físico.
```

YAML deve aparecer apenas em exemplos externos futuros:

```text
examples/YAML/
```

Esses exemplos poderão depender de `yaml-cpp`, mas essa dependência não deve chegar ao alvo `FVGridMaker`.

## 7. Tests

Testes atuais:

```text
run_tst_ID
run_tst_Types
run_tst_StrongTypes
run_tst_Version
run_tst_ErrorCodes
run_tst_ErrorCatalog
run_tst_ErrorRecord
run_tst_FVGridException
run_tst_ThrowError
run_tst_Coordinates1D
run_tst_Domain1D
run_tst_GridPattern1D
run_tst_Axis1D
run_tst_Uniform1D
run_tst_Custom1D
```

## 8. Regras de implementação

```text
1. Código, comentários e API em inglês.
2. Respostas e discussão podem permanecer em português.
3. Evitar enum em categorias extensíveis.
4. Enum pequeno é aceitável para propriedade estrutural fechada.
5. Cada classe deve expor ID próprio quando puder ser origem de erro.
6. require() deve receber ErrorDescriptor + ID sempre que possível.
7. Axis1D não deve decidir regras de reconstrução específicas de padrões.
8. GridPattern1D deve conter regras de reconstrução próprias do padrão.
9. Axis1D deve armazenar geometria completa e calcular métricas gerais.
10. Examples públicos devem ter alvo run_ex_*.
11. Tests devem ter alvo run_tst_*.
12. YAML não deve entrar em FVGridMakerLib.
13. YAML deve aparecer apenas em examples/ ou em aplicações externas.
```