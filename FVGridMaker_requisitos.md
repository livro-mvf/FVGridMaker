# FVGridMaker — Requisitos Funcionais e Não Funcionais

Este documento consolida os requisitos discutidos para o **FVGridMaker**, restritos à parte de **geração de malhas estruturadas**.

O FVGridMaker deve ser uma biblioteca independente para geração, armazenamento, validação, inspeção e exportação de malhas estruturadas compostas por eixos unidimensionais.

## 1. Escopo

O FVGridMaker deve tratar de:

```text
malhas estruturadas;
malhas 1D por direção;
composição multidimensional por eixos 1D;
faces;
centros de volumes;
métricas geométricas 1D;
validação de malha;
outputs de conferência;
exportação CSV;
exportação VTK Rectilinear Grid;
leitura opcional de configuração via YAML.
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
células poligonais genéricas;
boundary-fitted coordinates;
remalhamento adaptativo não estruturado.
```

A fronteira conceitual é:

```text
FVGridMaker:
  gera e valida malhas estruturadas.

FVMaker:
  consome essas malhas e implementa campos, operadores, condições de contorno,
  discretizações, equações e solvers.
```

## 2. Terminologia básica

Para uma malha 1D com `NVol` volumes:

```text
NVol volumes;
NVol coordenadas de centros;
NVol + 1 coordenadas de faces;
NVol distâncias entre faces;
NVol + 1 distâncias associadas aos centros.
```

Arrays fundamentais:

```text
faces.size() == NVol + 1
centers.size() == NVol
deltaFaces.size() == NVol
deltaCenters.size() == NVol + 1
```

Convenção de volume:

```text
volume i:
  face esquerda = faces[i]
  centro        = centers[i]
  face direita  = faces[i + 1]
```

## 3. Requisitos funcionais

## RF-GM-001 — Geração de malha estruturada 1D

O FVGridMaker deve gerar uma malha estruturada unidimensional a partir de parâmetros de entrada definidos por uma policy de geração.

A malha final deve conter:

```text
faces;
centers;
deltaFaces;
deltaCenters;
NVol;
metadados da geração;
metadados de seed, quando aplicável.
```

Critérios de aceitação:

```text
centers.size() == NVol;
faces.size() == NVol + 1;
deltaFaces.size() == NVol;
deltaCenters.size() == NVol + 1;
faces devem ser estritamente crescentes;
centers devem ser estritamente crescentes;
todos os deltaFaces devem ser positivos;
todos os deltaCenters devem ser positivos.
```

## RF-GM-002 — Acesso read-only às faces e aos centros

O FVGridMaker deve fornecer acesso público, eficiente e somente leitura às coordenadas das faces e dos centros dos volumes.

API esperada:

```cpp
std::span<const Real> faces() const noexcept;
std::span<const Real> centers() const noexcept;

Real face(Index i) const;
Real center(Index i) const;
```

Critérios de aceitação:

```text
faces() retorna std::span<const Real>;
centers() retorna std::span<const Real>;
o usuário não consegue modificar arrays internos por esses acessos;
o acesso escalar é O(1);
os dados são armazenados de forma contígua.
```

## RF-GM-003 — Cálculo de deltaFaces

O FVGridMaker deve calcular as distâncias entre faces consecutivas.

Regra:

```text
deltaFaces[i] = faces[i + 1] - faces[i],
para i = 0, ..., NVol - 1.
```

Critérios de aceitação:

```text
deltaFaces.size() == NVol;
a regra é a mesma para todos os volumes;
não há exceção nas fronteiras;
deltaFaces[i] > 0 para todo i.
```

## RF-GM-004 — Cálculo de deltaCenters

O FVGridMaker deve calcular as distâncias associadas às faces em relação aos centros dos volumes.

Regras:

```text
deltaCenters[0] = centers[0] - faces[0];

deltaCenters[i] = centers[i] - centers[i - 1],
para i = 1, ..., NVol - 1;

deltaCenters[NVol] = faces[NVol] - centers[NVol - 1].
```

Critérios de aceitação:

```text
deltaCenters.size() == NVol + 1;
apenas deltaCenters[0] e deltaCenters[NVol] são exceções;
as entradas internas usam distância centro-centro;
deltaCenters[i] > 0 para todo i.
```

## RF-GM-005 — Malha volume-centrada

O FVGridMaker deve suportar malhas volume-centradas.

Definição:

```text
malha volume-centrada:
  conheço as coordenadas das faces;
  calculo as coordenadas dos centros dos volumes.
```

