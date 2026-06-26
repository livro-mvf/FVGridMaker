# FVGridMaker — Requisitos Funcionais e Não Funcionais

Este documento consolida os requisitos atuais do FVGridMaker, restritos à geração, armazenamento, validação, inspeção, operação geométrica e exportação básica de grids estruturados.

O FVGridMaker é uma biblioteca independente, em C++20, para construção de grids estruturados compostos por eixos unidimensionais. A biblioteca não resolve equações diferenciais, não monta sistemas lineares e não representa campos físicos; ela fornece infraestrutura geométrica, validação, operações e saídas de conferência.

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
A biblioteca FVGridMakerLib não deve depender de códigos YAML no módulo ErrorHandling.
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

### 2.3. Internal

São internos todos os detalhes auxiliares que não devem ser usados diretamente por usuários externos e que podem mudar sem aviso dentro da mesma série de versão.

Exemplos atuais:

```text
helpers em namespace fvgrid::detail;
headers em pastas Detail;
rotinas auxiliares de travessia;
detalhes de validação;
implementações auxiliares de output.
```

### 2.4. Future

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

A arquitetura deve seguir:

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

O sistema de erros deve ser textual, extensível e sem `enum` central.

A arquitetura atual do `ErrorHandling` deve usar descritores tipados por tag. Cada erro interno estável da biblioteca deve ser representado por um tipo leve, normalmente uma `struct final`, com metadados estáticos de código, mensagem e categoria.

Componentes obrigatórios:

```text
BuiltInErrors    -> tags dos erros internos estáveis da FVGridMaker;
ErrorDescriptor  -> code + message + category;
ErrorTraits      -> conceito e extração de descritores a partir de tags;
ErrorContext     -> pares chave/valor para diagnóstico contextual;
ErrorRecord      -> registro completo do erro;
FVGridException  -> exceção base;
ThrowError       -> throw_error() e require(), preferencialmente header-only.
```

Não usar na arquitetura atual:

```text
ErrorCodes.h;
ErrorCatalog.h;
enum central de erros;
namespace error_code;
namespace error_catalog;
catálogo global de descritores.
```

Uso interno preferencial:

```cpp
require<errors::grid::InvalidNVol>(
    condition,
    Uniform1D::id()
);
```

Uso interno com contexto:

```cpp
require<errors::grid::InvalidNVol>(
    condition,
    Uniform1D::id(),
    {
        make_error_context("nvol", std::to_string(nvol.value())),
        make_error_context("expected", "> 0")
    }
);
```

Uso externo permitido com descritor próprio:

```cpp
require(
    condition,
    user_descriptor,
    UserClass::id()
);
```

ou, para mensagem contextual sem tag própria:

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
errors::grid::InvalidCoordinateKind
```

Esse erro deve ser usado quando `Coordinates1D::kind()` não coincide com o `input_kind()` declarado pelo padrão selecionado.

Códigos YAML não pertencem ao núcleo da biblioteca. YAML pode existir em exemplos, aplicações externas ou ferramentas auxiliares, mas a biblioteca `FVGridMakerLib` não deve depender de códigos YAML no módulo `ErrorHandling`.

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

Ponto a decidir em gate futuro:

```text
Definir formalmente se pattern_name representa o padrão geométrico original
ou a procedência da operação que gerou o eixo.
```

## 9. Formatação e serialização de `Axis1D`

A travessia tabular de `Axis1D` deve ser centralizada em helper interno, usado tanto por:

```text
operator<<(std::ostream&, const Axis1D&);
Axis1DCSVWriter::write(...).
```

O helper comum não deve definir o formato final da saída. Cada consumidor continua responsável por seu próprio contrato:

```text
operator<<        -> saída diagnóstica, humana, livre para inspeção;
Axis1DCSVWriter   -> saída CSV tabular, estável, voltada a arquivo e pós-processamento.
```

O helper interno deve ficar em pasta `Detail`, respeitando o padrão de nomes de diretórios do projeto, e pode usar namespace interno minúsculo:

```text
FVGridMakerLib/include/FVGridMaker/OneDimensional/Axis1D/Detail/Axis1DRows.h
namespace fvgrid::detail
```

O contrato de saída deve ser protegido por testes:

```text
operator<< contém cabeçalho humano esperado;
operator<< imprime a última face sem centro e sem comprimento de célula;
Axis1DCSVWriter escreve exatamente o cabeçalho i,xF,xC,Dx,dx;
Axis1DCSVWriter escreve a última linha com campos vazios para xC e Dx.
```

Essa refatoração não deve fazer o CSV chamar `operator<<`, nem fazer `operator<<` chamar o escritor CSV, porque os contratos são diferentes.

## 10. Requisitos de coordenadas primárias 1D

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
```

Regras:

```text
Coordinates1D não deve reconstruir geometria;
Coordinates1D não deve validar monotonicidade completa;
Coordinates1D apenas identifica se a entrada primária são faces ou centers.
```

A reconstrução da geometria completa deve ser responsabilidade do padrão de grid selecionado.

## 11. Requisitos de `GridPattern1D`

Um `GridPattern1D` define como completar a geometria a partir das coordenadas primárias.

Padrões atuais:

```text
VolumeCentered1D;
FaceCentered1D.
```

### 11.1. `VolumeCentered1D`

Entrada primária esperada:

```text
faces
```

Regra:

```text
centers[i] = 0.5 * (faces[i] + faces[i + 1])
```

Validações mínimas:

```text
número de faces >= 2;
faces estritamente crescentes;
faces finitas.
```

### 11.2. `FaceCentered1D`

Entrada primária esperada:

```text
centers
```

Regra para faces internas:

```text
faces[i] = 0.5 * (centers[i - 1] + centers[i])
```

Faces externas exigem domínio com limites explícitos.

Validações mínimas:

```text
número de centers >= 1;
centers estritamente crescentes;
centers finitos;
domínio com xmin e xmax;
xmin < centers.front();
centers.back() < xmax.
```

## 12. Requisitos de domínio 1D

O domínio 1D deve representar limites geométricos quando eles forem necessários para completar a malha.

Requisitos:

```text
armazenar xmin;
armazenar xmax;
validar xmax > xmin quando limites forem usados;
permitir consulta de bounds;
permitir distinguir domínio com bounds de domínio não informado.
```

`FaceCentered1D` deve exigir domínio com bounds.

`VolumeCentered1D` não deve exigir domínio externo quando as faces já são fornecidas.

## 13. Requisitos de distribuições 1D

Distribuições 1D são responsáveis por gerar coordenadas primárias e delegar a reconstrução completa ao padrão selecionado.

Distribuições atuais:

```text
Uniform1D;
Random1D;
Roberts1D;
Custom1D.
```

### 13.1. `Uniform1D`

Requisitos:

```text
receber NVol;
receber Length;
receber XInit;
gerar faces uniformemente espaçadas por padrão;
usar VolumeCentered1D por padrão;
permitir padrão compatível quando aplicável;
validar NVol > 0;
validar Length > 0.
```

### 13.2. `Random1D`

Requisitos:

```text
receber NVol;
receber Length;
receber XInit;
receber espaçamento mínimo opcional;
gerar faces internas aleatórias;
fixar faces externas;
ordenar faces;
validar NVol > 0;
validar Length > 0;
validar min_spacing >= 0;
validar se min_spacing é possível para o domínio.
```

A malha aleatória deve ser útil como teste de estresse geométrico.

### 13.3. `Roberts1D`

Requisitos:

```text
gerar coordenadas por transformação de Roberts;
validar NVol > 0;
validar Length > 0;
validar beta > 1.
```

O erro para `beta` inválido deve pertencer ao domínio de grid.

### 13.4. `Custom1D`

Requisitos:

```text
receber Coordinates1D;
receber GridPattern1D compatível;
validar compatibilidade entre CoordinateKind1D e input_kind() do padrão;
delegar reconstrução ao padrão;
não inferir silenciosamente se a entrada são faces ou centers.
```

Quando houver incompatibilidade entre coordenada primária e padrão, usar:

```text
errors::grid::InvalidCoordinateKind
```

## 14. Requisitos de operações 1D

`Operations1D` deve conter operações geométricas entre eixos 1D.

Operações atuais:

```text
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
tolerância deve ser finita e não negativa;
operações não devem alterar eixos de entrada;
resultados devem ser novos objetos;
interseção vazia deve ser representável;
operações que exigem intervalo não vazio devem lançar erro tipado.
```

Erros esperados:

```text
errors::operation::InvalidTolerance;
errors::operation::IncompatibleGridPatterns;
errors::operation::EmptyGridIntersection.
```

Ponto a decidir em gate futuro:

```text
Definir se clip_faces_to_interval deve preservar pattern_name original,
usar pattern_name específico de operação ou registrar ambas as informações.
```

## 15. Requisitos de output

O módulo de output deve ser separado do core geométrico.

Requisitos gerais:

```text
não introduzir dependência do core em output;
não alterar objetos geométricos durante escrita;
usar std::ostream quando possível;
ter overload para arquivo quando aplicável;
criar diretórios pais quando necessário;
lançar erro tipado em falha de abertura;
lançar erro tipado em falha de escrita.
```

Erros esperados:

```text
errors::output::FileOpenFailed;
errors::output::FileWriteFailed.
```

### 15.1. CSV 1D

`Axis1DCSVWriter` deve escrever saída tabular estável.

