Estratégia de testes
====================

A suíte usa um executável por arquivo ``tst_*``. Os testes cobrem valores
nominais, invariantes, índices, degenerações admissíveis e falhas de I/O.

.. list-table:: Matriz de prevenção
   :header-rows: 1

   * - Componente
     - Erros prevenidos
   * - ``Axis1D``
     - contagens incorretas, NaN/infinito, ordem, centros fora das faces
   * - geradores 1D
     - volumes zero, comprimentos inválidos, parâmetros impossíveis
   * - patterns/custom
     - tipo de coordenada incompatível e domínio ausente
   * - ``Operations1D``
     - tolerância inválida, interseção vazia e recorte degenerado
   * - ``StructuredGrid2D``
     - índices, ordem row-major e medidas não físicas
   * - sistemas de coordenadas
     - fórmulas cartesianas, polares, cilíndricas, esféricas e customizadas
   * - CSV/VTK
     - conteúdo, ordem, criação de diretório e falha de escrita

Comandos
--------

.. code-block:: bash

   ctest --test-dir build-tests --output-on-failure
   cmake --build build-tests --target run_all_tests
