API unidimensional
==================

A camada 1D é a unidade básica da FVGridMaker. Um eixo armazena faces,
centros, distâncias face--face e distâncias centro--centro em vetores
contíguos. Geradores e patterns constroem eixos completos; operações e métricas
consomem eixos já validados.

Eixo e geometria
----------------

.. list-table::
   :header-rows: 1

   * - Arquivo
     - Responsabilidade
   * - ``OneDimensional/Axis1D/Axis1D.h``
     - ``BasicAxis1D<T>`` e aliases ``Axis1D``, ``Axis1DFloat`` e ``Axis1DLongDouble``
   * - ``OneDimensional/Axis1D/Axis1D.tpp``
     - implementação template de validação, métricas e acesso
   * - ``OneDimensional/Axis1D/Detail/Axis1DRows.h``
     - iteração tabular compartilhada por impressão e CSV
   * - ``OneDimensional/GridPattern1D/AxisGeometry1D.h``
     - pacote de faces, centros e nome do pattern
   * - ``OneDimensional/GridPattern1D/Coordinates1D.h``
     - coordenadas primárias fornecidas pelo usuário
   * - ``OneDimensional/GridPattern1D/Domain1D.h``
     - domínio físico usado por patterns baseados em centros

Geradores
---------

.. list-table::
   :header-rows: 1

   * - Arquivo
     - Distribuição
   * - ``Distribution1D/Uniform1D.h``
     - malha uniforme e ``uniform_axis_1d``
   * - ``Distribution1D/Random1D.h``
     - particionamento pseudoaleatório reprodutível e ``random_axis_1d``
   * - ``Distribution1D/Roberts1D.h``
     - concentração suave de Roberts e ``roberts_axis_1d``
   * - ``Distribution1D/Custom1D.h``
     - coordenadas fornecidas explicitamente pelo usuário

Quando o pattern não é especificado, os geradores ``Uniform1D``, ``Random1D``
e ``Roberts1D`` usam ``FaceCentered1D``. A construção direta
``Axis1D{faces}`` continua sendo o atalho explícito para dados de faces e,
por isso, produz um eixo ``VolumeCentered1D``.

Patterns 1D
-----------

.. list-table::
   :header-rows: 1

   * - Arquivo
     - Papel numérico
   * - ``GridPattern1D/VolumeCentered1D.h``
     - reconstrução de centros a partir de faces
   * - ``GridPattern1D/FaceCentered1D.h``
     - reconstrução de faces a partir de centros e domínio
   * - ``GridPattern1D/CentersFromFaces1D.h``
     - família paramétrica para centros calculados por peso
   * - ``GridPattern1D/FacesFromCenters1D.h``
     - família paramétrica para faces calculadas por peso
   * - ``GridPattern1D/ConstantWeight1D.h``
     - regra de peso constante usada por patterns customizados
   * - ``GridPattern1D/CoordinateTags1D.h``
     - tags ``FaceCoordinates1D`` e ``CenterCoordinates1D``
   * - ``GridPattern1D/GridPatternConcept1D.h``
     - conceitos ``GridPattern1DFor`` e ``GridPattern1D``

Operações e qualidade
---------------------

.. list-table::
   :header-rows: 1

   * - Arquivo
     - Papel
   * - ``Operations1D/AxisInterval1D.h``
     - intervalo vazio, ponto ou segmento
   * - ``Operations1D/Operations1D.h``
     - interseção, compatibilidade de patterns e recorte
   * - ``Operations1D/Operations1D.tpp``
     - implementação template das operações
   * - ``Quality1D/Quality1D.h``
     - relatórios de qualidade unidimensional
   * - ``Quality1D/Quality1D.tpp``
     - cálculos de mínimos, máximos e razões

Ver também
----------

* :doc:`../user_guide/axis1d`
* :doc:`../architecture/finite_volume_geometry`
* :doc:`complete`
