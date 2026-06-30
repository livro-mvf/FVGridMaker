Saída CSV e VTK
===============

FVGridMaker escreve arquivos de texto simples para inspeção, pós-processamento
e visualização. A biblioteca não depende da biblioteca VTK; o formato legacy é
gerado diretamente.

CSV de eixo 1D
--------------

``Axis1DCSVWriter`` grava índice, face, centro, ``dx_faces`` e ``dx_centers``.
Ele aceita um ``std::ostream`` ou um caminho e cria diretórios pais.

.. code-block:: cpp

   fvgrid::Axis1DCSVWriter::write(axis, "axis.csv");

Esse arquivo é apropriado para conferir tabelas de malha em planilhas ou anexos
de relatórios.

VTK legacy
----------

``write_vtk`` escolhe automaticamente o dataset legado adequado:

* ``RECTILINEAR_GRID`` para malhas cartesianas retangulares;
* ``STRUCTURED_GRID`` para malhas mapeadas, como polar ou esférica.

.. code-block:: cpp

   fvgrid::write_vtk(grid, "grid.vtk");

O campo escalar escrito para a medida física chama-se ``cell_measure``. A área
lógica permanece disponível na API C++, mas não deve ser confundida com a
medida física exportada.

Tipos escalares no VTK
----------------------

Malhas ``float`` são exportadas como ``float``. Malhas ``double`` e
``long double`` são exportadas como ``double`` no arquivo legacy, seguindo as
limitações práticas do formato e dos leitores comuns.