Regra principal:

```text
centers[i] = (faces[i] + faces[i + 1]) / 2.
```

Critérios de aceitação:

```text
faces são coordenadas primárias;
centers são reconstruídos;
faces.size() == NVol + 1;
centers.size() == NVol;
cada centro deve estar entre suas duas faces.
```

## RF-GM-006 — Malha face-centrada

O FVGridMaker deve suportar malhas face-centradas.

Definição:

```text
malha face-centrada:
  conheço as coordenadas dos centros dos volumes;
  calculo as coordenadas das faces.
```

Regra para faces internas:

```text
a face interna entre dois volumes vizinhos fica no ponto médio entre os centros.
```

Com a convenção:

```text
face i está entre os volumes i - 1 e i,
para i = 1, ..., NVol - 1.
```

A regra é:

```text
faces[i] = (centers[i - 1] + centers[i]) / 2,
para i = 1, ..., NVol - 1.
```

As faces de fronteira devem ser calculadas por uma policy de fechamento de fronteira.

Critérios de aceitação:

```text
centers são coordenadas primárias;
faces internas são reconstruídas por médias entre centros vizinhos;
faces de fronteira seguem regra documentada;
faces.size() == NVol + 1;
centers.size() == NVol.
```

## RF-GM-007 — Independência entre geração e centragem

O FVGridMaker deve separar tipo de geração e padrão de centragem/reconstrução.

Exemplos válidos:

```text
uniforme + volume-centrada;
uniforme + face-centrada;
aleatória + volume-centrada;
aleatória + face-centrada;
malha customizada + padrão customizado.
```

Critérios de aceitação:

```text
geração e reconstrução são policies separáveis;
uma nova malha pode ser volume-centrada;
uma nova malha pode ser face-centrada;
uma nova malha pode usar padrão próprio;
não usar enum fechado para tipo de malha;
não usar enum fechado para centragem;
não usar herança virtual para isso.
```

## RF-GM-008 — Malha uniforme de fábrica

O FVGridMaker deve fornecer uma policy de malha uniforme de fábrica.

Dados de entrada:

```text
NVol;
Length;
XInit.
```

Definições:

```text
h = Length / NVol;
xEnd = XInit + Length.
```

Para a versão volume-centrada:

```text
faces[i] = XInit + i * h,
para i = 0, ..., NVol;

centers[i] = (faces[i] + faces[i + 1]) / 2,
para i = 0, ..., NVol - 1.
```

Critérios de aceitação:

```text
NVol > 0;
Length > 0;
faces[0] == XInit;
faces[NVol] == XInit + Length;
deltaFaces[i] == h para todo i;
deltaCenters[0] == h / 2;
deltaCenters[NVol] == h / 2;
deltaCenters[i] == h para i = 1, ..., NVol - 1.
```

## RF-GM-009 — Malha aleatória de fábrica

O FVGridMaker deve fornecer uma policy de malha aleatória de fábrica, baseada em perturbação de uma malha uniforme.

Dados de entrada:

```text
NVol;
Length;
XInit;
Epsilon;
Seed opcional.
```

A malha aleatória deve partir da malha uniforme com:

```text
h = Length / NVol.
```

Para a versão volume-centrada, as faces internas devem ser perturbadas:

```text
faces[0] == XInit;
faces[NVol] == XInit + Length;

faces[i] = XInit + i * h + r_i * Epsilon * h / 2,
para i = 1, ..., NVol - 1;

r_i pertence a [-1, 1].
```

Critérios de aceitação:

```text
0 <= Epsilon < 1;
faces de fronteira permanecem fixas;
apenas faces internas são perturbadas;
faces continuam estritamente crescentes;
deltaFaces[i] > 0 para todo i;
mesmo seed gera a mesma malha;
sem seed informado, o código gera seed aleatório;
o seed efetivo usado deve ser armazenado e consultável.
```

## RF-GM-010 — Seed opcional em malhas aleatórias

O FVGridMaker deve aceitar seed opcional nas policies aleatórias.

Regras:

```text
com seed informado:
  a geração é determinística e reprodutível.

sem seed informado:
  o próprio código gera um seed aleatório;
  a malha pode mudar a cada execução.
```

Critérios de aceitação:

```text
seed informado é preservado;
seed efetivo é consultável;
flag seedWasUserProvided é consultável;
malha com mesmo seed e mesmos parâmetros é idêntica;
malha sem seed gera seed interno válido.
```

## RF-GM-011 — Extensão de tipos de malha sem alteração do núcleo

