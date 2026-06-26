A base factual para o cronograma é esta: o projeto já declara como princípios **C++20**, armazenamento contíguo, `std::span`, ausência de polimorfismo em hot paths, ausência de enums para conceitos extensíveis e separação entre core, YAML, output, testes e documentação.  O header público, porém, já expõe módulos além do 1D básico, incluindo `Roberts1D`, `Operations1D`, CSV, VTK e 2D.  Então o cronograma abaixo organiza o fechamento por dependência técnica, não por data.

# Cronograma técnico para encerramento da programação da FVGridMaker

## Princípios fixos do fechamento

A FVGridMaker deve ser encerrada como uma biblioteca C++20 para geração, armazenamento, validação, operação e exportação de grids estruturados para volumes finitos.

As decisões arquiteturais fixas são:

```text
1. Data-Oriented Design nos caminhos geométricos e numéricos.
2. Armazenamento contíguo em std::vector.
3. Exposição de leitura por std::span.
4. Ausência de classes virtuais no core.
5. Ausência de herança dinâmica em hot paths.
6. Uso de classes final, funções livres, factories, concepts e traits.
7. Separação entre armazenamento geométrico e regra de construção.
8. Separação entre biblioteca, exemplos, testes, documentação e ferramentas externas.
9. Nenhuma dependência YAML dentro de FVGridMakerLib.
10. API pública pequena, estável e coerente com a documentação.
```

## Gate 1, estabilização do escopo público

Objetivo: decidir exatamente o que entra na versão final da biblioteca.

Atividades:

```text
1. Definir se a versão final será 1D apenas, 1D + 2D experimental, ou 1D + 2D oficial.
2. Classificar cada módulo como público, experimental ou interno.
3. Revisar FVGridMaker.h para exportar apenas o que deve ser API pública.
4. Decidir se Roberts1D, Operations1D, CSV, VTK e StructuredGrid2D entram como módulos oficiais.
5. Remover do umbrella header qualquer módulo que ainda não esteja pronto para ser usado externamente.
6. Atualizar README para refletir exatamente o escopo implementado.
7. Atualizar FVGridMaker_requisitos.md para deixar de tratar como futuro o que já está implementado.
```

Critério de fechamento:

```text
O header público, o README, os requisitos e os exemplos dizem a mesma coisa sobre o que a biblioteca faz.
```

Dependências liberadas:

```text
Depois deste gate, é possível fechar a API sem continuar movendo classes entre público, experimental e interno.
```

## Gate 2, correção de versionamento e identidade do projeto

Objetivo: eliminar ambiguidade entre versão de CMake, cabeçalhos, documentação e release.

Atividades:

```text
1. Escolher a versão real da biblioteca.
2. Corrigir project(... VERSION ...) no CMake.
3. Corrigir os cabeçalhos que ainda indicam versão diferente.
4. Padronizar Version.h, version.hpp gerado e função fvgrid::version().
5. Definir se a versão será 0.x ou 1.0.0.
6. Garantir que o target info imprima versão, hash Git e estado dirty corretamente.
7. Remover scripts ou mecanismos de release que estejam desatualizados.
```

Recomendação:

```text
Não usar 1.0.0 se a API ainda estiver em consolidação.
Usar 0.2.0 ou 0.3.0 se 1D, Roberts, Operations, CSV e parte 2D forem incluídos.
```

Critério de fechamento:

```text
Toda referência de versão no projeto aponta para a mesma versão semântica.
```

Dependências liberadas:

```text
Depois deste gate, pode-se preparar release, documentação técnica e pacote de registro sem contradição.
```

## Gate 3, fechamento do Core

Objetivo: estabilizar os tipos fundamentais, identidade e sistema de erro.

Atividades:

```text
1. Revisar Types.h.
2. Revisar StrongTypes.h.
3. Revisar ID.h.
4. Revisar Version.h.
5. Confirmar que Real, Size e Index são suficientes para todos os módulos atuais.
6. Confirmar que NVol, Length, XInit, XFinal, MinSpacing, Beta e Seed cobrem as entradas públicas.
7. Decidir se strong types validam somente tipo ou também valor.
8. Manter validação de valor nas fábricas, salvo motivo forte para mover validação para os próprios strong types.
9. Limpar comentários mortos em headers e fontes.
```

