# FVGridMaker - Requisitos Funcionais e Não Funcionais

Este documento consolida os requisitos atuais do FVGridMaker, restritos à geração, armazenamento, validação, inspeção, operação geométrica e exportação básica de grids estruturados.

O FVGridMaker deve ser uma biblioteca independente para construção de grids estruturados compostos por eixos unidimensionais.

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
operações geométricas sobre eixos 1D;
outputs de conferência;
exportação CSV para eixos 1D;
exportação VTK 2D em caráter experimental;
grids estruturados 2D em caráter experimental;
sistemas de coordenadas 2D em caráter experimental.
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

## 2. Classificação da API

A versão atual deve classificar seus módulos em quatro grupos:

```text
Stable Public API;
Experimental Public API;
Internal;
Future.
```

### 2.1. Stable Public API

Os módulos estáveis são:

```text
Core;
ErrorHandling;
Axis1D;
GridPattern1D;
Uniform1D;
Random1D;
Roberts1D;
Custom1D;
Operations1D;
Axis1DCSVWriter.
```

Esses módulos compõem a API principal da versão atual.

### 2.2. Experimental Public API

Os módulos experimentais são:

```text
CoordinateSystem2D;
CoordinateMetrics2D;
CoordinateMappingFactory2D;
StructuredGrid2D;
LegacyVTKRectilinearGrid2DWriter.
```

Um módulo experimental pode ser exposto pelo header público e usado por exemplos, mas sua API ainda pode mudar antes da estabilização.

### 2.3. Future

Funcionalidades futuras incluem:

```text
grids estruturados 3D;
configuração externa por YAML em exemplos;
exportação binária;
novos sistemas de coordenadas;
novos formatos de saída;
testes de instalação e consumo externo;
empacotamento de release.
```

### 2.4. Internal

São internos todos os detalhes auxiliares que não devem ser usados diretamente por usuários externos e que podem mudar sem aviso dentro da mesma série de versão.

## 3. Requisitos de versionamento

A versão semântica da biblioteca deve ser definida em um único ponto canônico:

```text
CMakeLists.txt
```

A definição deve ser feita por meio de:

```cmake
project(FVGridMaker
    VERSION X.Y.Z
    LANGUAGES CXX
)
```

Na versão atual, a versão semântica é:

```text
0.2.0
```

`Core/Version.h` deve expor a versão pública a partir das informações geradas pelo CMake.

O header gerado pelo CMake deve ficar em:

```text
<build-dir>/generated/FVGridMaker/version.hpp
```

Esse header gerado deve conter:

```text
versão major;
versão minor;
versão patch;
versão tweak;
hash Git;
branch Git;
estado dirty;
timestamp de build;
data de build;
versão semântica;
versão completa.
```

A API pública de versionamento deve expor pelo menos:

```text
version_major();
version_minor();
version_patch();
version_string();
full_version_string();
git_hash();
git_branch();
build_timestamp();
build_date();
is_development_build();
is_tagged_release();
```

`version_string()` deve retornar apenas a versão semântica.

Exemplo:

```text
0.2.0
```

`full_version_string()` pode retornar a versão com metadados Git.

Exemplo:

```text
0.2.0+48fa0bd9e0c9-dirty
```

O sufixo `-dirty` indica que havia alterações locais não commitadas quando o CMake configurou a build.

Comentários de cabeçalho de arquivos não devem duplicar número de versão. Quando houver campo `Version`, ele deve usar:

```text
Version: see <FVGridMaker/Core/Version.h>
```

Não usar em cabeçalhos de arquivos:

```text
Version: 0.1.0
Version: 0.2.0
Version: 1.0.0
```

Motivo: o número da versão pertence à configuração central do projeto, não a comentários repetidos em dezenas de arquivos.

## 4. Vocabulário obrigatório

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

## 5. Requisitos de arquitetura