O FVGridMaker deve permitir novos tipos de malha por meio de tipos concretos, policies, traits e concepts.

Critérios de aceitação:

```text
usuário pode criar nova policy em novo arquivo;
nova policy não exige alteração de enum central;
nova policy não exige alteração de classe-base virtual;
nova policy é validada por concept;
strings de seleção ficam restritas a YAML, diagnóstico ou registry local;
loops de geração usam tipos concretos.
```

## RF-GM-012 — Builder 1D

O FVGridMaker deve fornecer um builder para criação fluente de malhas 1D.

Exemplo conceitual:

```cpp
auto axis =
    GridAxis1DBuilder{}
        .set_nvol(NVol{100})
        .set_length(Length{1.0})
        .set_xinit(XInit{0.0})
        .build_with(UniformFaces{}, CentersFromFaces{});
```

Critérios de aceitação:

```text
builder valida configuração antes de construir;
builder lança FVGridException em caso inválido;
builder não usa enum para tipo de malha;
builder não usa herança virtual;
builder permite policies concretas.
```

## RF-GM-013 — Composição direcional 2D

O FVGridMaker deve representar malhas bidimensionais como composição de duas malhas 1D independentes.

Regra:

```text
malha 2D = MalhaX(i) + MalhaY(j).
```

Não deve haver representação central obrigatória:

```text
malha2D(i, j).
```

Critérios de aceitação:

```text
axis_x é GridAxis1D;
axis_y é GridAxis1D;
não materializar células (i,j) no núcleo;
não criar conectividade 2D explícita;
usuário acessa x.faces(), x.centers(), y.faces(), y.centers().
```

## RF-GM-014 — Sistemas de coordenadas analíticos estruturados

O FVGridMaker deve permitir associação de malhas estruturadas a sistemas de coordenadas analíticos.

Sistemas previstos:

```text
cartesiano, obrigatório;
cilíndrico, desejável;
esférico, desejável;
elipsoidal, futuro desejável.
```

Critérios de aceitação:

```text
cada direção é representada por uma malha 1D;
o sistema de coordenadas interpreta os eixos;
o sistema de coordenadas não gera a malha;
não usar enum fechado para sistema de coordenadas;
novos sistemas podem ser adicionados por tipos, traits e policies.
```

## RF-GM-015 — Ausência de boundary-fitted coordinates

O FVGridMaker não deve implementar coordenadas ajustadas à fronteira.

Critérios de aceitação:

```text
não resolver transformações geométricas arbitrárias;
não aceitar mapeamentos gerais x(xi, eta), y(xi, eta);
não deformar malhas para ajustar domínios irregulares;
não implementar malha curvilínea genérica.
```

## RF-GM-016 — Leitura YAML de configuração

O FVGridMaker deve permitir leitura de configuração via YAML em módulo separado.

Critérios de aceitação:

```text
yaml-cpp aparece apenas no módulo YAML;
núcleo compila sem yaml-cpp;
YAML pode definir NVol, Length, XInit, policy, Epsilon e Seed;
erros YAML usam FVGridException;
strings YAML são convertidas para policies por registry local;
não há enum central para tipo de malha.
```

## RF-GM-017 — Sistema próprio de erros e exceções

O FVGridMaker deve reportar erros por sistema próprio.

Elementos mínimos:

```text
FVGridException;
ErrorRecord;
throw_error;
std::source_location;
códigos textuais estáveis.
```

Critérios de aceitação:

```text
não usar enum para erros;
não usar enum para exceções;
não usar enum para mensagens;
não lançar std::runtime_error diretamente no domínio;
cada erro possui código textual;
cada erro possui mensagem;
cada erro possui módulo e localização.
```

## RF-OUT-001 — Output textual 1D para conferência

O FVGridMaker deve gerar relatório textual tabular para conferência de uma malha 1D.

Colunas mínimas:

```text
i;
face_left;
center;
face_right;
delta_face;
delta_center_left;
delta_center_right;
cell_type.
```

Critérios de aceitação:

```text
usar std::ostream;
permitir precisão configurável;
incluir metadados;
não depender de bibliotecas externas;
teste compara saída esperada.
```

## RF-OUT-002 — Output textual 2D por direção

Para malhas bidimensionais, o output textual deve imprimir uma direção e depois outra.

Formato esperado:

```text
[axis x]
tabela da malha x

[axis y]
tabela da malha y
```

Critérios de aceitação:

```text
não imprimir tabela (i,j);
não materializar células bidimensionais para relatório;
cada direção usa o formato tabular 1D;
ordem das direções é estável e documentada.
```

## RF-OUT-003 — CSV por volume

O FVGridMaker deve exportar cada malha 1D para CSV com uma linha por volume.

Colunas obrigatórias:

```text
i;
face_left;
center;
face_right;
delta_face;
delta_center_left;
delta_center_right;
cell_type.
```

Critérios de aceitação:

```text
CSV é legível em editor ou planilha;
metadados podem ser incluídos como comentários no topo;
precisão é configurável;
locale numérico deve ser estável.
```

## RF-OUT-004 — CSV por face

O FVGridMaker deve permitir exportação opcional com uma linha por face.

Colunas obrigatórias:

```text
i;
face;
delta_center;
face_type.
```

Critérios de aceitação:

```text
faces.size() == NVol + 1;
deltaCenters.size() == NVol + 1;
cada linha associa uma face a um deltaCenter.
```

## RF-OUT-005 — CSV multidirecional

Para uma malha estruturada multidimensional, o FVGridMaker deve exportar arquivos por eixo.

Exemplo:

```text
mesh_x_cells.csv;
mesh_y_cells.csv;
mesh_x_faces.csv, opcional;
mesh_y_faces.csv, opcional.
```

Critérios de aceitação:

```text
cada eixo é exportado isoladamente;
não há tabela (i,j);
nomes de arquivo são estáveis;
metadados de eixo são incluídos.
```

## RF-OUT-006 — VTK Rectilinear Grid

O FVGridMaker deve exportar malhas estruturadas multidimensionais em formato VTK Rectilinear Grid, inicialmente no padrão legacy ASCII `.vtk`.

Critérios de aceitação:

```text
usar coordenadas das faces;
usar DATASET RECTILINEAR_GRID;
não usar biblioteca VTK;
não converter para malha não estruturada;
ParaView deve conseguir ler o arquivo;
para 2D, usar dimensão z degenerada.
```

## RF-DOC-001 — Documentação Sphinx

O FVGridMaker deve possuir documentação Sphinx semelhante ao bloco II.

Recursos previstos:

```text
pydata_sphinx_theme;
myst_parser;
sphinx_design;
sphinx_copybutton;
CSS próprio;
página inicial com hero;
toctree organizada;
guia de usuário;
guia de desenvolvedor;
documentação de requisitos;
documentação de arquitetura;
documentação de output.
```

Critérios de aceitação:

```text
Sphinx gera HTML sem erros;
documentação possui página inicial;
documentação explica GridAxis1D;
documentação explica malha 2D por eixos;
documentação explica output textual, CSV e VTK.
```

## 4. Requisitos não funcionais

## RNF-GM-001 — Malhas sempre estruturadas

O FVGridMaker deve tratar apenas malhas estruturadas.

Critérios:

```text
malha multidimensional é composição de eixos 1D;
não há conectividade não estruturada;
não há células arbitrárias;
não há owner-neighbour genérico no núcleo.
```

## RNF-GM-002 — Núcleo sem dependências externas

O núcleo do FVGridMaker deve depender apenas da biblioteca padrão C++20 ou superior.

Exceções:

```text
yaml-cpp, apenas no módulo YAML;
GoogleTest, apenas nos testes.
```

Critérios:

```text
GridAxis1D, builders, metrics e policies principais compilam sem yaml-cpp;
headers públicos do núcleo não incluem yaml-cpp;
headers públicos do núcleo não incluem GoogleTest.
```

## RNF-GM-003 — C++20 ou superior

O FVGridMaker deve usar C++20 como padrão mínimo.

Recursos recomendados:

```text
std::vector;
std::array;
std::span;
std::string_view;
std::filesystem;
std::optional;
std::source_location;
templates;
concepts;
traits;
policies;
std::execution quando aplicável;
std::jthread quando aplicável.
```

## RNF-GM-004 — Prioridade para tempo de execução

O projeto deve priorizar tempo de execução, localidade de memória, vectorização e eficiência paralela acima de tempo de compilação.

Critérios:

```text
templates e concepts são aceitáveis;
compilação mais lenta é aceitável se execução melhorar;
hot paths não devem ter virtual dispatch;
hot paths não devem fazer string lookup;
hot paths não devem alocar dinamicamente.
```

## RNF-GM-005 — Sem herança virtual no núcleo

O FVGridMaker não deve usar herança virtual para representar:

```text
malhas;
eixos;
distribuições;
policies;
métricas;
writers centrais;
validadores centrais.
```

