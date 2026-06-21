Layout e indexação
==================

Para ``N1`` por ``N2`` células:

.. list-table::
   :header-rows: 1

   * - Dado
     - Tamanho
     - Índice
   * - volumes
     - ``N1*N2``
     - ``j*N1+i``
   * - faces da primeira direção
     - ``(N1+1)*N2``
     - ``j*(N1+1)+i``
   * - faces da segunda direção
     - ``N1*(N2+1)``
     - ``j*N1+i``

Faces, centros e distâncias de cada direção são mantidos apenas no respectivo
``Axis1D``. A malha 2D não duplica esses vetores.

