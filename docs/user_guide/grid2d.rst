Malha estruturada 2D
====================

``StructuredGrid2D`` compõe dois ``Axis1D`` independentes. As coordenadas
lógicas não são matrizes ``x[i][j]``: cada direção conserva seu vetor 1D e as
medidas derivadas ficam em vetores lineares.

Construção
----------

.. code-block:: cpp

   const auto x = fvgrid::uniform_axis_1d(40, 0.0, 2.0);
   const auto y = fvgrid::uniform_axis_1d(20, -1.0, 1.0);

   const fvgrid::StructuredGrid2D grid{x, y};

Os dois eixos precisam usar patterns compatíveis. Eixos gerados sem pattern
explícito usam ``FaceCentered1D`` nos dois sentidos.

Indexação
---------

As células usam ordem row-major:

.. math::

   k = j N_1 + i.

As medidas também são vetores lineares:

.. list-table::
   :header-rows: 1

   * - Dado
     - Tamanho
     - Índice lógico
   * - ``cell_measures()``
     - ``N1*N2``
     - ``j*N1+i``
   * - ``first_face_measures()``
     - ``(N1+1)*N2``
     - ``j*(N1+1)+i``
   * - ``second_face_measures()``
     - ``N1*(N2+1)``
     - ``j*N1+i``

Área lógica e medida física
---------------------------

A malha separa dois conceitos:

* ``cell_logical_area(i, j)`` é a área no espaço das coordenadas independentes;
* ``cell_measure(i, j)`` é a medida física definida pelo sistema de coordenadas.

Em cartesiano, os valores coincidem. Em polar, por exemplo, a medida física
inclui o fator radial. Essa distinção é importante para evitar que um texto
acadêmico chame volume físico de área computacional, ou o contrário.

Recorte e interseção
--------------------

``Operations2D`` calcula caixas lógicas de interseção e recorta malhas. O
recorte aceita eixos ambos ``VolumeCentered1D`` ou ambos ``FaceCentered1D`` e
preserva o pattern quando reconstrói os eixos recortados.

Qualidade
---------

``Quality2D`` reporta mínimos, máximos e razões de medidas de células, áreas
computacionais e medidas das faces. Use esse relatório para registrar a
regularidade da malha antes de discutir erro numérico ou convergência.