Critérios:

```text
nenhum loop crítico depende de chamada virtual;
substituição de comportamento ocorre por policies e concepts;
interfaces virtuais não são usadas no núcleo.
```

## RNF-GM-006 — Data-Oriented Design

O FVGridMaker deve seguir DOD para armazenamento e processamento de dados geométricos.

Critérios:

```text
dados geométricos principais em arrays contíguos;
usar SoA;
usar std::span para views;
não criar objeto Cell por volume;
não criar objeto Face por face;
não alocar por entidade;
não transformar malha estruturada em malha não estruturada artificial.
```

## RNF-GM-007 — SOLID moderno

O FVGridMaker deve aplicar SOLID reinterpretado para C++ moderno.

Critérios:

```text
Single Responsibility:
  cada componente tem responsabilidade clara.

Open/Closed:
  novas malhas por extensão local, não modificação global.

Liskov:
  substituição por concepts, não por herança virtual.

Interface Segregation:
  interfaces pequenas e específicas.

Dependency Inversion:
  núcleo não depende de YAML, output ou FVMaker.
```

## RNF-GM-008 — STL intensiva

O FVGridMaker deve usar intensivamente a STL quando isso melhorar clareza, segurança e desempenho.

Recursos esperados:

```text
std::vector;
std::span;
std::array;
std::transform;
std::for_each;
std::iota;
std::adjacent_difference;
std::all_of;
std::is_sorted;
std::ranges quando apropriado.
```

Critérios:

```text
loops manuais são permitidos quando mais claros ou eficientes;
containers associativos não devem aparecer em hot paths;
std::function e std::any não devem aparecer em hot paths.
```

## RNF-GM-009 — Paralelização padrão C++ quando vantajosa

O FVGridMaker deve permitir paralelização com recursos padrão C++20 ou superior quando houver ganho real.

Critérios:

```text
não usar TBB;
não usar OpenMP;
não usar Boost;
não paralelizar automaticamente malhas pequenas;
preservar determinismo quando possível;
escrever em posições independentes de arrays.
```

## RNF-GM-010 — Validação explícita

O FVGridMaker deve rejeitar configurações inválidas explicitamente.

Critérios:

```text
não corrigir silenciosamente entradas inválidas;
não ignorar tags ou policies inválidas;
não permitir malha parcialmente válida;
falhas geram FVGridException;
mensagens devem indicar parâmetro e contexto.
```

## RNF-GM-011 — API pública simples e limpa

A API pública deve esconder detalhes internos.

Critérios:

```text
usuário cria malha sem conhecer implementação interna;
headers públicos não expõem YAML;
headers públicos não expõem GoogleTest;
headers públicos expõem tipos concretos, views e builders;
não expor detalhes de registry interno.
```

## RNF-GM-012 — Separação de responsabilidades

O FVGridMaker deve separar:

```text
builders constroem;
objetos de malha armazenam;
validators validam;
metric policies calculam métricas;
writers exportam;
YAML readers configuram;
tests verificam.
```

Critérios:

```text
GridAxis1D não lê YAML;
GridAxis1D não escreve CSV;
GridAxis1D não escreve VTK;
GridAxis1D não decide todas as policies.
```

## RNF-GM-013 — Sem enum para categorias extensíveis

O FVGridMaker não deve usar enum para categorias que podem crescer.

Proibido usar enum para:

```text
tipo de malha;
centragem;
generation policy;
reconstruction policy;
metric policy;
writer;
sistema de coordenadas extensível;
erros;
exceções;
mensagens.
```

Critérios:

```text
usar tipos concretos;
usar traits;
usar concepts;
usar tags;
usar identifiers textuais;
usar registries locais quando necessário.
```

## RNF-GM-014 — Enums apenas para categorias estruturalmente fechadas

Enums só podem ser considerados para categorias comprovadamente fechadas.

Exemplo possível:

```text
dimensão espacial, se o projeto decidir que 1D, 2D e 3D são o conjunto completo.
```

Mesmo nesse caso, tipos e traits continuam preferíveis se houver chance de extensão.

## RNF-GM-015 — Sistema de erros sem enum

O sistema de erros deve usar códigos textuais estáveis.

Critérios:

```text
não usar enum para ErrorCode;
não usar enum para mensagens;
não usar enum para categorias de exceção;
novos módulos podem adicionar códigos sem editar enum central;
ErrorRecord carrega strings e contexto.
```

## RNF-GM-016 — Clean code moderno