Critério de fechamento:

```text
Core não depende de OneDimensional, TwoDimensional, Output, Tests, Examples ou Docs.
```

Dependências liberadas:

```text
Axis1D, distribuições 1D, operações, 2D e output passam a depender de uma fundação estável.
```

## Gate 4, fechamento do sistema de erros

Objetivo: estabilizar diagnóstico, exceções e contratos de erro.

Atividades:

```text
1. Revisar ErrorCodes.
2. Revisar ErrorDescriptor.
3. Revisar ErrorCatalog.
4. Revisar ErrorRecord.
5. Revisar FVGridException.
6. Revisar throw_error() e require().
7. Remover códigos de erro que pertençam a YAML ou exemplos, se não forem usados pelo core.
8. Garantir que toda classe capaz de lançar erro exponha id().
9. Confirmar que source_location está correto em todos os caminhos relevantes.
10. Padronizar mensagens em inglês.
```

Critério de fechamento:

```text
Todo erro lançado pela biblioteca produz código textual estável, categoria, mensagem, classe de origem e localização.
```

Dependências liberadas:

```text
Todos os módulos avançados podem usar diagnóstico consistente sem criar sistemas próprios de erro.
```

## Gate 5, fechamento de Axis1D

Objetivo: estabilizar a unidade geométrica fundamental.

Atividades:

```text
1. Revisar construtores de Axis1D.
2. Decidir formalmente o significado de pattern_name.
3. Evitar que pattern_name prometa uma regra geométrica que não foi realmente usada.
4. Considerar remover ou renomear Axis1D(faces, pattern_name).
5. Manter Axis1D como armazenamento de geometria completa, não como fábrica de padrões.
6. Garantir validação de faces, centers, finitude, monotonicidade e interioridade dos centros.
7. Confirmar tamanhos:
   faces      = nvol + 1
   centers    = nvol
   dx_faces   = nvol
   dx_centers = nvol + 1
8. Confirmar que cell_lengths() é alias de leitura para dx_faces().
9. Padronizar saída operator<<.
10. Completar testes de borda para NaN, infinito e domínios degenerados.
```

Critério de fechamento:

```text
Axis1D só armazena geometria validada e não contém regra específica de geração, exceto se houver construtor explicitamente documentado para isso.
```

Dependências liberadas:

```text
Uniform1D, Random1D, Roberts1D, Custom1D, Operations1D, CSV e StructuredGrid2D passam a depender de um eixo fechado.
```

## Gate 6, fechamento dos padrões 1D

Objetivo: estabilizar VolumeCentered1D, FaceCentered1D e o contrato de GridPattern1D.

Atividades:

```text
1. Revisar CoordinateKind1D.
2. Revisar Coordinates1D.
3. Revisar Domain1D.
4. Revisar AxisGeometry1D.
5. Revisar VolumeCentered1D.
6. Revisar FaceCentered1D.
7. Criar ou consolidar um concept para padrões 1D.
8. Exigir input_kind() e complete_geometry().
9. Decidir se name(), primary_coordinates() e secondary_coordinates() também serão obrigatórios.
10. Garantir que FaceCentered1D rejeite ausência de domínio quando precisar de fronteiras.
11. Garantir que VolumeCentered1D não dependa de domínio quando não precisar dele.
```

Critério de fechamento:

```text
Um novo padrão 1D pode ser adicionado sem modificar Uniform1D, Random1D ou Custom1D, desde que satisfaça o concept exigido.
```

Dependências liberadas:

```text
As distribuições 1D podem ser fechadas com traits/concepts em vez de herança virtual.
```

## Gate 7, fechamento das distribuições 1D

Objetivo: estabilizar as formas de geração de eixos 1D.

Atividades:

```text
1. Revisar Uniform1D.
2. Revisar Random1D.
3. Revisar Roberts1D.
4. Revisar Custom1D.
5. Confirmar que todas as distribuições aceitam padrões por template, não por ponteiros para base virtual.
6. Confirmar que a chamada sem padrão usa VolumeCentered1D como padrão.
7. Documentar a interpretação estatística de Random1D.
8. Documentar a fórmula e o domínio de validade de Roberts1D.
9. Confirmar que Roberts1D usa beta > 1.
10. Confirmar que Random1D respeita min_spacing.
11. Confirmar reprodutibilidade por Seed.
12. Adicionar testes para casos extremos:
    nvol = 1
    length muito pequeno
    min_spacing no limite exato
    beta próximo de 1
    beta grande
```

