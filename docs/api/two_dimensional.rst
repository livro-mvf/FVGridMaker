API bidimensional
=================

A malha 2D e uma composicao tensorial de dois eixos 1D independentes. A
biblioteca separa coordenadas logicas de medidas fisicas: ``cell_logical_area``
e area computacional; ``cell_measure`` e medida fisica dada pelo sistema de
coordenadas.

Malha estruturada
-----------------

.. list-table::
   :header-rows: 1

   * - Header
     - Responsabilidade
   * - ``TwoDimensional/StructuredGrid2D/StructuredGrid2D.h``
     - ``BasicStructuredGrid2D<T>`` e aliases publicos
   * - ``TwoDimensional/StructuredGrid2D/StructuredGrid2D.tpp``
     - construcao, indexacao, pontos fisicos e medidas derivadas

Sistemas de coordenadas
-----------------------

.. list-table::
   :header-rows: 1

   * - Header
     - Conteudo
   * - ``CoordinateSystem2D/CoordinateSystem2D.h``
     - pontos fisicos, celulas logicas, conceitos e sistemas cartesianos/polares
   * - ``CoordinateSystem2D/CoordinateMetrics2D.h``
     - vetores, metricas auxiliares e sistema esferico axisimetrico
   * - ``CoordinateSystem2D/CoordinateMappingFactory2D.h``
     - fabricas para mapeamentos funcionais customizados

Operacoes e qualidade
---------------------

.. list-table::
   :header-rows: 1

   * - Header
     - Papel
   * - ``Operations2D/LogicalBox2D.h``
     - caixa logica formada por dois intervalos 1D
   * - ``Operations2D/Operations2D.h``
     - intersecao, recorte e compatibilidade de patterns
   * - ``Operations2D/Operations2D.tpp``
     - implementacao template das operacoes 2D
   * - ``Quality2D/Quality2D.h``
     - relatorio de medidas e razoes de malha
   * - ``Quality2D/Quality2D.tpp``
     - calculos de qualidade em vetores contiguos

Ver tambem
----------

* :doc:`../user_guide/grid2d`
* :doc:`../user_guide/coordinate_systems`
* :doc:`../architecture/data_layout`
* :doc:`complete`