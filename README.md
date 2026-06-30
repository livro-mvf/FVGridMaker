# FVGridMaker

FVGridMaker é uma biblioteca C++20 para construir geometrias de malhas estruturadas de volumes finitos. Ela fornece eixos 1D, malhas estruturadas 2D por produto tensorial, sistemas de coordenadas cartesiano, polar e axisimétricos, medidas geométricas, validação de invariantes e saída CSV/VTK legacy.

A biblioteca tem foco acadêmico. A API procura deixar explícitas as hipóteses geométricas da malha antes de qualquer solver, campo físico ou operador discreto entrar no problema.

## Escopo

FVGridMaker trata de:

```text
malhas estruturadas de volumes finitos;
eixos unidimensionais;
malhas 2D por produto tensorial de dois eixos 1D;
faces, centros, distâncias e medidas geométricas;
padrões de reconstrução face centrada e volume centrada;
sistemas de coordenadas cartesiano, polar e axisimétricos;
validação de invariantes geométricos;
saída CSV para eixos 1D;
saída VTK legacy para malhas 2D.
```

FVGridMaker não trata de:

```text
equações diferenciais;
campos físicos;
condições de contorno;
operadores de difusão/advecção;
montagem de matriz;
solvers lineares;
integração temporal;
malhas não estruturadas;
YAML ou outros parsers de configuração no núcleo da biblioteca.
```

## Documentação

As instruções de compilação, execução, testes, exemplos e programas do manual ficam na documentação Sphinx/HTML e no manual da biblioteca. Este README é apenas a porta de entrada do repositório.

Pontos principais:

```text
docs/index.rst                         página inicial da documentação Sphinx
docs/getting_started.rst               início rápido e instruções de build
docs/examples/index.rst                exemplos oficiais ex_*
docs/manual/index.rst                  programas completos do manual man_*
docs/user_guide/index.rst              guia de uso da API
docs/architecture/index.rst            decisões de arquitetura
docs/development/index.rst             testes e desenvolvimento
docs/api/index.rst                     referência da API gerada por Doxygen
docs/_static/pdf/Manual_do_FVGridMaker.pdf
```

A documentação HTML já gerada fica na pasta `html/`. Para acessá-la, basta abrir:

```text
html/index.html
```

## Organização

```text
FVGridMakerLib/       biblioteca principal
examples/             exemplos curtos e oficiais, com prefixo ex_*
manual/Grid1D/        programas 1D completos do manual, com prefixo man_*
manual/Grid2D/        programas 2D completos do manual, com prefixo man_*
tests/                testes unitários e de invariantes
docs/                 fontes da documentação Sphinx/Doxygen/Breathe
html/                 documentação HTML gerada; abra html/index.html
cmake/                configuração modular de build
```

## Convenções dos códigos didáticos

Os exemplos em `examples/` são pequenos, independentes e usam apenas a API pública. Eles seguem o padrão `ex_*` e são material de consulta rápida.

Os programas em `manual/Grid1D/` e `manual/Grid2D/` são mais completos e comentados. Eles seguem o padrão `man_*`, trazem verificações de consistência e produzem saídas textuais voltadas para o manual.

Arquivos antigos fora desses padrões não devem ser usados como exemplos novos.

## Princípios de projeto

```text
C++20;
valores sempre válidos;
armazenamento contíguo;
sem polimorfismo virtual no caminho geométrico;
conceitos e traits para extensão;
sem dependência de FVMaker;
sem dependência real na biblioteca VTK;
separação entre geometria de malha e solver numérico.
```