Critério de fechamento:

```text
Uniform1D, Random1D, Roberts1D e Custom1D geram Axis1D válido para padrões compatíveis sem conhecer classes derivadas ou bases virtuais.
```

Dependências liberadas:

```text
Operations1D, exemplos 1D, CSV 1D e composição 2D podem ser finalizados sobre geradores estáveis.
```

## Gate 8, fechamento de Operations1D

Objetivo: consolidar operações geométricas sobre eixos já construídos.

Atividades:

```text
1. Revisar AxisInterval1D.
2. Revisar Operations1D.
3. Confirmar semântica de empty, point e interval.
4. Confirmar tolerância em intersection().
5. Confirmar comportamento de require_interval_intersection().
6. Confirmar unique_sorted_coordinates() com tolerância zero e tolerância positiva.
7. Revisar clip_faces_to_interval().
8. Decidir se o eixo recortado deve preservar pattern_name original ou receber pattern_name próprio.
9. Adicionar testes de interseção sem sobreposição, interseção pontual e interseção com tolerância.
10. Adicionar testes de clipping com faces exatamente nas fronteiras do intervalo.
```

Critério de fechamento:

```text
Operations1D opera somente sobre Axis1D validado e sempre retorna resultados geométricos bem definidos.
```

Dependências liberadas:

```text
Operações de composição, comparação e recorte passam a poder ser usadas por exemplos, output e futuras rotinas 2D.
```

## Gate 9, fechamento de Output 1D

Objetivo: estabilizar exportação simples de eixos.

Atividades:

```text
1. Revisar Axis1DCSVWriter.
2. Decidir nomes finais das colunas.
3. Padronizar precisão numérica.
4. Garantir que diretórios sejam criados quando necessário.
5. Garantir erro claro quando arquivo não puder ser aberto ou escrito.
6. Adicionar teste de escrita em stream.
7. Adicionar teste de escrita em arquivo temporário.
8. Documentar o formato CSV.
```

Critério de fechamento:

```text
Um Axis1D pode ser exportado para CSV de forma reprodutível, documentada e testada.
```

Dependências liberadas:

```text
Exemplos, documentação e material do livro podem usar saída CSV como formato de conferência.
```

## Gate 10, fechamento dos traits de coordenadas 2D

Objetivo: estabilizar o contrato matemático para sistemas de coordenadas 2D.

Atividades:

```text
1. Revisar CoordinateSystem2D.
2. Revisar CoordinateMetrics2D.
3. Separar claramente:
   mapeamento computacional -> físico
   medida de célula
   medida da primeira família de faces
   medida da segunda família de faces
4. Confirmar traits para cartesiano.
5. Confirmar traits para polar.
6. Confirmar traits para axisimétrico cilíndrico.
7. Confirmar traits para axisimétrico esférico, se permanecer no escopo.
8. Verificar se nomes first/second são suficientes ou se convém usar coordinate tags.
9. Evitar std::function em hot paths.
10. Manter FunctionalCoordinateMapping2D como template concreto, sem virtual.
11. Testar medidas analíticas contra casos conhecidos.
12. Melhorar formatação e estilo dos headers 2D.
```

Critério de fechamento:

```text
Um sistema de coordenadas 2D é definido por traits/concepts, não por classe base virtual.
```

Dependências liberadas:

```text
StructuredGrid2D pode calcular medidas físicas sem depender de polimorfismo dinâmico.
```

## Gate 11, fechamento de StructuredGrid2D

Objetivo: estabilizar composição bidimensional por dois Axis1D independentes.

Atividades:

```text
1. Revisar StructuredGrid2D.
2. Confirmar indexação linear row-major.
3. Confirmar nomes x_axis/y_axis versus first_axis/second_axis.
4. Decidir se x_axis/y_axis devem existir apenas para cartesiano ou como aliases genéricos.
5. Revisar armazenamento de physical_face_points.
6. Revisar cell_measures.
7. Revisar first_face_measures e second_face_measures.
8. Garantir validação de índices em todas as consultas.
9. Garantir que medidas físicas sejam positivas ou não negativas conforme o caso.
10. Testar cartesiano uniforme e não uniforme.
11. Testar polar com domínio radial positivo.
12. Testar axisimétrico.
13. Testar mapeamento funcional definido pelo usuário.
14. Padronizar estilo de código para o mesmo padrão do módulo 1D.
```

