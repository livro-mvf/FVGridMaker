Saída CSV e VTK
===============

``Axis1DCSVWriter`` grava índice, face, centro e distâncias de um eixo. Ele
aceita um ``std::ostream`` ou um caminho e cria os diretórios pais.

``write_vtk`` escolhe automaticamente o dataset legado adequado:

* ``RECTILINEAR_GRID`` para coordenadas cartesianas/retangulares;
* ``STRUCTURED_GRID`` para malhas mapeadas, como polar e esférica.

.. code-block:: cpp

   fvgrid::Axis1DCSVWriter::write(axis, "axis.csv");
   fvgrid::write_vtk(grid, "grid.vtk");