```text
C++20;
Data-Oriented Design onde houver dados geométricos;
armazenamento contíguo em std::vector;
exposição de leitura via std::span;
sem classes virtuais no core;
sem runtime polymorphism em hot paths;
sem enum para categorias extensíveis;
sem enum central para erros;
sem enum central para IDs de classe;
sem dependência do core em YAML, output, testes ou documentação;
comentários e API em inglês;
respostas e discussão em português são aceitáveis fora do código.
```

Observação sobre extensão de comportamento:

```text
Quando for necessário variar comportamento, usar preferencialmente:
concepts;
traits;
templates;
factories;
funções livres;
classes final pequenas;
composição por valor.
```

Observação sobre `enum`:

```text
Enums não devem ser usados para categorias extensíveis.

O uso de enum é aceitável para escolhas estruturais fechadas e pequenas,
desde que elas não representem famílias extensíveis. O caso atual é
CoordinateKind1D, que representa somente se as coordenadas primárias são
faces ou centers.
```

## 6. Identidade de classes

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

## 7. Sistema de erros

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

Os códigos YAML eventualmente existentes no sistema de erro devem ser tratados como candidatos à remoção ou restrição a exemplos antes de estabilizar a API pública. A biblioteca não deve depender desses códigos para funcionalidade interna.

## 8. Requisitos de `Axis1D`

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
dx_faces[i]       = xface[i + 1] - xface[i]
dx_centers[0]     = xcenter[0] - xface[0]
dx_centers[i]     = xcenter[i] - xcenter[i - 1]
dx_centers[nvol]  = xface[nvol] - xcenter[nvol - 1]
```

Essas métricas são geométricas e valem para qualquer padrão de grid quando `faces` e `centers` já existem.

`Axis1D` não deve conter regra específica de reconstrução de padrão. Ele deve receber geometria completa já reconstruída por uma distribuição ou por um `GridPattern1D`.

Ponto a decidir no Gate 5:

```text
Definir formalmente se pattern_name representa o padrão geométrico original
ou a procedência da operação que gerou o eixo.
```

## 9. Requisitos de coordenadas primárias 1D

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

## 10. Requisitos de domínio 1D

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

## 11. Requisitos de `GridPattern1D`

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

### 11.1. `VolumeCentered1D`

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

### 11.2. `FaceCentered1D`

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

## 12. Requisitos de `Uniform1D`

`Uniform1D` deve gerar coordenadas primárias uniformes de acordo com o padrão selecionado.

Entrada:

```text
NVol
Length
XInit
```

Regra:

```text
Se o padrão aceita Faces:
  Uniform1D gera faces uniformes.

Se o padrão aceita Centers:
  Uniform1D gera centers uniformes dentro do domínio.
