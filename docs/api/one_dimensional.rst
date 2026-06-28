API unidimensional
==================

A camada 1D e a unidade basica de FVGridMaker. Um eixo armazena faces,
centros, distancias face--face e distancias centro--centro em vetores
contiguos. Geradores e patterns constroem eixos completos; operacoes e metricas
consomem eixos ja validados.

Eixo e geometria
----------------

.. list-table::
   :header-rows: 1

   * - Header
     - Responsabilidade
   * - ``OneDimensional/Axis1D/Axis1D.h``
     - ``BasicAxis1D<T>`` e aliases ``Axis1D``, ``Axis1DFloat`` e ``Axis1DLongDouble``
   * - ``OneDimensional/Axis1D/Axis1D.tpp``
     - implementacao template de validacao, metricas e acesso
   * - ``OneDimensional/Axis1D/Detail/Axis1DRows.h``
     - iteracao tabular compartilhada por impressao e CSV
   * - ``OneDimensional/GridPattern1D/AxisGeometry1D.h``
     - pacote de faces, centros e nome do pattern
   * - ``OneDimensional/GridPattern1D/Coordinates1D.h``
     - coordenadas primarias fornecidas pelo usuario
   * - ``OneDimensional/GridPattern1D/Domain1D.h``
     - dominio fisico usado por patterns baseados em centros

Geradores
---------

.. list-table::
   :header-rows: 1

   * - Header
     - Distribuicao
   * - ``Distribution1D/Uniform1D.h``
     - malha uniforme e ``uniform_axis_1d``
   * - ``Distribution1D/Random1D.h``
     - particionamento pseudoaleatorio reprodutivel e ``random_axis_1d``
   * - ``Distribution1D/Roberts1D.h``
     - concentracao suave de Roberts e ``roberts_axis_1d``
   * - ``Distribution1D/Custom1D.h``
     - coordenadas fornecidas explicitamente pelo usuario

Quando o pattern nao e especificado, os geradores ``Uniform1D``, ``Random1D``
e ``Roberts1D`` usam ``FaceCentered1D``. A construcao direta
``Axis1D{faces}`` continua sendo o atalho explicito para dados de faces e,
por isso, produz um eixo ``VolumeCentered1D``.

Patterns 1D
-----------

.. list-table::
   :header-rows: 1

   * - Header
     - Papel numerico
   * - ``GridPattern1D/VolumeCentered1D.h``
     - reconstrucao de centros a partir de faces
   * - ``GridPattern1D/FaceCentered1D.h``
     - reconstrucao de faces a partir de centros e dominio
   * - ``GridPattern1D/CentersFromFaces1D.h``
     - familia parametrica para centros calculados por peso
   * - ``GridPattern1D/FacesFromCenters1D.h``
     - familia parametrica para faces calculadas por peso
   * - ``GridPattern1D/ConstantWeight1D.h``
     - regra de peso constante usada por patterns customizados
   * - ``GridPattern1D/CoordinateTags1D.h``
     - tags ``FaceCoordinates1D`` e ``CenterCoordinates1D``
   * - ``GridPattern1D/GridPatternConcept1D.h``
     - conceitos ``GridPattern1DFor`` e ``GridPattern1D``

Operacoes e qualidade
---------------------

.. list-table::
   :header-rows: 1

   * - Header
     - Papel
   * - ``Operations1D/AxisInterval1D.h``
     - intervalo vazio, ponto ou segmento
   * - ``Operations1D/Operations1D.h``
     - intersecao, compatibilidade de patterns e recorte
   * - ``Operations1D/Operations1D.tpp``
     - implementacao template das operacoes
   * - ``Quality1D/Quality1D.h``
     - relatorios de qualidade unidimensional
   * - ``Quality1D/Quality1D.tpp``
     - calculos de minimos, maximos e razoes

Ver tambem
----------

* :doc:`../user_guide/axis1d`
* :doc:`../architecture/finite_volume_geometry`
* :doc:`complete`