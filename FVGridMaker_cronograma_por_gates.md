FVGridMaker — Cronograma por Gates
Este documento define o cronograma técnico atual do FVGridMaker. O avanço deve ocorrer por gates de qualidade, não por calendário.
Regra geral:
```text
Só avançamos para o próximo bloco quando o bloco anterior estiver compilando,
testado com GoogleTest, executável por CTest, documentado minimamente e com
exemplos funcionais quando a funcionalidade for pública.
```
1. Regras formais de avanço
```text
1. Nenhum bloco novo começa com teste quebrado.
2. Nenhum bloco novo começa com exemplo quebrado.
3. Nenhum bloco novo começa com documentação essencial ausente.
4. Nenhuma funcionalidade pública entra sem exemplo.
5. Nenhuma regra geométrica entra sem teste numérico.
6. Nenhuma distribution nova entra sem teste de invariantes.
7. Nenhum grid pattern novo entra sem teste.
8. Nenhuma operation nova entra sem teste.
9. Nenhum output entra sem arquivo de referência simples.
10. Nenhum erro esperado entra sem código textual testado.
11. Nenhum módulo periférico pode criar dependência reversa no núcleo.
12. Nenhuma categoria extensível deve ser implementada com enum.
13. Toda classe que possa ser origem de erro deve expor ID próprio.
14. require() deve receber ErrorDescriptor + ID sempre que possível.
```
2. Status resumido
```text
Bloco 0 — Fundação do projeto                  CONCLUÍDO
Bloco 1 — Core                                  CONCLUÍDO
Bloco 2 — ErrorHandling                         CONCLUÍDO
Bloco 3 — GridPattern1D básico                  CONCLUÍDO
Bloco 4 — Axis1D                                CONCLUÍDO
Bloco 5 — Uniform1D volume-centred              CONCLUÍDO
Bloco 6 — Output textual via operator<<         PRÓXIMO RECOMENDADO
Bloco 7 — Custom1D / Axis1D from complete data  PENDENTE
Bloco 8 — FaceCentered construction path        PENDENTE
Bloco 9 — Random1D                              PENDENTE
Bloco 10 — Operations1D                         PENDENTE
Bloco 11 — CoordinateSystem                     PENDENTE
Bloco 12 — StructuredGrid2D                     PENDENTE
```
---
Blocos concluídos
Bloco 0 — Fundação do projeto
Objetivo: criar um projeto mínimo, compilável e testável.
Entregas:
```text
estrutura inicial do projeto;
FVGridMakerLib/ como nova biblioteca;
FVGridMakerLibOld/ preservada como legado e fora do build;
README inicial;
LICENSE MIT;
.gitignore;
.clang-format;
exemplo mínimo;
teste mínimo;
alvos run_ex_Minimal e run_tst_Minimal.
```
Gate de saída:
```text
make run_ex_Minimal passa;
make run_tst_Minimal passa;
ctest --output-on-failure passa.
```
Status: concluído.
Bloco 1 — Core
Objetivo: criar a fundação tipada da biblioteca.
Entregas:
```text
Types.h;
StrongTypes.h;
Version.h;
ID.h;
Version.cc;
ID.cc;
testes de Types, StrongTypes, Version e ID.
```
Decisão atual sobre `ID`:
```text
ID é um valor imutável com module, class_name e class_id.
Não há enum de classes.
Não há herança virtual para identificação.
Não há macro DefineIdentity.
Toda classe deve expor sua própria identidade quando relevante.
```
Gate de saída:
```text
make run_tst_ID passa;
make run_tst_Types passa;
make run_tst_StrongTypes passa;
make run_tst_Version passa;
ctest --output-on-failure passa.
```
Status: concluído.
Bloco 2 — ErrorHandling
Objetivo: criar um sistema de erro padronizado, textual, extensível e sem `enum`.
Entregas:
```text
ErrorCodes.h;
ErrorDescriptor.h;
ErrorCatalog.h;
ErrorRecord.h;
FVGridException.h/.cc;
ThrowError.h/.cc;
exemplo Ex_ErrorHandling;
testes de ErrorCodes, ErrorCatalog, ErrorRecord, FVGridException e ThrowError.
```
Decisões:
```text
códigos de erro são std::string_view;
mensagens padrão vivem em ErrorCatalog;
ErrorDescriptor contém code, message e category;
ErrorRecord contém code, message, category, ID source e source_location;
FVGridException formata diagnóstico completo;
require() deve preferir ErrorDescriptor + ID;
erros externos podem usar descritores próprios sem registrar enum.
```
Uso interno preferencial:
```cpp
require(
    condition,
    error_catalog::kInvalidFaceCount,
    Axis1D::id()
);
```
Gate de saída:
```text
make run_tst_ErrorCodes passa;
make run_tst_ErrorCatalog passa;
make run_tst_ErrorRecord passa;
make run_tst_FVGridException passa;
make run_tst_ThrowError passa;
make run_ex_ErrorHandling passa;
ctest --output-on-failure passa.
```
Status: concluído.
Bloco 3 — GridPattern1D básico
Objetivo: criar descritores de padrão 1D e regras iniciais de reconstrução.
Entregas:
```text
VolumeCentered1D.h/.cpp;
FaceCentered1D.h/.cpp;
tst_GridPattern1D.cc.
```
Regras atuais:
```text
VolumeCentered1D:
  primary_coordinates   = faces
  secondary_coordinates = centers
  centers_from_faces(faces)

FaceCentered1D:
  primary_coordinates   = centers
  secondary_coordinates = faces
  faces_from_centers(centers, x_min, x_max) prevista para uso futuro
```
Observação: `Uniform1D` foi mantido restrito ao caminho volume-centred neste momento.
Gate de saída:
```text
make run_tst_GridPattern1D passa;
ctest --output-on-failure passa.
```
Status: concluído.
Bloco 4 — Axis1D
Objetivo: criar a representação geométrica 1D fundamental.
Entregas:
```text
Axis1D.h/.cpp;
tst_Axis1D.cc;
ex_Axis1D.cc.
```
`Axis1D` armazena:
```text
faces       tamanho nvol + 1;
centers     tamanho nvol;
dx_faces    tamanho nvol;
dx_centers  tamanho nvol + 1;
pattern_name.
```
Métricas:
```text
dxface[i]       = xface[i + 1] - xface[i]
dxcenter[0]     = xcenter[0] - xface[0]
dxcenter[i]     = xcenter[i] - xcenter[i - 1]
dxcenter[nvol]  = xface[nvol] - xcenter[nvol - 1]
```
Decisão arquitetural:
```text
Axis1D armazena geometria completa e calcula métricas gerais.
Axis1D não deve conter regras específicas de reconstrução de padrões.
VolumeCentered1D calcula centers a partir de faces.
FaceCentered1D calculará faces a partir de centers quando esse caminho for ativado.
```
Gate de saída:
```text
make run_tst_Axis1D passa;
make run_ex_Axis1D passa;
ctest --output-on-failure passa.
```
Status: concluído.
Bloco 5 — Uniform1D volume-centred
Objetivo: criar geração uniforme 1D no caminho volume-centred.
Entregas:
```text
Uniform1D.h/.cc;
tst_Uniform1D.cc;
ex_Uniform1D.cc.
```
Regra implementada:
```text
1. Uniform1D recebe NVol, Length e XInit.
2. Uniform1D gera faces uniformes.
3. VolumeCentered1D calcula centers a partir das faces.
4. Axis1D recebe faces + centers.
5. Axis1D calcula dx_faces e dx_centers.
```
Exemplo público:
```text
make run_ex_Uniform1D
```
A saída deve mostrar uma tabela com:
```text
i, xface[i], xcenter[i], dxface[i], dxcenter[i]
```
Gate de saída:
```text
make run_tst_Uniform1D passa;
make run_ex_Uniform1D passa;
make run_tst_GridPattern1D passa;
make run_tst_Axis1D passa;
ctest --output-on-failure passa.
```
Status: concluído.
---
Próximos blocos recomendados
Bloco 6 — Output textual via `operator<<`
Objetivo: permitir impressão simples de `Axis1D`.
Uso desejado:
```cpp
std::cout << axis << '\n';
```
Entregas:
```text
operator<<(std::ostream&, const Axis1D&);
teste de streaming em tst_Axis1D.cc;
Ex_Uniform1D simplificado para usar std::cout << axis.
```
Gate de saída:
```text
make run_tst_Axis1D passa;
make run_ex_Uniform1D passa;
ctest --output-on-failure passa.
```
Bloco 7 — Custom1D / Axis1D from complete data
Objetivo: estabilizar a construção a partir de `faces + centers` dados pelo usuário.
Entregas previstas:
```text
Custom1D ou helpers equivalentes;
validação explícita de faces e centers;
testes de geometrias não uniformes;
testes de dx_faces e dx_centers;
exemplo público.
```
Bloco 8 — Face-centred construction path
Objetivo: implementar corretamente construção face-centred em bloco próprio.
Regra desejada:
```text
1. distribuição gera centers;
2. FaceCentered1D calcula faces;
3. Axis1D recebe faces + centers;
4. Axis1D calcula métricas.
```
Atenção: não misturar este caminho com `Uniform1D` volume-centred enquanto os testes e a API não estiverem claros.
Bloco 9 — Random1D
Objetivo: criar distribuição 1D aleatória/controlada.
Pontos pendentes:
```text
seed;
monotonicidade;
controle de mínimo espaçamento;
uso de GridPattern1D quando a coordenada primária variar;
testes de reprodutibilidade.
```
Bloco 10 — Operations1D
Objetivo: operações entre eixos 1D.
Inclui:
```text
intersecção geométrica;
merge/soma de eixos;
remoção de duplicatas;
remoção de volumes pequenos;
validação de padrões compatíveis.
```
Regra já decidida:
```text
A intersecção geométrica entre duas grades não pode ser vazia.
Pode ser um ponto/faces coincidentes.
Pode ser um intervalo.
```
Bloco 11 — CoordinateSystem
Objetivo: calcular medidas geométricas físicas a partir de eixos estruturados.
Decisão:
```text
CoordinateSystem não gera grid.
CoordinateSystem interpreta eixos e calcula medidas.
Deve ser extensível e sem enum central.
```
Bloco 12 — StructuredGrid2D
Objetivo: compor dois `Axis1D` em uma grade estruturada 2D.
Dependências:
```text
Axis1D estável;
CoordinateSystem básico;
regras de indexação 2D;
medidas geométricas.
```