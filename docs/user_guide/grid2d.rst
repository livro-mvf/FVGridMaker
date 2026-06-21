Malha estruturada 2D
====================

``StructuredGrid2D`` compõe dois ``Axis1D`` independentes. As coordenadas
lógicas não são matrizes ``x[i][j]``: cada direção conserva seu vetor 1D.

Indexação
---------

As células usam ordem row-major:

.. math::

   k = j N_1 + i.

As medidas também são vetores lineares:

* ``cell_measures()``: ``N1*N2`` valores;
* ``first_face_measures()``: ``(N1+1)*N2`` valores;
* ``second_face_measures()``: ``N1*(N2+1)`` valores.

Essa disposição evita vetores de vetores, reduz alocações e permite que a
FVMaker percorra blocos contíguos.

