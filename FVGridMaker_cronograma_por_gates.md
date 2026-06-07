# FVGridMaker — Cronograma por Gates

Este documento define um cronograma técnico sem datas para o desenvolvimento do **FVGridMaker**. O avanço deve ocorrer por **gates de qualidade**, não por calendário.

A regra central é:

```text
Não iniciar um novo bloco funcional enquanto o bloco anterior não estiver
compilando, testado, documentado minimamente e com exemplos executáveis quando
a funcionalidade for visível ao usuário.
```

## 1. Regras gerais de avanço

Nenhum bloco novo deve começar se qualquer uma das condições abaixo estiver pendente:

```text
algum teste GoogleTest falhando;
alguma classe nova sem teste próprio;
alguma policy nova sem teste próprio;
algum writer novo sem teste próprio;
algum exemplo público quebrado;
documentação mínima ausente;
erro esperado sem código textual testado;
decisão arquitetural pendente que afete o bloco seguinte.
```

As regras formais de avanço são:

```text
1. Nenhum bloco novo começa com teste quebrado.
2. Nenhum bloco novo começa com exemplo quebrado.
3. Nenhum bloco novo começa com documentação essencial ausente.
4. Nenhuma funcionalidade pública entra sem exemplo.
5. Nenhuma regra geométrica entra sem teste numérico.
6. Nenhuma policy nova entra sem teste de invariantes.
7. Nenhum output entra sem arquivo de referência simples.
8. Nenhum erro esperado entra sem código textual testado.
9. Nenhum módulo periférico pode criar dependência reversa no núcleo.
10. Nenhuma categoria extensível deve ser implementada com enum.
11. Nenhuma classe nova é considerada pronta sem teste GoogleTest.
12. Nenhuma funcionalidade é considerada pronta sem passar pelo CTest completo.
```

## 2. Política obrigatória de testes

O FVGridMaker usa **GoogleTest** como infraestrutura principal de testes unitários e de integração.

A regra é:

```text
classe nova sem teste não é considerada implementada;
policy nova sem teste não é considerada implementada;
writer novo sem teste não é considerado implementado;
erro novo sem teste não é considerado implementado;
builder novo sem teste não é considerado implementado.
```

A restrição de dependências externas vale para a biblioteca em runtime:

```text
Dependências da biblioteca:
  C++ standard library;
  yaml-cpp apenas no módulo YAML.

Dependências de teste:
  GoogleTest.
```

O GoogleTest não deve aparecer em headers públicos da biblioteca.

### 2.1 Convenção sugerida para testes

Estrutura sugerida:

```text
tests/
  Core/
    tst_Types.cc
    tst_StrongTypes.cc

  Error/
    tst_FVGridException.cc
    tst_ThrowError.cc

  Grid/
    tst_GridBuildConfig1D.cc
    tst_GridAxis1D.cc
    tst_GridAxis1DBuilder.cc

  Metrics/
    tst_StandardMetrics1D.cc

  Policies/
    tst_UniformFaces.cc
    tst_UniformCenters.cc
    tst_RandomFaces.cc
    tst_CentersFromFaces.cc
    tst_FacesFromCenters.cc

  Output/
    tst_TextAxisReportWriter.cc
    tst_CsvAxisCellsWriter.cc
    tst_CsvAxisFacesWriter.cc
    tst_VtkRectilinearWriter.cc

  YAML/
    tst_YamlGridReader.cc

  Integration/
    tst_CreateUniformGrid1D.cc
    tst_CreateRandomGrid1D.cc
    tst_CreateStructuredGrid2D.cc
```

Nome de arquivo recomendado:

```text
tst_NomeDaClasse.cc
```

Todos os testes devem ser executáveis via CTest.

## 3. Cronograma por blocos

## Bloco 0 — Fundação do projeto

Objetivo: preparar o esqueleto técnico do FVGridMaker antes da lógica de malha.

Atividades:

```text
definir estrutura de diretórios;
definir CMake modular;
definir opção FVGRID_BUILD_TESTS;
integrar GoogleTest;
integrar CTest;
definir padrão .clang-format baseado no MohidNG;
definir namespaces;
definir tipos básicos: Real, Index, Size;
definir política de includes;
definir política de comentários;
definir sistema mínimo de exemplos;
definir sistema mínimo de testes;
definir documentação Sphinx inicial;
definir README inicial.
```

Critério de passagem:

```text
cmake configura;
projeto compila;
um exemplo mínimo executa;
um teste GoogleTest mínimo executa;
CTest roda e passa;
Sphinx gera documentação;
clang-format está definido;
nenhum módulo de malha ainda é necessário.
```

## Bloco 1 — Sistema próprio de erros

Objetivo: criar a base de erros antes das validações da malha.

Atividades:

