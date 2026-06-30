API bidimensional
=================

A malha 2D é uma composição tensorial de dois eixos 1D independentes. A
biblioteca separa coordenadas lógicas de medidas físicas: ``cell_logical_area``
é área computacional; ``cell_measure`` é medida física dada pelo sistema de
coordenadas.

Malha estruturada
-----------------

.. list-table::
   :header-rows: 1

   * - Arquivo
     - Responsabilidade
   * - ``TwoDimensional/StructuredGrid2D/StructuredGrid2D.h``
     - ``BasicStructuredGrid2D<T>`` e aliases públicos
   * - ``TwoDimensional/StructuredGrid2D/StructuredGrid2D.tpp``
     - construção, indexação, pontos físicos e medidas derivadas

Sistemas de coordenadas
-----------------------

.. list-table::
   :header-rows: 1

   * - Arquivo
     - Conteúdo
   * - ``CoordinateSystem2D/CoordinateSystem2D.h``
     - pontos físicos, células lógicas, conceitos e sistemas cartesianos/polares
   * - ``CoordinateSystem2D/CoordinateMetrics2D.h``
     - vetores, métricas auxiliares e sistema esférico axisimétrico
   * - ``CoordinateSystem2D/CoordinateMappingFactory2D.h``
     - fábricas para mapeamentos funcionais customizados

Operações e qualidade
---------------------

.. list-table::
   :header-rows: 1

   * - Arquivo
     - Papel
   * - ``Operations2D/LogicalBox2D.h``
     - caixa lógica formada por dois intervalos 1D
   * - ``Operations2D/Operations2D.h``
     - interseção, recorte e compatibilidade de patterns
   * - ``Operations2D/Operations2D.tpp``
     - implementação template das operações 2D
   * - ``Quality2D/Quality2D.h``
     - relatório de medidas e razões de malha
   * - ``Quality2D/Quality2D.tpp``
     - cálculos de qualidade em vetores contíguos

Ver também
----------

* :doc:`../user_guide/grid2d`
* :doc:`../user_guide/coordinate_systems`
* :doc:`../architecture/data_layout`
* :doc:`complete`
