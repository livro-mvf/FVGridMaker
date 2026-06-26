# FVGridMaker — Cronograma por Gates

Este documento registra o avanço arquitetural do FVGridMaker por gates. Ele não substitui `FVGridMaker_requisitos.md`; serve apenas como controle editorial e técnico das fases de desenvolvimento.

## Estado geral

```text
Gate 1 — Escopo público                              concluído
Gate 2 — Versionamento                               concluído
Gate 3 — Core                                        concluído
Gate 4 — ErrorHandling                               concluído
Gate 5 — Output/Formatting                           concluído
Gate 6 — Documentação e limpeza de artefatos antigos em andamento
```

## Gate 1 — Escopo público

Estado:

```text
concluído
```

Objetivo:

```text
definir o escopo atual da biblioteca;
separar API estável, API experimental, módulos internos e trabalho futuro;
manter YAML fora da biblioteca;
classificar 2D e VTK como experimentais.
```

Resultado:

```text
Core definido como estável;
ErrorHandling definido como estável;
OneDimensional definido como estável;
Axis1DCSVWriter definido como estável;
TwoDimensional definido como experimental;
LegacyVTKRectilinearGrid2DWriter definido como experimental;
YAML restrito a exemplos ou aplicações externas.
```

## Gate 2 — Versionamento

Estado:

```text
concluído
```

Objetivo:

```text
centralizar a versão semântica no CMake;
expor versão pública por Core/Version.h;
gerar metadados Git durante a configuração;
remover versões literais de cabeçalhos.
```

Resultado:

```text
CMakeLists.txt é a fonte canônica de versão;
Version.h expõe accessors públicos;
version.hpp gerado contém metadados de build;
cabeçalhos devem usar Version: see <FVGridMaker/Core/Version.h>.
```

## Gate 3 — Core

Estado:

```text
concluído
```

Objetivo:

```text
estabilizar tipos fundamentais;
estabilizar strong types;
estabilizar identidade de componentes;
evitar enum central para identificação de classes.
```

Resultado:

```text
Types.h;
StrongTypes.h;
ID.h;
Version.h.
```

Decisão:

```text
componentes que podem originar erro devem expor ID próprio;
std::source_location rastreia função e linha;
não há enum central de classes.
```

## Gate 4 — ErrorHandling

Estado:

```text
concluído
```

Objetivo:

```text
substituir a arquitetura centralizada de erros por descritores tipados;
manter códigos textuais estáveis;
permitir extensão externa;
adicionar contexto estruturado;
remover catálogo global;
remover enum central de erro.
```

Resultado:

```text
BuiltInErrors.h;
ErrorContext.h;
ErrorDescriptor.h;
ErrorRecord.h;
ErrorTraits.h;
FVGridException.h;
ThrowError.h header-only.
```

Forma interna preferencial:

```cpp
require<errors::grid::InvalidNVol>(
    condition,
    Uniform1D::id()
);
```

Forma com contexto:

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

Critério de fechamento:

```text
biblioteca compila;
exemplos compilam;
testes passam;
exemplos migrados;
testes antigos removidos ou substituídos;
documentação principal atualizada.
```

Status:

```text
fechado
```

## Gate 5 — Output/Formatting

Estado:

```text
concluído
```

Objetivo:

```text
remover duplicação de travessia tabular em Axis1D;
manter contratos separados para saída humana e CSV;
proteger os contratos por testes.
```

Resultado:

```text
Axis1DRows.h criado como helper interno;
operator<< de Axis1D usa travessia comum;
Axis1DCSVWriter usa travessia comum;
contrato humano protegido por teste;
contrato CSV protegido por teste.
```

Decisão:

```text
operator<< não deve chamar Axis1DCSVWriter;
Axis1DCSVWriter não deve chamar operator<<;
ambos podem compartilhar uma rotina interna de travessia.
```

Contratos protegidos:

```text
operator<< contém cabeçalho humano esperado;
operator<< imprime a última face sem centro e sem comprimento de célula;
Axis1DCSVWriter escreve exatamente o cabeçalho i,xF,xC,Dx,dx;
Axis1DCSVWriter escreve a última linha com campos vazios para xC e Dx.
```

Status:

```text
fechado
```

## Gate 6 — Documentação e limpeza de artefatos antigos

Estado:

```text
em andamento
```

Objetivo:

```text
sincronizar README.md;
sincronizar FVGridMaker_tree.md;
sincronizar FVGridMaker_cronograma_por_gates.md;
garantir que a documentação auxiliar descreve a arquitetura atual;
remover descrições ativas de APIs antigas;
garantir que YAML apareça apenas como recurso externo ou futuro.
```

Arquivos-alvo:

```text
README.md;
FVGridMaker_tree.md;
FVGridMaker_cronograma_por_gates.md.
```

Critério de fechamento:

```text
README.md descreve a API atual;
FVGridMaker_tree.md lista os arquivos atuais;
cronograma marca Gates 1–5 como concluídos;
documentos auxiliares não apresentam APIs removidas como atuais;
testes continuam passando.
```

Comandos de validação:

```bash
git grep -n "error_catalog::\|error_code::" README.md FVGridMaker_tree.md FVGridMaker_cronograma_por_gates.md
git grep -n "AxisGrid1D" README.md FVGridMaker_tree.md FVGridMaker_cronograma_por_gates.md
cmake --build build --target run_all_tests
```

Status esperado após este gate:

```text
fechado
```

## Próximo gate candidato — Consumo externo

Objetivo:

```text
validar instalação;
validar uso por projeto externo mínimo;
validar headers públicos;
validar targets CMake exportáveis;
validar find_package quando implementado.
```

## Próximo gate candidato — Semântica de pattern_name

Objetivo:

```text
decidir se pattern_name representa o padrão geométrico original;
decidir se pattern_name representa a operação que gerou o eixo;
definir eventual histórico de operações.
```

## Próximo gate candidato — Estabilização 2D

Objetivo:

```text
decidir se StructuredGrid2D será promovido a API estável;
consolidar sistemas de coordenadas 2D;
consolidar métricas 2D;
reforçar testes de VTK 2D;
definir contratos de output 2D.
```

## Regra geral de fechamento de gate

Um gate só deve ser fechado quando:

```text
biblioteca compila;
exemplos compilam;
testes passam;
documentação relevante foi atualizada;
git contém apenas alterações intencionais prontas para commit.
```