Cabeçalho obrigatório:

```text
i,xF,xC,Dx,dx
```

Interpretação:

```text
i   -> índice da linha;
xF  -> coordenada da face;
xC  -> coordenada do centro, quando houver;
Dx  -> comprimento do volume, quando houver;
dx  -> distância associada aos centros.
```

A última linha representa a última face. Portanto, ela não possui centro nem comprimento de célula.

Exemplo conceitual para duas células:

```text
0,0,0.5,1,0.5
1,1,2,2,1.5
2,3,,,1
```

### 15.2. Saída humana de `Axis1D`

`operator<<(std::ostream&, const Axis1D&)` deve produzir saída diagnóstica legível.

Deve conter:

```text
nome Axis1D;
pattern_name;
número de faces;
número de células;
xmin;
xmax;
length;
tabela com xface[i], xcenter[i], dxface[i], dxcenter[i].
```

A última face deve ser impressa sem centro e sem comprimento de célula.

Essa saída é diagnóstica e não deve ser usada como formato estável de dados.

### 15.3. VTK 2D experimental

`LegacyVTKRectilinearGrid2DWriter` é experimental.

Requisitos:

```text
escrever VTK Legacy ASCII;
usar DATASET RECTILINEAR_GRID quando o sistema permitir;
usar DATASET STRUCTURED_GRID quando necessário;
escrever coordenadas de pontos;
escrever cell_area como CELL_DATA;
lançar erro tipado em falha de abertura ou escrita.
```

O escritor VTK 2D ainda não é Stable Public API.

## 16. Requisitos de grids estruturados 2D

`StructuredGrid2D` é experimental.

Deve ser composto por dois eixos 1D independentes:

```text
first_axis;
second_axis.
```

Deve fornecer:

```text
num_cells_x();
num_cells_y();
num_cells();
num_faces_x();
num_faces_y();
xmin();
xmax();
ymin();
ymax();
length_x();
length_y();
x_face();
y_face();
x_center();
y_center();
x_cell_length();
y_cell_length();
cell_area();
cell_measure();
physical_face_point();
```

Deve validar índices com erro tipado:

```text
errors::core::OutOfRange
```

Deve armazenar ou calcular:

```text
cell_measures;
first_face_measures;
second_face_measures;
physical_face_points.
```

A API 2D pode mudar antes de estabilização.

## 17. Requisitos de sistemas de coordenadas 2D

Sistemas de coordenadas 2D são experimentais.

Objetivo:

```text
permitir composição 2D por eixos lógicos;
permitir mapeamento para coordenadas físicas;
permitir cálculo de métricas geométricas;
permitir futura extensão para sistemas não cartesianos.
```

O sistema cartesiano 2D deve ser o padrão estável de referência conceitual, embora o módulo 2D ainda esteja classificado como experimental.

Regras:

```text
mapeamentos devem ser explícitos;
métricas devem ser validadas;
medidas de célula devem ser finitas e positivas;
medidas de face devem ser finitas e não negativas.
```

## 18. Requisitos de testes

Todo módulo estável deve ter testes unitários.

Os testes devem cobrir:

```text
construção válida;
validação de entradas inválidas;
mensagens/códigos de erro quando fizer sentido;
identidade de classes;
contratos de saída;
casos de borda.
```

Para `ErrorHandling`, os testes devem cobrir:

```text
BuiltInErrors;
ErrorDescriptor;
ErrorTraits;
ErrorContext;
ErrorRecord;
FVGridException;
ThrowError.
```

Não deve haver testes ativos para:

```text
ErrorCodes.h;
ErrorCatalog.h;
namespace error_code;
namespace error_catalog.
```

Para `Axis1D`, os testes devem cobrir:

```text
faces;
centers;
dx_faces;
dx_centers;
cell_lengths como alias;
bounds;
length;
pattern_name;
validação geométrica;
operator<<.
```

Para CSV, os testes devem cobrir:

```text
cabeçalho exato;
linhas de células;
última face com campos vazios;
criação de diretório pai;
falha de stream;
falha ao escrever em diretório.
```

## 19. Requisitos de exemplos

Exemplos devem compilar com a API pública atual.

Regras:

```text
não usar headers removidos;
não usar APIs antigas;
não usar ErrorCodes.h;
não usar ErrorCatalog.h;
não usar namespace error_code;
não usar namespace error_catalog;
usar erros tipados quando exemplificar ErrorHandling.
```

Exemplos podem usar YAML apenas como aplicação externa, desde que isso não introduza dependência em `FVGridMakerLib`.

## 20. Requisitos de documentação

A documentação deve refletir apenas a arquitetura atual.

Documentos que devem permanecer sincronizados:

```text
README.md;
FVGridMaker_requisitos.md;
FVGridMaker_tree.md;
FVGridMaker_cronograma_por_gates.md.
```

