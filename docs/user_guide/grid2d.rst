Malha estruturada 2D
====================

``StructuredGrid2D`` compoe dois ``Axis1D`` independentes. As coordenadas
logicas nao sao matrizes ``x[i][j]``: cada direcao conserva seu vetor 1D e as
medidas derivadas ficam em vetores lineares.

Construcao
----------

.. code-block:: cpp

   const auto x = fvgrid::uniform_axis_1d(40, 0.0, 2.0);
   const auto y = fvgrid::uniform_axis_1d(20, -1.0, 1.0);

   const fvgrid::StructuredGrid2D grid{x, y};

Os dois eixos precisam usar patterns compativeis. Eixos gerados sem pattern
explicito usam ``FaceCentered1D`` nos dois sentidos.

Indexacao
---------

As celulas usam ordem row-major:

.. math::

   k = j N_1 + i.

As medidas tambem sao vetores lineares:

.. list-table::
   :header-rows: 1

   * - Dado
     - Tamanho
     - Indice logico
   * - ``cell_measures()``
     - ``N1*N2``
     - ``j*N1+i``
   * - ``first_face_measures()``
     - ``(N1+1)*N2``
     - ``j*(N1+1)+i``
   * - ``second_face_measures()``
     - ``N1*(N2+1)``
     - ``j*N1+i``

Area logica e medida fisica
---------------------------

A malha separa dois conceitos:

* ``cell_logical_area(i, j)`` e a area no espaco das coordenadas independentes;
* ``cell_measure(i, j)`` e a medida fisica definida pelo sistema de coordenadas.

Em cartesiano, os valores coincidem. Em polar, por exemplo, a medida fisica
inclui o fator radial. Essa distincao e importante para evitar que um texto
academico chame volume fisico de area computacional, ou o contrario.

Recorte e intersecao
--------------------

``Operations2D`` calcula caixas logicas de intersecao e recorta malhas. O
recorte aceita eixos ambos ``VolumeCentered1D`` ou ambos ``FaceCentered1D`` e
preserva o pattern quando reconstrui os eixos recortados.

Qualidade
---------

``Quality2D`` reporta minimos, maximos e razoes de medidas de celulas, areas
computacionais e medidas das faces. Use esse relatorio para registrar a
regularidade da malha antes de discutir erro numerico ou convergencia.