
Estratégia de testes
====================

Esta página descreve a parte pública da estratégia de testes do
FVGridMaker. O objetivo não é listar todos os testes da suíte, mas
explicar quais contratos de comportamento são protegidos e como o usuário
ou desenvolvedor pode executar a validação localmente.

Os testes públicos devem ser lidos como documentação executável: eles
registram propriedades que a biblioteca deve preservar ao longo do
desenvolvimento. Detalhes internos de implementação podem mudar; os
contratos de comportamento da API pública não devem mudar sem decisão
explícita.

Objetivo dos testes
-------------------

A suíte de testes verifica três grupos de propriedades:

* invariantes geométricas, como ordenação de faces, positividade de
  medidas e consistência entre centros e faces;
* contratos de indexação e armazenamento, como a ordem linear
  ``row-major`` em malhas bidimensionais;
* operações de entrada, saída e manipulação de malhas, como escrita CSV,
  escrita VTK, interseção, recorte e relatórios de qualidade.

A documentação pública dos testes deve enfatizar essas propriedades, e
não a estrutura interna de cada caso de teste.

Como compilar os testes
-----------------------

Configure um diretório de build dedicado aos testes:

.. code-block:: bash

   cmake -S . -B build_tests \
     -DBUILD_TESTS=ON \
     -DBUILD_TESTING=ON \
     -DBUILD_EXAMPLES=OFF \
     -DBUILD_DOCUMENTATION=OFF

Em seguida, compile todos os testes:

.. code-block:: bash

   cmake --build build_tests -j

O projeto usa GoogleTest. Quando o GoogleTest não é encontrado no sistema,
o CMake pode baixá-lo automaticamente, desde que
``FVG_TESTS_FETCH_GOOGLETEST`` esteja ativado.

Como executar todos os testes
-----------------------------

Há duas formas usuais de executar a suíte.

A primeira usa o alvo agregado criado pelo projeto:

.. code-block:: bash

   cmake --build build_tests --target run_all_tests -j

A segunda usa diretamente o CTest:

.. code-block:: bash

   ctest --test-dir build_tests --output-on-failure

O CTest é útil quando se deseja integração com ferramentas de CI ou saída
padronizada por caso de teste.

Como executar um teste específico
---------------------------------

Cada arquivo de teste gera um executável próprio. Para rodar apenas um
grupo específico, use o alvo ``run_<nome_do_alvo>`` criado pelo CMake.

Por exemplo, se o alvo do teste for ``tst_Core_Types``, o comando será:

.. code-block:: bash

   cmake --build build_tests --target run_tst_Core_Types

O nome exato do alvo depende do caminho do arquivo dentro da pasta
``tests``. Em caso de dúvida, liste os alvos disponíveis:

.. code-block:: bash

   cmake --build build_tests --target help | grep tst_

Convenção de nomes
------------------

A suíte considera como pontos de entrada os arquivos com os padrões:

.. code-block:: text

   tst_*.cpp
   tst_*.cxx
   tst_*.cc

Esses arquivos devem ficar dentro da pasta ``tests``. O nome do arquivo e
o caminho relativo dentro dessa pasta são usados para construir o nome do
alvo CMake correspondente.

Testes como contrato público
----------------------------

A tabela abaixo resume quais comportamentos devem ser tratados como
contratos públicos da biblioteca.

.. list-table:: Contratos públicos protegidos pelos testes
   :header-rows: 1
   :widths: 28 72

   * - Componente
     - Contrato protegido
   * - ``Axis1D``
     - Contagem coerente de faces, centros e volumes; coordenadas finitas;
       faces ordenadas; centros compatíveis com as faces.
   * - Geradores 1D
     - Geração de eixos válidos para distribuições uniformes, aleatórias,
       de Roberts e customizadas; rejeição de parâmetros impossíveis.
   * - Padrões 1D
     - Coerência entre eixos centrados em faces e eixos centrados em volumes;
       preservação do domínio e dos tipos de coordenada.
   * - ``Operations1D``
     - Interseção de intervalos, recorte, tolerâncias válidas e tratamento
       de interseções vazias.
   * - ``StructuredGrid2D``
     - Composição tensorial de dois eixos 1D; indexação linear ``row-major``;
       contagem correta de células; acesso consistente a faces, centros,
       áreas lógicas e medidas físicas.
   * - Sistemas de coordenadas 2D
     - Diferença entre área lógica e medida física; fórmulas cartesianas,
       polares, axisimétricas cilíndricas, axisimétricas esféricas e
       mapeamentos customizados.
   * - ``Operations2D``
     - Caixa lógica do domínio, interseção entre malhas e recorte para uma
       caixa lógica válida.
   * - ``Quality2D``
     - Medidas mínimas e máximas, razões entre células, áreas computacionais
       e medidas de faces.
   * - Saída CSV/VTK
     - Criação de arquivos, ordem dos dados, conteúdo geométrico e falhas
       controladas de escrita.

O que não deve ser tratado como contrato público
------------------------------------------------

Nem todo detalhe testado deve ser documentado como garantia da API.
Em particular, os seguintes pontos podem mudar sem quebrar o contrato
público, desde que o comportamento observável seja preservado:

* nomes de funções auxiliares usadas apenas dentro dos testes;
* organização interna dos arquivos em subpastas;
* decomposição de um caso de teste em vários testes menores;
* mensagens internas muito específicas, quando o tipo de erro e o contexto
  continuarem claros;
* detalhes de implementação que não aparecem na interface pública.

Essa separação evita que a documentação pública congele decisões internas
do projeto. O papel da documentação é explicar o comportamento garantido,
não reproduzir a suíte linha por linha.

Verificação de memória
----------------------

Quando ``BUILD_MEMCHECK`` está ativado e o ``valgrind`` está disponível,
o projeto também pode criar alvos de verificação de memória. Para configurar
esse modo:

.. code-block:: bash

   cmake -S . -B build_memcheck \
     -DBUILD_TESTS=ON \
     -DBUILD_TESTING=ON \
     -DBUILD_MEMCHECK=ON \
     -DBUILD_EXAMPLES=OFF \
     -DBUILD_DOCUMENTATION=OFF

Para executar todos os alvos de verificação de memória:

.. code-block:: bash

   cmake --build build_memcheck --target memcheck_all_tests -j

Use essa etapa antes de alterações maiores em estruturas de armazenamento,
operações de escrita ou rotinas que manipulam muitos objetos temporários.