```text
criar FVGridException;
criar ErrorRecord;
criar throw_error;
usar std::source_location;
definir códigos textuais estáveis;
não usar enum para erros;
não usar enum para exceções;
não usar enum para mensagens;
não usar std::runtime_error diretamente no domínio;
criar testes de erro;
documentar política de erros.
```

Testes obrigatórios:

```text
tst_FVGridException.cc;
tst_ThrowError.cc.
```

Critério de passagem:

```text
lançar FVGridException funciona;
ErrorRecord contém código, mensagem, módulo e localização;
testes confirmam códigos textuais;
nenhum enum é usado para erro, exceção ou mensagem;
documentação da política de erro existe;
CTest completo passa.
```

## Bloco 2 — Tipos fortes e configuração básica 1D

Objetivo: definir os dados de entrada da malha sem ambiguidade.

Atividades:

```text
criar tipos fortes: NVol, Length, XInit, Epsilon, Seed;
criar GridBuildConfig1D;
validar NVol > 0;
validar Length > 0;
validar epsilon quando aplicável;
definir seed opcional;
definir seed efetivo;
criar testes de configuração válida e inválida.
```

Testes obrigatórios:

```text
tst_StrongTypes.cc;
tst_GridBuildConfig1D.cc.
```

Critério de passagem:

```text
configurações válidas são aceitas;
configurações inválidas geram FVGridException;
seed opcional funciona conceitualmente;
seed efetivo pode ser armazenado;
testes passam;
documentação mínima dos dados de entrada existe;
CTest completo passa.
```

## Bloco 3 — Armazenamento DOD da malha 1D

Objetivo: criar o objeto central da biblioteca.

Atividades:

```text
criar GridAxis1D;
armazenar faces em std::vector<Real>;
armazenar centers em std::vector<Real>;
armazenar deltaFaces em std::vector<Real>;
armazenar deltaCenters em std::vector<Real>;
expor std::span<const Real>;
criar acesso escalar read-only;
definir invariantes:
  centers.size() == NVol;
  faces.size() == NVol + 1;
  deltaFaces.size() == NVol;
  deltaCenters.size() == NVol + 1.
```

Testes obrigatórios:

```text
tst_GridAxis1D.cc.
```

Critério de passagem:

```text
GridAxis1D existe;
dados são contíguos;
views read-only funcionam;
usuário não modifica arrays internos;
invariantes são testadas;
sem herança virtual;
sem enum;
CTest completo passa.
```

## Bloco 4 — Métricas padrão 1D

Objetivo: implementar as métricas geométricas básicas.

Atividades:

```text
implementar deltaFaces:
  deltaFaces[i] = faces[i+1] - faces[i];

implementar deltaCenters:
  deltaCenters[0] = centers[0] - faces[0];
  deltaCenters[i] = centers[i] - centers[i-1];
  deltaCenters[NVol] = faces[NVol] - centers[NVol-1];

validar positividade;
criar funções livres testáveis;
documentar a convenção.
```

Testes obrigatórios:

```text
tst_StandardMetrics1D.cc.
```

Critério de passagem:

```text
deltaFaces tem NVol entradas;
deltaCenters tem NVol + 1 entradas;
somente deltaCenters[0] e deltaCenters[NVol] são casos especiais;
todas as métricas são positivas;
testes numéricos passam para malhas simples;
documentação da métrica existe;
CTest completo passa.
```

## Bloco 5 — Política volume-centrada uniforme

Objetivo: implementar a primeira malha funcional de fábrica.

Atividades:

```text
criar policy UniformFaces;
criar reconstruction policy CentersFromFaces;
gerar faces a partir de NVol, Length, XInit;
calcular centers;
calcular métricas;
validar malha final.
```

Testes obrigatórios:

```text
tst_UniformFaces.cc;
tst_CentersFromFaces.cc;
tst_CreateUniformGrid1D.cc.
```

Critério de passagem:

```text
malha uniforme volume-centrada é gerada;
faces[0] == XInit;
faces[NVol] == XInit + Length;
centers[i] == (faces[i] + faces[i+1]) / 2;
deltaFaces é constante;
deltaCenters de fronteira são h/2;
exemplo executável existe;
testes passam;
CTest completo passa.
```

## Bloco 6 — Builder 1D

Objetivo: oferecer API pública limpa para construir malhas.

Atividades:

```text
criar GridAxis1DBuilder;
adicionar set_nvol;
adicionar set_length;
adicionar set_xinit;
adicionar build_with(policy);
permitir composição com reconstruction policy;
validar antes de construir;
não usar enum;
não usar virtual.
```

Testes obrigatórios:

```text
tst_GridAxis1DBuilder.cc.
```

Critério de passagem:

```text
usuário cria malha via builder;
malha uniforme é reproduzida via builder;
erros do builder são estruturados;
exemplo público funciona;
testes passam;
documentação do builder existe;
CTest completo passa.
```

