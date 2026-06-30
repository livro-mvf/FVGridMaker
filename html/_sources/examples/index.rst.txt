Exemplos
========

Os exemplos ativos têm nome ``ex_*`` e são compilados com
``BUILD_EXAMPLES=ON``. Eles são exemplos curtos, usados para demonstrar a API
sem alongar a leitura com comentários de manual.

Os programas completos e mais comentados ficam em :doc:`../manual/index`.
Eles seguem outro padrão: são material didático, com explicações no próprio
código, verificações simples e saídas textuais voltadas à leitura no manual.

Conteúdo atual
--------------

.. list-table::
   :header-rows: 1

   * - Exemplo
     - Conteúdo
   * - ``ex_Axis1D``
     - inspeção de faces, centros e distâncias
   * - ``ex_Uniform1D``
     - eixo uniforme
   * - ``ex_Random1D``
     - malha aleatória reprodutível
   * - ``ex_Roberts1D``
     - distribuição de Roberts
   * - ``ex_Custom1D``
     - coordenadas fornecidas pelo usuário
   * - ``ex_SymmetricRandomWeightedCenters1D``
     - centros ponderados em malha aleatória simétrica
   * - ``ex_Operations1D``
     - interseção e recorte de eixos
   * - ``ex_StructuredGrid2D``
     - composição cartesiana e indexação
   * - ``ex_NonUniformStructuredGrid2D``
     - malha estruturada 2D com eixos não uniformes
   * - ``ex_CoordinateSystems2D``
     - sistemas cartesiano, polar e mapeamento customizado
   * - ``ex_FVMakerGeometry2D``
     - distâncias dos eixos e áreas/volumes da malha
   * - ``ex_Operations2D``
     - interseção e recorte de malhas 2D
   * - ``ex_Quality2D``
     - relatório de qualidade de malhas 2D
   * - ``ex_LegacyVTKRectilinearGrid2DWriter``
     - exportação para ParaView em VTK legacy
   * - ``ex_ErrorHandling``
     - tratamento estruturado de erros

Compilar e executar
-------------------

Configure um build dedicado para exemplos:

.. code-block:: bash

   cmake -S . -B build-examples -DBUILD_EXAMPLES=ON -DBUILD_TESTS=OFF

O projeto não permite ativar ``BUILD_TESTS`` e ``BUILD_EXAMPLES`` ao mesmo tempo.
Essa separação mantém os testes e os exemplos em árvores de build distintas.

Para compilar todos os exemplos:

.. code-block:: bash

   cmake --build build-examples -j

Para executar todos os exemplos:

.. code-block:: bash

   cmake --build build-examples --target run_all_examples

Cada arquivo ``examples/.../ex_nome.cc`` ou ``examples/.../ex_nome.cpp`` gera
um alvo executável ``ex_nome`` e um alvo de execução ``run_ex_nome``. Por exemplo:

.. code-block:: bash

   cmake --build build-examples --target ex_Axis1D
   cmake --build build-examples --target run_ex_Axis1D

Os executáveis são gravados em:

.. code-block:: text

   build-examples/bin/examples/

Alvos individuais
-----------------

.. list-table::
   :header-rows: 1

   * - Arquivo
     - Compilar
     - Executar
   * - ``examples/OneDimensional/Axis1D/ex_Axis1D.cc``
     - ``ex_Axis1D``
     - ``run_ex_Axis1D``
   * - ``examples/OneDimensional/Distribution1D/ex_Uniform1D.cc``
     - ``ex_Uniform1D``
     - ``run_ex_Uniform1D``
   * - ``examples/OneDimensional/Distribution1D/ex_Random1D.cc``
     - ``ex_Random1D``
     - ``run_ex_Random1D``
   * - ``examples/OneDimensional/Distribution1D/ex_Roberts1D.cc``
     - ``ex_Roberts1D``
     - ``run_ex_Roberts1D``
   * - ``examples/OneDimensional/Distribution1D/ex_Custom1D.cc``
     - ``ex_Custom1D``
     - ``run_ex_Custom1D``
   * - ``examples/OneDimensional/Exercises/ex_SymmetricRandomWeightedCenters1D.cc``
     - ``ex_SymmetricRandomWeightedCenters1D``
     - ``run_ex_SymmetricRandomWeightedCenters1D``
   * - ``examples/OneDimensional/Operations1D/ex_Operations1D.cc``
     - ``ex_Operations1D``
     - ``run_ex_Operations1D``
   * - ``examples/TwoDimensional/StructuredGrid2D/ex_StructuredGrid2D.cc``
     - ``ex_StructuredGrid2D``
     - ``run_ex_StructuredGrid2D``
   * - ``examples/TwoDimensional/StructuredGrid2D/ex_NonUniformStructuredGrid2D.cc``
     - ``ex_NonUniformStructuredGrid2D``
     - ``run_ex_NonUniformStructuredGrid2D``
   * - ``examples/TwoDimensional/CoordinateSystems2D/ex_CoordinateSystems2D.cc``
     - ``ex_CoordinateSystems2D``
     - ``run_ex_CoordinateSystems2D``
   * - ``examples/TwoDimensional/CoordinateSystems2D/ex_FVMakerGeometry2D.cc``
     - ``ex_FVMakerGeometry2D``
     - ``run_ex_FVMakerGeometry2D``
   * - ``examples/TwoDimensional/Operations2D/ex_Operations2D.cc``
     - ``ex_Operations2D``
     - ``run_ex_Operations2D``
   * - ``examples/TwoDimensional/Quality2D/ex_Quality2D.cc``
     - ``ex_Quality2D``
     - ``run_ex_Quality2D``
   * - ``examples/Output/VTK/ex_LegacyVTKRectilinearGrid2DWriter.cc``
     - ``ex_LegacyVTKRectilinearGrid2DWriter``
     - ``run_ex_LegacyVTKRectilinearGrid2DWriter``
   * - ``examples/ErrorHandling/ex_ErrorHandling.cpp``
     - ``ex_ErrorHandling``
     - ``run_ex_ErrorHandling``

