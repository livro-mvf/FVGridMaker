Programas do manual
===================

Esta página reúne os programas completos usados no manual da FVGridMaker.
Os arquivos ficam nas pastas ``manual/Grid1D/`` e ``manual/Grid2D/`` e são
compilados pelo CMake quando ``BUILD_EXAMPLES=ON`` está ativo.

Ao contrário de exemplos mínimos, estes programas são deliberadamente
didáticos: trazem comentários extensos, verificações simples de consistência
e mensagens de saída que explicam cada etapa executada.

Compilação e execução
---------------------

Os programas do manual usam o mesmo modo de build dos exemplos da biblioteca.
Por isso, configure o projeto com ``BUILD_EXAMPLES=ON`` e ``BUILD_TESTS=OFF``:

.. code-block:: bash

   cmake -S . -B build-manual -DBUILD_EXAMPLES=ON -DBUILD_TESTS=OFF

O projeto não permite ativar ``BUILD_TESTS`` e ``BUILD_EXAMPLES`` ao mesmo tempo.
Essa separação evita misturar executáveis de teste, exemplos curtos e programas
didáticos do manual no mesmo build.

Para compilar todos os programas do manual:

.. code-block:: bash

   cmake --build build-manual -j

Para executar todos os programas do manual:

.. code-block:: bash

   cmake --build build-manual --target run_all_manual_programs

Cada arquivo ``manual/Grid1D/man_nome.cc`` ou ``manual/Grid2D/man_nome.cc``
gera um alvo executável ``man_nome`` e um alvo de execução ``run_man_nome``.
Por exemplo:

.. code-block:: bash

   cmake --build build-manual --target man_uniformGrid
   cmake --build build-manual --target run_man_uniformGrid

Os executáveis são gravados em:

.. code-block:: text

   build-manual/bin/manual/

Alvos individuais
-----------------

.. list-table::
   :header-rows: 1

   * - Arquivo
     - Compilar
     - Executar
   * - ``Grid1D/man_alternatingWeightedCenters1D.cc``
     - ``man_alternatingWeightedCenters1D``
     - ``run_man_alternatingWeightedCenters1D``
   * - ``Grid1D/man_axis1DCSV.cc``
     - ``man_axis1DCSV``
     - ``run_man_axis1DCSV``
   * - ``Grid2D/man_axisymmetricGrid2D.cc``
     - ``man_axisymmetricGrid2D``
     - ``run_man_axisymmetricGrid2D``
   * - ``Grid2D/man_cartesianArea2D.cc``
     - ``man_cartesianArea2D``
     - ``run_man_cartesianArea2D``
   * - ``Grid2D/man_clipGrid2D.cc``
     - ``man_clipGrid2D``
     - ``run_man_clipGrid2D``
   * - ``Grid1D/man_customCoordinates1D.cc``
     - ``man_customCoordinates1D``
     - ``run_man_customCoordinates1D``
   * - ``Grid2D/man_ellipticGrid2D.cc``
     - ``man_ellipticGrid2D``
     - ``run_man_ellipticGrid2D``
   * - ``Grid2D/man_incompleteAnnulus2D.cc``
     - ``man_incompleteAnnulus2D``
     - ``run_man_incompleteAnnulus2D``
   * - ``Grid2D/man_linearIndex2D.cc``
     - ``man_linearIndex2D``
     - ``run_man_linearIndex2D``
   * - ``Grid2D/man_nonUniformGrid2D.cc``
     - ``man_nonUniformGrid2D``
     - ``run_man_nonUniformGrid2D``
   * - ``Grid2D/man_polarGrid2D.cc``
     - ``man_polarGrid2D``
     - ``run_man_polarGrid2D``
   * - ``Grid1D/man_powerLawGrid.cc``
     - ``man_powerLawGrid``
     - ``run_man_powerLawGrid``
   * - ``Grid1D/man_quality1D.cc``
     - ``man_quality1D``
     - ``run_man_quality1D``
   * - ``Grid2D/man_qualityAndVTK2D.cc``
     - ``man_qualityAndVTK2D``
     - ``run_man_qualityAndVTK2D``
   * - ``Grid1D/man_randomGrid.cc``
     - ``man_randomGrid``
     - ``run_man_randomGrid``
   * - ``Grid1D/man_robertsGrid.cc``
     - ``man_robertsGrid``
     - ``run_man_robertsGrid``
   * - ``Grid2D/man_scalarField2D.cc``
     - ``man_scalarField2D``
     - ``run_man_scalarField2D``
   * - ``Grid2D/man_structuredGrid2D.cc``
     - ``man_structuredGrid2D``
     - ``run_man_structuredGrid2D``
   * - ``Grid1D/man_uniformGrid.cc``
     - ``man_uniformGrid``
     - ``run_man_uniformGrid``