## Bloco 7 — Output textual de conferência 1D

Objetivo: permitir inspeção humana da malha.

Atividades:

```text
criar writer textual por volume;
imprimir i, face_left, center, face_right;
imprimir delta_face;
imprimir delta_center_left;
imprimir delta_center_right;
incluir metadados;
permitir precisão configurável;
usar std::ostream.
```

Testes obrigatórios:

```text
tst_TextAxisReportWriter.cc.
```

Critério de passagem:

```text
relatório de uma malha 1D é legível;
valores batem com os arrays internos;
não há dependência externa;
teste compara saída esperada;
exemplo executável gera relatório;
CTest completo passa.
```

## Bloco 8 — CSV 1D

Objetivo: criar formato simples de conferência em colunas.

Atividades:

```text
criar CSV por volume;
criar CSV por face, se necessário;
incluir metadados no topo;
definir separador;
definir precisão;
forçar locale estável;
documentar colunas.
```

Testes obrigatórios:

```text
tst_CsvAxisCellsWriter.cc;
tst_CsvAxisFacesWriter.cc, se CSV por face for implementado.
```

Critério de passagem:

```text
CSV por volume é gerado;
colunas estão documentadas;
arquivo pode ser aberto em editor/planilha;
teste verifica cabeçalho e valores;
exemplo executável gera CSV;
CTest completo passa.
```

## Bloco 9 — Política face-centrada uniforme

Objetivo: suportar a segunda centragem de fábrica.

Atividades:

```text
criar policy UniformCenters;
criar reconstruction policy FacesFromCenters;
gerar centers;
calcular faces internas por média entre centros vizinhos;
definir política de fechamento das faces de fronteira;
calcular métricas;
validar malha final.
```

Testes obrigatórios:

```text
tst_UniformCenters.cc;
tst_FacesFromCenters.cc.
```

Critério de passagem:

```text
malha face-centrada uniforme é gerada;
faces internas obedecem à regra definida;
faces de fronteira obedecem à política documentada;
invariantes são preservadas;
testes passam;
exemplo executável existe;
CTest completo passa.
```

## Bloco 10 — Malha aleatória volume-centrada

Objetivo: implementar a primeira malha aleatória de fábrica.

Atividades:

```text
criar RandomFaces;
partir da malha uniforme;
perturbar apenas faces internas;
usar deslocamento em [-epsilon*h/2, +epsilon*h/2];
exigir 0 <= epsilon < 1;
manter faces de fronteira fixas;
usar seed fornecido ou seed gerado internamente;
armazenar seed efetivo.
```

Testes obrigatórios:

```text
tst_RandomFaces.cc;
tst_CreateRandomGrid1D.cc.
```

Critério de passagem:

```text
malha aleatória mantém faces crescentes;
fronteiras permanecem fixas;
mesmo seed gera mesma malha;
sem seed gera seed interno;
seed efetivo é consultável;
testes estatísticos simples passam;
exemplo executável existe;
CTest completo passa.
```

## Bloco 11 — Extensão por policies, traits e concepts

Objetivo: consolidar a arquitetura aberta.

Atividades:

```text
definir concepts para generation policy;
definir concepts para reconstruction policy;
definir traits de policies;
definir nomes textuais estáveis das policies;
testar uma policy externa simples;
evitar enum;
evitar switch global.
```

Testes obrigatórios:

```text
tst_GenerationPolicyConcept.cc;
tst_ReconstructionPolicyConcept.cc;
tst_PolicyTraits.cc;
tst_CustomPolicyExtension.cc.
```

Critério de passagem:

```text
nova policy pode ser criada sem alterar o núcleo;
concepts detectam policy inválida;
traits fornecem nome e metadados;
testes demonstram extensão local;
documentação para criação de nova malha existe;
CTest completo passa.
```

## Bloco 12 — YAML de entrada

Objetivo: permitir configuração externa sem contaminar o núcleo.

Atividades:

```text
criar módulo YAML separado;
ler NVol, Length, XInit;
ler generation_policy;
ler reconstruction_policy;
ler epsilon;
ler seed opcional;
converter strings para policies registradas;
não incluir yaml-cpp no núcleo.
```

Testes obrigatórios:

```text
tst_YamlGridReader.cc.
```

Critério de passagem:

```text
núcleo compila sem yaml-cpp;
módulo YAML compila com yaml-cpp;
YAML uniforme gera mesma malha que builder C++;
YAML aleatório com seed é reprodutível;
erros YAML usam FVGridException;
exemplo executável existe;
CTest completo passa.
```

## Bloco 13 — Composição direcional 2D

Objetivo: representar malha bidimensional como duas malhas 1D.

Atividades:

```text
definir composição com axis_x e axis_y;
não materializar células (i,j);
permitir acesso a x.faces(), x.centers();
permitir acesso a y.faces(), y.centers();
definir metadados da dimensão;
definir sistema cartesiano inicial.
```