Código-fonte dos exemplos
-------------------------

Os blocos abaixo incluem os arquivos-fonte completos dos exemplos ativos.
O conteúdo vem diretamente da pasta do projeto, de modo que o HTML mostra
o mesmo código compilado pelos alvos CMake.

.. contents::
   :local:
   :depth: 1

ex_Axis1D.cc
------------

.. literalinclude:: ../../examples/OneDimensional/Axis1D/ex_Axis1D.cc
   :language: cpp
   :linenos:

ex_Uniform1D.cc
---------------

.. literalinclude:: ../../examples/OneDimensional/Distribution1D/ex_Uniform1D.cc
   :language: cpp
   :linenos:

ex_Random1D.cc
--------------

.. literalinclude:: ../../examples/OneDimensional/Distribution1D/ex_Random1D.cc
   :language: cpp
   :linenos:

ex_Roberts1D.cc
---------------

.. literalinclude:: ../../examples/OneDimensional/Distribution1D/ex_Roberts1D.cc
   :language: cpp
   :linenos:

ex_Custom1D.cc
--------------

.. literalinclude:: ../../examples/OneDimensional/Distribution1D/ex_Custom1D.cc
   :language: cpp
   :linenos:

ex_SymmetricRandomWeightedCenters1D.cc
--------------------------------------

.. literalinclude:: ../../examples/OneDimensional/Exercises/ex_SymmetricRandomWeightedCenters1D.cc
   :language: cpp
   :linenos:

ex_Operations1D.cc
------------------

.. literalinclude:: ../../examples/OneDimensional/Operations1D/ex_Operations1D.cc
   :language: cpp
   :linenos:

ex_StructuredGrid2D.cc
----------------------

.. literalinclude:: ../../examples/TwoDimensional/StructuredGrid2D/ex_StructuredGrid2D.cc
   :language: cpp
   :linenos:

ex_NonUniformStructuredGrid2D.cc
--------------------------------

.. literalinclude:: ../../examples/TwoDimensional/StructuredGrid2D/ex_NonUniformStructuredGrid2D.cc
   :language: cpp
   :linenos:

ex_CoordinateSystems2D.cc
-------------------------

.. literalinclude:: ../../examples/TwoDimensional/CoordinateSystems2D/ex_CoordinateSystems2D.cc
   :language: cpp
   :linenos:

ex_FVMakerGeometry2D.cc
-----------------------

.. literalinclude:: ../../examples/TwoDimensional/CoordinateSystems2D/ex_FVMakerGeometry2D.cc
   :language: cpp
   :linenos:

ex_Operations2D.cc
------------------

.. literalinclude:: ../../examples/TwoDimensional/Operations2D/ex_Operations2D.cc
   :language: cpp
   :linenos:

ex_Quality2D.cc
---------------

.. literalinclude:: ../../examples/TwoDimensional/Quality2D/ex_Quality2D.cc
   :language: cpp
   :linenos:

ex_LegacyVTKRectilinearGrid2DWriter.cc
--------------------------------------

.. literalinclude:: ../../examples/Output/VTK/ex_LegacyVTKRectilinearGrid2DWriter.cc
   :language: cpp
   :linenos:

ex_ErrorHandling.cpp
--------------------

.. literalinclude:: ../../examples/ErrorHandling/ex_ErrorHandling.cpp
   :language: cpp
   :linenos:

Arquivos antigos em ``examples/Grid`` e ``examples/IO`` são protótipos
históricos com nomes ``Ex_*``. Como começam com ``Ex_`` maiúsculo, não são
capturados pelo padrão CMake ``ex_*`` e não fazem parte dos alvos validados.
