Saida CSV e VTK
===============

FVGridMaker escreve arquivos de texto simples para inspecao, pos-processamento
e visualizacao. A biblioteca nao depende da biblioteca VTK; o formato legacy e
gerado diretamente.

CSV de eixo 1D
--------------

``Axis1DCSVWriter`` grava indice, face, centro, ``dx_faces`` e ``dx_centers``.
Ele aceita um ``std::ostream`` ou um caminho e cria diretorios pais.

.. code-block:: cpp

   fvgrid::Axis1DCSVWriter::write(axis, "axis.csv");

Esse arquivo e apropriado para conferir tabelas de malha em planilhas ou anexos
de relatorios.

VTK legacy
----------

``write_vtk`` escolhe automaticamente o dataset legado adequado:

* ``RECTILINEAR_GRID`` para malhas cartesianas retangulares;
* ``STRUCTURED_GRID`` para malhas mapeadas, como polar ou esferica.

.. code-block:: cpp

   fvgrid::write_vtk(grid, "grid.vtk");

O campo escalar escrito para a medida fisica chama-se ``cell_measure``. A area
logica permanece disponivel na API C++, mas nao deve ser confundida com a
medida fisica exportada.

Tipos escalares no VTK
----------------------

Malhas ``float`` sao exportadas como ``float``. Malhas ``double`` e
``long double`` sao exportadas como ``double`` no arquivo legacy, seguindo as
limitacoes praticas do formato e dos leitores comuns.