```

A chamada sem padrão deve continuar funcionando como atalho para `VolumeCentered1D`.

Fluxo:

```text
1. Uniform1D calcula as coordenadas primárias.
2. O padrão 1D calcula as coordenadas secundárias.
3. Axis1D recebe faces + centers.
4. Axis1D valida a geometria.
5. Axis1D calcula dx_faces e dx_centers.
```

## 13. Requisitos de `Random1D`

`Random1D` deve gerar coordenadas primárias aleatórias de acordo com o padrão selecionado.

Entrada:

```text
NVol
Length
XInit
Seed
MinSpacing opcional
Pattern opcional
```

Regras:

```text
A geração deve ser reprodutível por Seed.
A geração deve respeitar MinSpacing quando fornecido.
MinSpacing deve ser não negativo.
A soma dos espaçamentos deve reproduzir o comprimento do domínio.
A chamada sem padrão deve usar VolumeCentered1D.
```

Observação conceitual:

```text
Random1D gera uma partição aleatória do domínio por espaçamentos positivos
normalizados. Essa estratégia não precisa ser idêntica ao procedimento de
sortear faces internas independentes e depois ordená-las.
```

## 14. Requisitos de `Roberts1D`

`Roberts1D` deve gerar eixos 1D com concentração simétrica controlada por `Beta`.

Entrada:

```text
NVol
Length
XInit
Beta
Pattern opcional
```

Regras:

```text
NVol deve ser positivo.
Length deve ser positivo.
Beta deve ser maior que 1.
A chamada sem padrão deve usar VolumeCentered1D.
A geração deve respeitar o domínio [XInit, XInit + Length].
A geração deve aceitar padrões compatíveis por template.
```

`Roberts1D` deve permanecer independente de herança virtual.

## 15. Requisitos de `Custom1D`

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

## 16. Requisitos de `Operations1D`

`Operations1D` deve operar sobre eixos 1D já construídos e validados.

Funções esperadas:

```text
validate_tolerance();
same_pattern();
require_same_pattern();
domain_interval();
intersection();
require_interval_intersection();
unique_sorted_coordinates();
clip_faces_to_interval();
```

Regras:

```text
Operations1D não deve gerar distribuições primárias.
Operations1D não deve modificar eixos existentes.
Operations1D pode retornar novos Axis1D quando a operação criar nova geometria.
Operations1D deve validar tolerâncias.
Operations1D deve rejeitar interseções vazias quando a operação exigir intervalo não vazio.
```

Ponto a decidir no Gate 8:

```text
Decidir se clip_faces_to_interval() preserva pattern_name original ou usa
um pattern_name próprio de operação.
```

## 17. Requisitos de output 1D

`Axis1DCSVWriter` deve exportar `Axis1D` para CSV.

Requisitos:

```text
escrever em std::ostream;
escrever em arquivo;
criar diretórios pais quando necessário;
usar precisão suficiente para double;
emitir erro claro em falha de abertura;
emitir erro claro em falha de escrita.
```

O formato CSV deve ser documentado no README ou na documentação técnica.

## 18. Requisitos experimentais de 2D

O módulo 2D deve ser tratado como experimental enquanto sua API não for congelada.

`StructuredGrid2D` deve compor dois `Axis1D` independentes.

Requisitos mínimos:

```text
armazenar primeiro eixo;
armazenar segundo eixo;
fornecer número de células em cada direção;
fornecer número total de células;
fornecer número de faces em cada direção;
fornecer indexação linear;
fornecer coordenadas de faces;
fornecer coordenadas de centros;
fornecer medidas computacionais;
fornecer medidas físicas por traits de coordenadas;
validar índices de consulta.
```

A extensão para diferentes sistemas de coordenadas deve usar:

```text
traits;
concepts;
templates;
factories;
composição por valor.
```

Não usar:

```text
classe base virtual para sistemas de coordenadas;
ponteiro polimórfico obrigatório;
estado global de sistema de coordenadas.
```

## 19. Requisitos experimentais de sistemas de coordenadas 2D

Sistemas de coordenadas 2D devem fornecer, por traits ou objetos concretos:

```text
name();
first_coordinate_name();
second_coordinate_name();
vtk_rectilinear();
map(first, second);
cell_measure(cell);
```

Métricas adicionais podem ser fornecidas por funções especializadas ou fallback geométrico.

Sistemas atualmente previstos ou experimentais:

```text
CartesianCoordinates2D;
PolarCoordinates2D;
AxisymmetricCoordinates2D;
SphericalAxisymmetricCoordinates2D;
FunctionalCoordinateMapping2D.
```

Antes de estabilizar o módulo, devem ser revisados:

```text
nomes físicos das coordenadas;
validade dos domínios;
positividade das medidas;
compatibilidade com VTK;
precisão das métricas de face;
testes analíticos.
```

## 20. Requisitos experimentais de output 2D

O writer VTK 2D deve permanecer experimental até que seu contrato seja fechado.

Antes de estabilizar:

```text
decidir se o writer exporta apenas VTK RectilinearGrid;
decidir se grids não retangulares devem ser rejeitados;
testar dimensões;
testar número de pontos;
testar número de células;
testar arquivo mínimo legível;
documentar limitações.
```

O nome do writer deve corresponder exatamente ao formato exportado.

## 21. Requisitos de exemplos

Cada funcionalidade pública estável deve ter exemplo executável.

Exemplos obrigatórios para API estável:

```text
Minimal;
ErrorHandling;
Axis1D;
Uniform1D;
Random1D;
Roberts1D;
Custom1D;
Operations1D;
Axis1DCSVWriter.
```

Exemplos recomendados para API experimental:

```text
StructuredGrid2D;
CoordinateSystems2D;
VTK2D.
```

Exemplos futuros podem usar YAML, desde que YAML fique restrito ao diretório `examples/` e não entre em `FVGridMakerLib`.

Regra de impressão para eixos 1D:

```text
linhas 0..nvol-1 têm xface, xcenter, dxface, dxcenter;
linha nvol tem xface e dxcenter;
linha nvol não tem xcenter nem dxface.
```

## 22. Requisitos de testes

Toda classe nova deve ter GoogleTest.

Testes obrigatórios para API estável:

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
tst_Random1D
tst_Roberts1D
tst_Custom1D
tst_Operations1D
tst_Axis1DCSVWriter
```