Critério de fechamento:

```text
StructuredGrid2D representa uma malha estruturada 2D por produto tensorial de dois Axis1D e calcula métricas sem herança virtual.
```

Dependências liberadas:

```text
Exportação VTK 2D e exemplos 2D podem ser fechados sobre uma malha estável.
```

## Gate 12, fechamento de Output 2D

Objetivo: estabilizar exportação de grids 2D.

Atividades:

```text
1. Revisar LegacyVTKRectilinearGrid2DWriter.
2. Decidir se o writer suporta apenas grids VTK rectilinear ou também grids curvilíneos.
3. Se suportar apenas rectilinear, rejeitar explicitamente mapeamentos não retangulares.
4. Se suportar curvilíneo, renomear writer e ajustar formato VTK.
5. Adicionar teste de arquivo VTK mínimo.
6. Adicionar teste para dimensões e número de pontos/células.
7. Adicionar teste para rejeição de grid incompatível.
8. Documentar limitações do formato.
```

Critério de fechamento:

```text
A saída VTK exporta apenas o que o nome do writer promete, sem aceitar silenciosamente geometrias incompatíveis.
```

Dependências liberadas:

```text
Exemplos visuais e documentação de uso com ParaView podem ser finalizados.
```

## Gate 13, exemplos oficiais

Objetivo: garantir que cada funcionalidade pública tenha exemplo mínimo.

Atividades:

```text
1. Revisar examples/README.md.
2. Criar exemplo mínimo de Axis1D.
3. Criar exemplo de Uniform1D.
4. Criar exemplo de Random1D.
5. Criar exemplo de Roberts1D.
6. Criar exemplo de Custom1D.
7. Criar exemplo de Operations1D.
8. Criar exemplo de CSV 1D.
9. Criar exemplo de StructuredGrid2D cartesiano.
10. Criar exemplo de StructuredGrid2D polar, se polar for oficial.
11. Criar exemplo de VTK 2D, se VTK for oficial.
12. Remover exemplos que dependam de API experimental não documentada.
13. Garantir que exemplos sejam pequenos e adequados ao livro.
```

Critério de fechamento:

```text
Todo recurso público tem pelo menos um exemplo curto, compilável e executável.
```

Dependências liberadas:

```text
Documentação do usuário pode referenciar exemplos reais em vez de pseudocódigo.
```

## Gate 14, testes e gates de qualidade

Objetivo: fechar a biblioteca com evidência objetiva de funcionamento.

Atividades:

```text
1. Rodar build de testes.
2. Rodar ctest.
3. Rodar build de exemplos.
4. Rodar exemplos oficiais.
5. Rodar sanitizers em Debug.
6. Rodar build Release.
7. Rodar Valgrind quando aplicável.
8. Criar teste de instalação.
9. Criar teste de uso externo com find_package(FVGridMaker).
10. Adicionar GitHub Actions, se ainda não houver.
11. Testar pelo menos GCC e Clang.
12. Registrar no README os comandos oficiais de verificação.
```

Critério de fechamento:

```text
A biblioteca compila, testa, instala e pode ser consumida por um projeto externo mínimo.
```

Dependências liberadas:

```text
A versão pode ser considerada candidata a release e a registro.
```

## Gate 15, instalação e empacotamento CMake

Objetivo: permitir uso externo limpo.

Atividades:

```text
1. Revisar ConfigInstall.cmake.
2. Revisar FVGridMakerConfig.cmake.in.
3. Confirmar instalação de headers.
4. Confirmar instalação do target exportado.
5. Confirmar namespace FVGridMaker::FVGridMaker.
6. Confirmar version file.
7. Criar exemplo externo mínimo:
   find_package(FVGridMaker REQUIRED)
   target_link_libraries(app PRIVATE FVGridMaker::FVGridMaker)
8. Testar instalação local em prefixo temporário.
9. Testar consumo externo fora da árvore do repositório.
```

Critério de fechamento:

```text
Um usuário externo consegue instalar e usar a biblioteca sem conhecer a árvore interna do repositório.
```

