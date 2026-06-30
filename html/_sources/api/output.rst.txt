API de saída
============

A biblioteca escreve arquivos de texto simples para inspeção, pós-processamento
e visualização. Não há dependência na biblioteca VTK: os writers geram o
formato legacy diretamente.

CSV
---

.. list-table::
   :header-rows: 1

   * - Arquivo
     - Conteúdo
   * - ``Output/CSV/Axis1DCSVWriter.h``
     - escrita de ``BasicAxis1D<T>`` em ``std::ostream`` ou arquivo
   * - ``Output/CSV/Axis1DCSVWriter.tpp``
     - implementação template e criação de diretórios pais

VTK legacy
----------

.. list-table::
   :header-rows: 1

   * - Arquivo
     - Conteúdo
   * - ``Output/VTK/LegacyVTKRectilinearGrid2DWriter.h``
     - ``RECTILINEAR_GRID`` e função livre ``write_vtk``
   * - ``Output/VTK/LegacyVTKRectilinearGrid2DWriter.tpp``
     - serialização de coordenadas cartesianas e ``cell_measure``
   * - ``Output/VTK/LegacyVTKStructuredGrid2DWriter.h``
     - ``STRUCTURED_GRID`` para malhas mapeadas
   * - ``Output/VTK/LegacyVTKStructuredGrid2DWriter.tpp``
     - pontos físicos e medidas de células

Ver também
----------

* :doc:`../user_guide/output`
* :doc:`complete`
