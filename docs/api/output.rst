API de saida
============

A biblioteca escreve arquivos de texto simples para inspecao, pos-processamento
e visualizacao. Nao ha dependencia na biblioteca VTK: os writers geram o formato
legacy diretamente.

CSV
---

.. list-table::
   :header-rows: 1

   * - Header
     - Conteudo
   * - ``Output/CSV/Axis1DCSVWriter.h``
     - escrita de ``BasicAxis1D<T>`` em ``std::ostream`` ou arquivo
   * - ``Output/CSV/Axis1DCSVWriter.tpp``
     - implementacao template e criacao de diretorios pais

VTK legacy
----------

.. list-table::
   :header-rows: 1

   * - Header
     - Conteudo
   * - ``Output/VTK/LegacyVTKRectilinearGrid2DWriter.h``
     - ``RECTILINEAR_GRID`` e funcao livre ``write_vtk``
   * - ``Output/VTK/LegacyVTKRectilinearGrid2DWriter.tpp``
     - serializacao de coordenadas cartesianas e ``cell_measure``
   * - ``Output/VTK/LegacyVTKStructuredGrid2DWriter.h``
     - ``STRUCTURED_GRID`` para malhas mapeadas
   * - ``Output/VTK/LegacyVTKStructuredGrid2DWriter.tpp``
     - pontos fisicos e medidas de celulas

Ver tambem
----------

* :doc:`../user_guide/output`
* :doc:`complete`