O FVGridMaker deve aplicar clean code em C++ moderno.

Técnicas recomendadas:

```text
strong types;
builders;
funções livres testáveis;
objects read-only após construção;
namespaces detail para implementação interna;
[[nodiscard]];
constexpr quando útil;
noexcept quando correto;
std::source_location;
formatação automática;
comentários explicativos e não ornamentais.
```

Critérios:

```text
não criar classes gigantes;
não misturar I/O com armazenamento;
não misturar geração com física;
não duplicar regras geométricas;
documentar invariantes.
```

## RNF-GM-017 — Estilo MohidNG

O FVGridMaker deve adotar estilo baseado no MohidNG.

Regras:

```text
BasedOnStyle: Google;
C++20;
ColumnLimit: 100;
SortIncludes: true;
PointerAlignment: Left;
AllowShortIfStatementsOnASingleLine: false;
AllowShortLoopsOnASingleLine: false.
```

Convenções:

```text
PascalCase para tipos;
PascalCase para funções públicas, se mantido o padrão Google;
snake_case para variáveis;
membros de classe terminam com underscore;
constantes usam kName;
includes em três grupos: STL, externo, FVGridMaker;
headers do projeto com <FVGridMaker/...>;
comentários de código com //;
evitar blocos /* ... */.
```

## RNF-GM-018 — Testes obrigatórios com GoogleTest

Cada classe, policy, writer e componente relevante deve possuir teste GoogleTest próprio.

Critérios:

```text
classe nova sem teste não é considerada implementada;
policy nova sem teste não é considerada implementada;
writer novo sem teste não é considerado implementado;
erro novo sem teste não é considerado implementado;
builder novo sem teste não é considerado implementado;
todos os testes executam via CTest;
GoogleTest não aparece em headers públicos.
```

## RNF-GM-019 — Exemplos como contrato de API

Cada funcionalidade visível ao usuário deve ter exemplo executável.

Critérios:

```text
exemplo compila;
exemplo executa via CMake ou CTest;
mudança de API que quebra exemplo é quebra de contrato;
exemplos são simples e verificáveis.
```

## RNF-GM-020 — Output desacoplado do núcleo

Output não deve contaminar o núcleo.

Critérios:

```text
GridAxis1D não depende de CSV;
GridAxis1D não depende de VTK;
GridAxis1D não depende de YAML;
writers dependem das views da malha;
núcleo não depende dos writers.
```

## RNF-GM-021 — Determinismo de output

Para uma mesma malha, os arquivos exportados devem ser reprodutíveis.

Critérios:

```text
mesma ordem de linhas;
mesma precisão quando configurada;
mesmos metadados;
locale numérico estável;
sem dependência do locale do sistema.
```

## RNF-GM-022 — Precisão configurável de output

Os writers devem permitir configuração de precisão numérica.

Exemplo conceitual:

```cpp
struct CsvWriteOptions {
    int precision = 16;
    bool scientific = true;
    bool include_metadata = true;
};
```

## RNF-DOC-001 — Documentação Sphinx

A documentação deve usar Sphinx com tema moderno.

Critérios:

```text
usar pydata_sphinx_theme;
usar myst_parser;
usar sphinx_design;
usar sphinx_copybutton;
usar CSS próprio;
gerar HTML sem erro;
documentar requisitos, arquitetura, estilo, testes, output e guia de usuário.
```

## 5. Requisitos de prioridade

Prioridades iniciais:

```text
P0:
  fundação do projeto;
  erros próprios;
  tipos fortes;
  GridAxis1D;
  métricas 1D;
  uniforme volume-centrada;
  builder;
  testes GoogleTest;
  output textual 1D;
  CSV 1D.

P1:
  uniforme face-centrada;
  aleatória volume-centrada;
  policies, traits e concepts;
  YAML;
  composição 2D por eixos;
  output textual 2D por direção;
  CSV multidirecional.

P2:
  VTK Rectilinear Grid;
  documentação Sphinx completa;
  paralelização onde houver ganho;
  sistemas de coordenadas além do cartesiano.

P3:
  coordenadas cilíndricas, esféricas e elipsoidais;
  VTR XML;
  outputs adicionais se justificados.
```

## 6. Regra final de projeto

```text
O FVGridMaker deve gerar malhas estruturadas por direção, com dados contíguos,
API read-only, validação explícita, erros próprios sem enum, extensibilidade por
policies/traits/concepts, testes GoogleTest para cada classe e outputs simples
para conferência e visualização.
```