Testes recomendados para API experimental:

```text
tst_StructuredGrid2D
tst_CoordinateSystem2D
tst_CoordinateMetrics2D
tst_LegacyVTKRectilinearGrid2DWriter
```

Todo bloco deve ser fechado com:

```bash
cmake -S . -B build-tests -DBUILD_TESTS=ON -DBUILD_EXAMPLES=OFF
cmake --build build-tests
ctest --test-dir build-tests --output-on-failure
```

E, quando houver exemplo público:

```bash
cmake -S . -B build-examples -DBUILD_EXAMPLES=ON -DBUILD_TESTS=OFF
cmake --build build-examples
```

Se os targets agregados estiverem disponíveis:

```bash
cmake --build build-tests --target run_all_tests
cmake --build build-examples --target run_all_examples
```

## 23. Definition of Done do Gate 1

O Gate 1 estará fechado quando:

```text
FVGridMaker.h separar claramente API estável e API experimental;
README.md listar a mesma classificação;
FVGridMaker_requisitos.md listar a mesma classificação;
nenhum módulo experimental for apresentado como estável;
YAML continuar fora de FVGridMakerLib;
2D e VTK permanecerem experimentais até revisão específica;
o roadmap conter apenas itens ainda não estabilizados;
nenhuma classe virtual tiver sido introduzida;
nenhuma dependência externa tiver sido adicionada ao core;
os comandos de build e teste passarem localmente.
```

## 24. Definition of Done do Gate 2

O Gate 2 estará fechado quando:

```text
CMakeLists.txt definir a versão semântica canônica da biblioteca;
a versão semântica atual for 0.2.0;
cmake/templates/version.hpp.in gerar metadados de versão no namespace fvgrid::build_info;
FVGridMaker/Core/Version.h consumir o header gerado por CMake;
Version.h não duplicar manualmente a versão semântica;
version_string() retornar 0.2.0;
full_version_string() retornar a versão completa com metadados quando disponíveis;
version() continuar retornando a versão semântica para compatibilidade;
tests/Core/tst_Version.cc testar a versão 0.2.0;
git grep não encontrar Version: 0.1.0;
git grep não encontrar Version: 1.0.0;
git grep não encontrar 1.0.0 fora de contexto aceitável;
exemplos compilarem;
testes compilarem;
ctest passar.
```

Comandos mínimos de validação do Gate 2:

```bash
git grep -n "Version: 0.1.0"
git grep -n "Version: 1.0.0"
git grep -n "1.0.0"

rm -rf build-tests build-examples

cmake -S . -B build-tests -DBUILD_TESTS=ON -DBUILD_EXAMPLES=OFF
cmake --build build-tests
ctest --test-dir build-tests --output-on-failure
cmake --build build-tests --target run_all_tests

cmake -S . -B build-examples -DBUILD_EXAMPLES=ON -DBUILD_TESTS=OFF
cmake --build build-examples
cmake --build build-examples --target run_all_examples
```