Lista de programas
------------------

.. contents::
   :local:
   :depth: 1

man_alternatingWeightedCenters1D.cc
-----------------------------------

.. literalinclude:: ../../manual/Grid1D/man_alternatingWeightedCenters1D.cc
   :language: cpp
   :linenos:

man_axis1DCSV.cc
----------------

.. literalinclude:: ../../manual/Grid1D/man_axis1DCSV.cc
   :language: cpp
   :linenos:

man_axisymmetricGrid2D.cc
-------------------------

.. literalinclude:: ../../manual/Grid2D/man_axisymmetricGrid2D.cc
   :language: cpp
   :linenos:

man_cartesianArea2D.cc
----------------------

.. literalinclude:: ../../manual/Grid2D/man_cartesianArea2D.cc
   :language: cpp
   :linenos:

man_clipGrid2D.cc
-----------------

.. literalinclude:: ../../manual/Grid2D/man_clipGrid2D.cc
   :language: cpp
   :linenos:

man_customCoordinates1D.cc
--------------------------

.. literalinclude:: ../../manual/Grid1D/man_customCoordinates1D.cc
   :language: cpp
   :linenos:

man_ellipticGrid2D.cc
---------------------

.. literalinclude:: ../../manual/Grid2D/man_ellipticGrid2D.cc
   :language: cpp
   :linenos:

man_incompleteAnnulus2D.cc
--------------------------

.. literalinclude:: ../../manual/Grid2D/man_incompleteAnnulus2D.cc
   :language: cpp
   :linenos:

man_linearIndex2D.cc
--------------------

.. literalinclude:: ../../manual/Grid2D/man_linearIndex2D.cc
   :language: cpp
   :linenos:

man_nonUniformGrid2D.cc
-----------------------

.. literalinclude:: ../../manual/Grid2D/man_nonUniformGrid2D.cc
   :language: cpp
   :linenos:

man_polarGrid2D.cc
------------------

.. literalinclude:: ../../manual/Grid2D/man_polarGrid2D.cc
   :language: cpp
   :linenos:

man_powerLawGrid.cc
-------------------

.. literalinclude:: ../../manual/Grid1D/man_powerLawGrid.cc
   :language: cpp
   :linenos:

man_quality1D.cc
----------------

.. literalinclude:: ../../manual/Grid1D/man_quality1D.cc
   :language: cpp
   :linenos:

man_qualityAndVTK2D.cc
----------------------

.. literalinclude:: ../../manual/Grid2D/man_qualityAndVTK2D.cc
   :language: cpp
   :linenos:

man_randomGrid.cc
-----------------

.. literalinclude:: ../../manual/Grid1D/man_randomGrid.cc
   :language: cpp
   :linenos:

man_robertsGrid.cc
------------------

.. literalinclude:: ../../manual/Grid1D/man_robertsGrid.cc
   :language: cpp
   :linenos:

man_scalarField2D.cc
--------------------

.. literalinclude:: ../../manual/Grid2D/man_scalarField2D.cc
   :language: cpp
   :linenos:

man_structuredGrid2D.cc
-----------------------

.. literalinclude:: ../../manual/Grid2D/man_structuredGrid2D.cc
   :language: cpp
   :linenos:

man_uniformGrid.cc
------------------

.. literalinclude:: ../../manual/Grid1D/man_uniformGrid.cc
   :language: cpp
   :linenos:
