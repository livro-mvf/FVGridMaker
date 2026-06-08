FVGridMaker — Requisitos Funcionais e Não Funcionais
Este documento consolida os requisitos atuais do FVGridMaker, restritos à geração, armazenamento, validação, inspeção e exportação de grids estruturados.
O FVGridMaker deve ser uma biblioteca independente para construção de grades estruturadas compostas por eixos unidimensionais.
1. Escopo
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
outputs de conferência;
exportação CSV futura;
exportação VTK Rectilinear Grid futura;
leitura opcional de configuração via YAML futura.
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
malhas não estruturadas.
```
2. Vocabulário obrigatório
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
3. Requisitos de arquitetura
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
4. Identidade de classes
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
5. Sistema de erros
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
6. Requisitos de `Axis1D`
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
`Axis1D` não deve conter regra específica de reconstrução de padrão. Ele deve receber geometria completa ou usar um padrão externo para reconstruir a parte faltante.
7. Requisitos de `GridPattern1D`
`GridPattern1D` define a interpretação das coordenadas primárias e a reconstrução das secundárias.
`VolumeCentered1D`
```text
primary_coordinates   = faces
secondary_coordinates = centers
```
Regra:
```text
centers[i] = 0.5 * (faces[i] + faces[i + 1])
```
A função responsável deve ser `VolumeCentered1D::centers_from_faces()`.
`FaceCentered1D`
```text
primary_coordinates   = centers
secondary_coordinates = faces
```
Regra prevista:
```text
face[0]     = x_min
face[nvol]  = x_max
face[i]     = 0.5 * (center[i - 1] + center[i])
```
A função responsável deve ser `FaceCentered1D::faces_from_centers()` quando esse caminho for ativado.
8. Requisitos de `Uniform1D`
A versão atual de `Uniform1D` deve ser volume-centred.
Entrada:
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
`Uniform1D` não deve misturar o caminho face-centred nesta fase.
A versão face-centred deve entrar em bloco próprio, com testes específicos.
9. Requisitos de exemplos
Cada funcionalidade pública deve ter exemplo executável.
Exemplos atuais:
```text
run_ex_Minimal
run_ex_ErrorHandling
run_ex_Axis1D
run_ex_Uniform1D
```
`Ex_Uniform1D` deve demonstrar uma malha uniforme 1D por tabela com:
```text
i
xface[i]
xcenter[i]
dxface[i]
dxcenter[i]
```
Regra de impressão:
```text
linhas 0..nvol-1 têm xface, xcenter, dxface, dxcenter;
linha nvol tem xface e dxcenter;
linha nvol não tem xcenter nem dxface.
```
10. Requisitos de testes
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
tst_GridPattern1D
tst_Axis1D
tst_Uniform1D
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