Dependências liberadas:

```text
Manual técnico e release passam a poder descrever instalação real.
```

## Gate 16, documentação técnica

Objetivo: alinhar documentação com código final.

Atividades:

```text
1. Atualizar README.
2. Atualizar requisitos.
3. Atualizar árvore do projeto.
4. Atualizar documentação de build.
5. Atualizar documentação de testes.
6. Atualizar documentação de exemplos.
7. Escrever manual técnico curto.
8. Escrever seção de arquitetura.
9. Escrever seção de API 1D.
10. Escrever seção de API 2D, se entrar no escopo.
11. Escrever seção de output, se entrar no escopo.
12. Documentar explicitamente limitações conhecidas.
13. Documentar que não há classes virtuais no core.
14. Documentar o uso de concepts, traits e factories.
```

Critério de fechamento:

```text
A documentação descreve apenas funcionalidades existentes, testadas e públicas.
```

Dependências liberadas:

```text
A biblioteca fica pronta para release, registro e uso como material de apoio do livro.
```

## Gate 17, limpeza final de repositório

Objetivo: remover ruído antes de congelar a versão.

Atividades:

```text
1. Remover arquivos antigos que não entram no pacote final.
2. Remover diretórios experimentais não usados.
3. Garantir que scripts locais estejam no .gitignore, se não forem parte do projeto.
4. Remover comentários mortos.
5. Padronizar cabeçalhos de arquivos.
6. Padronizar nomes de arquivos .cc, .cpp ou .cxx.
7. Padronizar includes.
8. Rodar formatação, se houver ferramenta adotada.
9. Revisar licença.
10. Revisar autoria.
```

Critério de fechamento:

```text
O repositório contém apenas código, exemplos, testes e documentação que pertencem à versão final.
```

Dependências liberadas:

```text
Pode-se criar tag, ZIP, hash e pacote de registro.
```

## Gate 18, candidato a release

Objetivo: congelar uma versão tecnicamente coerente.

Atividades:

```text
1. Criar branch de release.
2. Rodar todos os gates de build, teste, exemplo e instalação.
3. Corrigir apenas bugs bloqueadores.
4. Atualizar changelog, se existir.
5. Definir tag.
6. Criar pacote fonte.
7. Conferir se o pacote contém apenas arquivos desejados.
8. Só depois gerar hash, quando for o momento do registro.
```

Critério de fechamento:

```text
A versão candidata pode ser reconstruída, testada, instalada e documentada de forma reprodutível.
```

## Ordem resumida de execução

A ordem recomendada é:

```text
1. Escopo público
2. Versionamento
3. Core
4. Sistema de erros
5. Axis1D
6. GridPattern1D
7. Distribuições 1D
8. Operations1D
9. Output 1D
10. Coordinate traits 2D
11. StructuredGrid2D
12. Output 2D
13. Exemplos oficiais
14. Testes e qualidade
15. Instalação e empacotamento
16. Documentação técnica
17. Limpeza de repositório
18. Candidato a release
```

## Regra de dependência principal

Nenhuma classe avançada deve ser fechada antes de seus contratos inferiores estarem estáveis.

```text
Core
  -> ErrorHandling
      -> Axis1D
          -> GridPattern1D
              -> Distributions1D
                  -> Operations1D
                      -> Output1D

Axis1D + CoordinateSystem2D traits
  -> StructuredGrid2D
      -> Output2D
```

## Regra arquitetural final

Nenhum novo recurso deve introduzir:

```text
1. classe base virtual;
2. ponteiro para interface polimórfica no core;
3. herança obrigatória para extensão;
4. armazenamento fragmentado em hot path;
5. dependência YAML dentro de FVGridMakerLib;
6. enum para famílias extensíveis;
7. API pública maior que a necessária.
```

Quando for necessário variar comportamento, usar preferencialmente:

```text
1. concepts;
2. traits;
3. templates;
4. factories;
5. funções livres;
6. classes final pequenas;
7. composição por valor;
8. armazenamento contíguo.
```

Eu colocaria como prioridade imediata os **Gates 1 a 5**. Eles destravam todo o restante. Sem fechar escopo, versão, core, erros e `Axis1D`, qualquer ajuste em `2D`, `VTK` ou documentação final pode precisar ser refeito.
