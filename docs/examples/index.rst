Exemplos
========

Os exemplos ativos têm nome ``ex_*`` e são compilados com
``BUILD_EXAMPLES=ON``.

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
   * - ``ex_Operations1D``
     - interseção e recorte de eixos
   * - ``ex_StructuredGrid2D``
     - composição cartesiana e indexação
   * - ``ex_CoordinateSystems2D``
     - polar e sistema inventado
   * - ``ex_FVMakerGeometry2D``
     - distâncias dos eixos e áreas/volumes da malha
   * - ``ex_LegacyVTKRectilinearGrid2DWriter``
     - exportação para ParaView

Compilar e executar
-------------------

.. code-block:: bash

   cmake -S . -B build-examples -DBUILD_EXAMPLES=ON -DBUILD_TESTS=OFF
   cmake --build build-examples -j
   cmake --build build-examples --target run_all_examples

Arquivos antigos em ``examples/Grid`` e ``examples/IO`` são protótipos
históricos com nomes ``Ex_*``; não fazem parte da API validada nem dos alvos.