A documentação não deve apresentar APIs removidas como atuais.

Termos obsoletos devem ser removidos ou explicitamente marcados como históricos:

```text
AxisGrid1D;
ErrorCodes;
ErrorCatalog;
error_code;
error_catalog;
YAML interno à biblioteca.
```

## 21. Requisitos de build

O projeto deve ser construído por CMake.

Requisitos:

```text
C++20;
biblioteca FVGridMakerLib independente;
testes opcionais por flag de build;
exemplos opcionais por flag de build;
sem dependência obrigatória de yaml-cpp;
sem dependência obrigatória de bibliotecas geométricas externas.
```

O build deve gerar:

```text
biblioteca FVGridMaker;
exemplos;
testes unitários;
alvos auxiliares de execução de testes.
```

Alvos de teste devem permitir:

```text
rodar teste individual;
rodar todos os testes.
```

## 22. Requisitos de desempenho

A biblioteca deve evitar custos desnecessários em hot paths.

Regras:

```text
armazenar dados geométricos em vetores contíguos;
expor leitura por std::span;
evitar alocação em consultas simples;
evitar virtual dispatch no core;
preferir funções pequenas e inline quando apropriado;
evitar cópia desnecessária de vetores grandes;
preservar move semantics em construtores.
```

A clareza da API pública não deve justificar custos evitáveis em rotinas fundamentais.

## 23. Requisitos de estilo

Código deve usar:

```text
comentários em inglês;
nomes de API em inglês;
mensagens de erro em inglês;
arquivos organizados por módulo;
pastas de módulo com inicial maiúscula, seguindo o padrão atual do projeto.
```

Namespaces internos podem usar minúsculas, como:

```text
fvgrid::detail
```

Pastas internas devem seguir o padrão visual do projeto:

```text
Detail
```

## 24. Estado dos gates

### Gate 1 — Escopo público

Estado:

```text
concluído.
```

Resultado:

```text
API pública classificada;
Core, ErrorHandling, OneDimensional e CSV definidos como estáveis;
TwoDimensional e VTK definidos como experimentais;
YAML removido do núcleo da biblioteca.
```

### Gate 2 — Versionamento

Estado:

```text
concluído.
```

Resultado:

```text
versão canônica em CMake;
Core/Version.h expõe versão pública;
headers não devem duplicar versão literal.
```

### Gate 3 — Core

Estado:

```text
concluído.
```

Resultado:

```text
Types;
StrongTypes;
ID;
Version;
identidade sem enum central.
```

### Gate 4 — ErrorHandling

Estado:

```text
concluído.
```

Resultado:

```text
ErrorHandling refatorado para erros tipados;
BuiltInErrors substitui catálogo global;
ErrorCodes.h removido;
ErrorCatalog.h removido;
ThrowError.h header-only;
testes e exemplos migrados.
```

### Gate 5 — Output/Formatting

Estado:

```text
concluído.
```

Resultado:

```text
travessia tabular de Axis1D centralizada;
operator<< e Axis1DCSVWriter usam helper comum;
contrato humano protegido por teste;
contrato CSV protegido por teste.
```

## 25. Próximos gates candidatos

### Gate 6 — Documentação e limpeza de artefatos antigos

Objetivo:

```text
sincronizar README.md;
sincronizar FVGridMaker_tree.md;
sincronizar FVGridMaker_cronograma_por_gates.md;
remover referências antigas a ErrorCodes/ErrorCatalog;
remover referências antigas a AxisGrid1D;
garantir que a documentação pública descreve a arquitetura atual.
```

### Gate futuro — Semântica de pattern_name

Objetivo:

```text
definir se pattern_name representa o padrão geométrico original;
definir se pattern_name representa a operação que gerou o eixo;
definir como preservar histórico de operações, se necessário.
```

### Gate futuro — Estabilização 2D

Objetivo:

```text
decidir se StructuredGrid2D será promovido a Stable Public API;
consolidar sistemas de coordenadas 2D;
consolidar métricas 2D;
reforçar testes de VTK 2D;
definir contratos de output 2D.
```

### Gate futuro — Consumo externo

Objetivo:

```text
testar instalação;
testar find_package;
testar uso por projeto externo mínimo;
validar headers públicos;
validar exportação de targets CMake.
```

## 26. Critério de aceitação global

Um gate só deve ser considerado fechado quando:

```text
biblioteca compila;
exemplos compilam;
testes passam;
documentação relevante é atualizada;
git está limpo ou com alterações intencionais prontas para commit.
```

Para mudanças arquiteturais, também exigir:

```text
remoção de APIs antigas do fluxo ativo;
testes cobrindo o novo contrato;
documentação explicitando a decisão de projeto.
```