Testes obrigatórios:

```text
tst_StructuredGrid2D.cc;
tst_CreateStructuredGrid2D.cc.
```

Critério de passagem:

```text
malha 2D é composta por duas GridAxis1D;
não há tabela (i,j) no núcleo;
exemplo cria malha_x e malha_y;
testes confirmam independência dos eixos;
documentação deixa claro: MeshX(i), MeshY(j);
CTest completo passa.
```

## Bloco 14 — Output textual 2D por direção

Objetivo: conferir malhas 2D sem tabela tensorial.

Atividades:

```text
escrever relatório com bloco axis x;
depois bloco axis y;
reutilizar formato tabular 1D;
incluir metadados globais;
não percorrer (i,j).
```

Testes obrigatórios:

```text
tst_StructuredGridReportWriter.cc.
```

Critério de passagem:

```text
relatório 2D imprime x e depois y;
cada direção usa a mesma tabela 1D;
não há materialização de células 2D;
teste compara saída esperada;
exemplo executável existe;
CTest completo passa.
```

## Bloco 15 — CSV multidirecional

Objetivo: exportar as direções de uma malha estruturada.

Atividades:

```text
gerar mesh_x_cells.csv;
gerar mesh_y_cells.csv;
opcionalmente gerar mesh_x_faces.csv;
opcionalmente gerar mesh_y_faces.csv;
incluir metadados por eixo;
definir nomes de arquivos.
```

Testes obrigatórios:

```text
tst_StructuredGridCsvWriter.cc.
```

Critério de passagem:

```text
CSV de x e y é gerado;
cada arquivo é conferível isoladamente;
testes verificam conteúdo;
exemplo executável existe;
CTest completo passa.
```

## Bloco 16 — VTK Rectilinear Grid ASCII

Objetivo: visualizar malhas estruturadas no ParaView.

Atividades:

```text
implementar writer VTK legacy RECTILINEAR_GRID;
usar coordenadas das faces;
suportar 2D com z degenerado;
não usar biblioteca VTK;
usar std::ostream;
testar arquivo mínimo.
```

Testes obrigatórios:

```text
tst_VtkRectilinearWriter.cc.
```

Critério de passagem:

```text
arquivo .vtk é gerado;
ParaView lê o arquivo;
dimensões usam NFacesX, NFacesY, 1;
não há malha não estruturada;
exemplo executável existe;
documentação do formato existe;
CTest completo passa.
```

## Bloco 17 — Documentação Sphinx

Objetivo: estabilizar documentação semelhante ao bloco II.

Atividades:

```text
configurar Sphinx;
usar pydata_sphinx_theme;
usar myst_parser;
usar sphinx_design;
usar sphinx_copybutton;
criar CSS próprio;
criar página inicial com hero;
criar guia de usuário;
criar guia de desenvolvedor;
documentar requisitos;
documentar arquitetura;
documentar estilo;
documentar testes;
documentar desempenho;
documentar output.
```

Critério de passagem:

```text
Sphinx gera HTML sem erro;
página inicial existe;
documentação de requisitos existe;
documentação de arquitetura existe;
documentação de coding standard existe;
documentação de testes existe;
documentação de uso básico existe.
```

## Bloco 18 — Testes de regressão e validação geral

Objetivo: garantir que a base está sólida antes de avançar para novas famílias de malha.

Atividades:

```text
rodar todos os testes;
rodar todos os exemplos;
validar outputs CSV;
validar outputs VTK;
testar erros esperados;
testar seed reprodutível;
testar documentação;
limpar warnings;
revisar cobertura dos requisitos;
verificar se toda classe possui teste correspondente.
```

Critério de passagem:

```text
todos os testes passam;
todos os exemplos executam;
documentação compila;
outputs conferidos;
nenhum erro silencioso conhecido;
nenhum requisito P0 pendente;
toda classe/policy/writer criada possui teste GoogleTest.
```

## 4. Ordem resumida

```text
0. Fundação do projeto
1. Erros e exceções
2. Tipos fortes e configuração
3. GridAxis1D
4. Métricas 1D
5. Uniforme volume-centrada
6. Builder 1D
7. Output textual 1D
8. CSV 1D
9. Uniforme face-centrada
10. Aleatória volume-centrada
11. Policies, traits e concepts
12. YAML
13. Composição 2D por eixos
14. Output textual 2D por direção
15. CSV multidirecional
16. VTK rectilinear
17. Sphinx
18. Regressão geral
```

## 5. Filosofia do cronograma

```text
Cada bloco deve produzir uma biblioteca melhor, executável e testada.
Nenhum bloco deve depender de promessa futura para funcionar.
O avanço só ocorre quando o passo anterior está funcional, testado e verificável.
```
