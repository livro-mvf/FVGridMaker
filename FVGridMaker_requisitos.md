FVGridMaker - Requisitos Funcionais e Não Funcionais

Este documento consolida os requisitos atuais do FVGridMaker, restritos à geração, armazenamento, validação, inspeção e exportação de grids estruturados.

O FVGridMaker deve ser uma biblioteca independente para construção de grades estruturadas compostas por eixos unidimensionais.

## 1. Escopo

O FVGridMaker deve tratar de:

```text
grids estruturados;
eixos 1D;
composição multidimensional por eixos 1D;
faces;
centros de volumes;
spacings entre faces;
spacings entre centros;
validação geométrica;
reconstrução de coordenadas secundárias a partir de coordenadas primárias;
construção customizada de eixos 1D;
outputs de conferência;
exportação CSV futura;
exportação VTK Rectilinear Grid futura.
```

O FVGridMaker não deve tratar de:

```text
equações diferenciais;
campos físicos;
condições de contorno físicas;
difusão;
advecção;
termos fonte;
montagem de matrizes;
solvers lineares;
integração temporal;
malhas Voronoi;
Delaunay;
CGAL;
malhas não estruturadas;
leitura YAML interna à biblioteca;
dependência de yaml-cpp na biblioteca.
```

Observação sobre YAML:

```text
YAML pode ser usado em exemplos ou aplicações externas.

A biblioteca FVGridMakerLib não deve ter módulo YAML.
A biblioteca FVGridMakerLib não deve depender de yaml-cpp.
A biblioteca FVGridMakerLib não deve expor headers YAML.
```

## 2. Vocabulário obrigatório

Usar preferencialmente:

```text
Grid
Axis
Axis1D
StructuredGrid
Distribution
GridPattern
CoordinateSystem
Operations
Output
```

Evitar em classes principais:

```text
Mesh
AxisGrid1D
```

Observação: documentos antigos podiam usar `AxisGrid1D`; a nomenclatura atual da unidade 1D é `Axis1D`.

## 3. Requisitos de arquitetura

```text
C++20;
Data-Oriented Design onde houver dados geométricos;
armazenamento contíguo em std::vector;
exposição de leitura via std::span;
sem runtime polymorphism em hot paths;
sem enum para categorias extensíveis;
sem enum central para erros;
sem enum central para IDs de classe;
sem dependência do core em YAML, output, testes ou documentação;
comentários e API em inglês;
respostas e discussão em português são aceitáveis fora do código.
```

Observação sobre `enum`:

```text
Enums não devem ser usados para categorias extensíveis.

O uso de enum é aceitável para escolhas estruturais fechadas e pequenas,
desde que elas não representem famílias extensíveis. O caso atual é
CoordinateKind1D, que representa somente se as coordenadas primárias são
faces ou centers.
```

## 4. Identidade de classes

Toda classe ou componente nomeado que possa ser origem de erro deve expor identidade própria.

Formato:

```text
module
class_name
class_id
```

A classe `ID` é um valor imutável.

Exemplo conceitual:

```cpp
[[nodiscard]] static constexpr ID id() noexcept {
    return ID{
        "OneDimensional",
        "Axis1D",
        "fvgrid.1d.axis.Axis1D"
    };
}
```

Não usar:

```text
enum de classes;
herança virtual obrigatória para identificação;
macros do tipo DefineIdentity.
```

## 5. Sistema de erros

O sistema de erros deve ser textual, extensível e sem `enum`.

Componentes obrigatórios:

```text
ErrorCodes       -> códigos textuais estáveis;
ErrorDescriptor  -> code + message + category;
ErrorCatalog     -> catálogo interno de erros FVGridMaker;
ErrorRecord      -> registro completo;
FVGridException  -> exceção base;
ThrowError       -> throw_error() e require().
```

Uso interno preferencial:

```cpp
require(
    condition,
    error_catalog::kInvalidNVol,
    Uniform1D::id()
);
```

Uso externo permitido:

```cpp
require(
    condition,
    user_descriptor,
    UserClass::id()
);
```

ou, para mensagem contextual:

```cpp
require(
    condition,
    "USER.CODE.INVALID_VALUE",
    "Context-specific diagnostic message.",
    "UserCategory",
    UserClass::id()
);
```

Erro obrigatório para incompatibilidade entre coordenada primária e padrão:

```text
FVGRID.GRID.INVALID_COORDINATE_KIND
```

Esse erro deve ser usado quando `Coordinates1D::kind()` não coincide com o `input_kind()` declarado pelo padrão selecionado.

Os códigos YAML atualmente existentes no sistema de erro devem ser tratados como candidatos à remoção ou restrição a exemplos antes de estabilizar a API pública. A biblioteca não deve depender desses códigos para funcionalidade interna.

## 6. Requisitos de `Axis1D`

`Axis1D` é a unidade geométrica fundamental.

Deve armazenar:

```text
faces       tamanho nvol + 1;
centers     tamanho nvol;
dx_faces    tamanho nvol;
dx_centers  tamanho nvol + 1;
pattern_name.
```

Deve fornecer:

```text
num_cells();
num_faces();
faces();
centers();
dx_faces();
dx_centers();
cell_lengths();
xmin();
xmax();
length();
pattern_name().
```

`cell_lengths()` deve ser alias de leitura para `dx_faces()`.

Métricas gerais:

```text
dxface[i]       = xface[i + 1] - xface[i]
dxcenter[0]     = xcenter[0] - xface[0]
dxcenter[i]     = xcenter[i] - xcenter[i - 1]
dxcenter[nvol]  = xface[nvol] - xcenter[nvol - 1]
```

Essas métricas são geométricas e valem para qualquer padrão de grid quando `faces` e `centers` já existem.

`Axis1D` não deve conter regra específica de reconstrução de padrão. Ele deve receber geometria completa já reconstruída por uma distribuição ou por um `GridPattern1D`.

## 7. Requisitos de coordenadas primárias 1D

A entrada customizada de coordenadas deve ser explícita.

O tipo estrutural das coordenadas primárias deve ser representado por:

```text
CoordinateKind1D
```

Valores permitidos:

```text
Faces
Centers
```

A entrada pública deve ser encapsulada em:

```text
Coordinates1D
```

Construtores/fábricas obrigatórios:

```text
Coordinates1D::faces(std::vector<Real>);
Coordinates1D::centers(std::vector<Real>);
```

`Coordinates1D` deve fornecer:

```text
kind();
values();
release_values();
```

Regra:

```text
O usuário deve sempre informar se os dados primários são faces ou centers.
Um padrão 1D deve aceitar exatamente um tipo de coordenada primária.
```

## 8. Requisitos de domínio 1D

O domínio físico 1D, quando necessário, deve ser representado por:

```text
Domain1D
```

`Domain1D` deve fornecer:

```text
Domain1D::none();
Domain1D::from_length(XInit, Length);
Domain1D::from_bounds(XInit, XFinal);
has_bounds();
xmin();
xmax();
length();
```

`Domain1D` deve ser leve, trivial de copiar e sem alocação dinâmica.

O domínio é obrigatório para reconstruções que precisam de fronteiras externas, como `FaceCentered1D`.

## 9. Requisitos de `GridPattern1D`

`GridPattern1D` define a interpretação das coordenadas primárias e a reconstrução das secundárias.

Cada padrão deve declarar:

```text
input_kind();
complete_geometry(...);
```

O retorno intermediário de `complete_geometry(...)` deve ser:

```text
AxisGeometry1D
```

`AxisGeometry1D` deve conter:

```text
faces;
centers;
pattern_name.
```

`VolumeCentered1D`

```text
primary_coordinates   = faces
secondary_coordinates = centers
input_kind            = Faces
```

Regra:

```text
centers[i] = 0.5 * (faces[i] + faces[i + 1])
```

A função responsável deve ser:

```text
VolumeCentered1D::centers_from_faces()
```

`VolumeCentered1D::complete_geometry()` deve receber faces e retornar faces + centers.

`FaceCentered1D`

```text
primary_coordinates   = centers
secondary_coordinates = faces
input_kind            = Centers
```

Regra:

```text
face[0]     = x_min
face[nvol]  = x_max
face[i]     = 0.5 * (center[i - 1] + center[i])
```

A função responsável deve ser:

```text
FaceCentered1D::faces_from_centers()
```

`FaceCentered1D::complete_geometry()` deve receber centers e `Domain1D` com fronteiras válidas, e retornar faces + centers.

## 10. Requisitos de `Uniform1D`

A versão atual de `Uniform1D` é volume-centred.

Entrada atual:

```text
NVol
Length
XInit
```

Regra atual:

```text
1. Uniform1D calcula as faces uniformes.
2. VolumeCentered1D calcula os centers a partir das faces.
3. Axis1D recebe faces + centers.
4. Axis1D calcula dx_faces e dx_centers.
```

Etapa futura recomendada:

```text
Uniform1D deve passar a gerar coordenadas primárias de acordo com o padrão.

Se o padrão aceita Faces:
  Uniform1D gera faces uniformes.

Se o padrão aceita Centers:
  Uniform1D gera centers uniformes dentro do domínio.
```

A chamada antiga deve continuar funcionando como atalho para `VolumeCentered1D`.

## 11. Requisitos de `Custom1D`

`Custom1D` deve construir `Axis1D` a partir de coordenadas primárias fornecidas pelo usuário.

Assinatura conceitual:

```cpp
Custom1D::make(
    Coordinates1D coordinates,
    Pattern pattern,
    Domain1D domain = Domain1D::none()
);
```

Regras:

```text
Custom1D não deve depender de padrões concretos.
Custom1D deve verificar coordinates.kind() contra Pattern::input_kind().
Custom1D deve rejeitar combinações incompatíveis.
Custom1D deve chamar Pattern::complete_geometry().
Custom1D deve construir Axis1D a partir de AxisGeometry1D.
```

Casos implementados:

```text
Coordinates1D::faces(...)   + VolumeCentered1D -> centers reconstruídos;
Coordinates1D::centers(...) + FaceCentered1D   -> faces reconstruídas.
```

Casos rejeitados:

```text
Coordinates1D::centers(...) + VolumeCentered1D;
Coordinates1D::faces(...)   + FaceCentered1D.
```

## 12. Requisitos de exemplos

Cada funcionalidade pública deve ter exemplo executável.

Exemplos atuais:

```text
run_ex_Minimal
run_ex_ErrorHandling
run_ex_Axis1D
run_ex_Uniform1D
run_ex_Custom1D
```

`Ex_Uniform1D` deve demonstrar uma malha uniforme 1D por tabela com:

```text
i
xface[i]
xcenter[i]
dxface[i]
dxcenter[i]
```

`Ex_Custom1D` deve demonstrar:

```text
construção volume-centred a partir de faces;
construção face-centred a partir de centers e domínio.
```

Exemplos futuros podem usar YAML, desde que YAML fique restrito ao diretório `examples/` e não entre em `FVGridMakerLib`.

Regra de impressão:

```text
linhas 0..nvol-1 têm xface, xcenter, dxface, dxcenter;
linha nvol tem xface e dxcenter;
linha nvol não tem xcenter nem dxface.
```

## 13. Requisitos de testes

Toda classe nova deve ter GoogleTest.

Testes atuais obrigatórios:

```text
tst_ID
tst_Types
tst_StrongTypes
tst_Version
tst_ErrorCodes
tst_ErrorCatalog
tst_ErrorRecord
tst_FVGridException
tst_ThrowError
tst_Coordinates1D
tst_Domain1D
tst_GridPattern1D
tst_Axis1D
tst_Uniform1D
tst_Custom1D
```

Todo bloco deve ser fechado com:

```bash
make
ctest --output-on-failure
```

E, quando houver exemplo público:

```bash
make run_ex_<Name>
```

Para o Bloco 7, o gate aprovado foi:

```text
run_tst_StrongTypes
run_tst_ErrorCodes
run_tst_ErrorCatalog
run_tst_Coordinates1D
run_tst_Domain1D
run_tst_GridPattern1D
run_tst_Custom1D
run_ex_Custom1D
run_all_tests
run_all_examples
```