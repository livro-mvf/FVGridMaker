Eixos 1D
========

``Axis1D`` é o proprietário dos dados geométricos de uma direção. Faces,
centros, distâncias face--face e distâncias centro--centro são vetores
contíguos expostos como ``std::span<const Real>``.

Convenções
----------

Para ``N`` volumes existem ``N+1`` faces, ``N`` centros, ``N`` valores em
``dx_faces()`` e ``N+1`` valores em ``dx_centers()``. Os elementos extremos de
``dx_centers()`` são as distâncias entre o contorno e o primeiro/último centro.

Geradores
---------

* ``Uniform1D``: espaçamento uniforme;
* ``Random1D``: partição pseudoaleatória reprodutível;
* ``Roberts1D``: concentração suave junto aos extremos;
* ``Custom1D``: coordenadas fornecidas pelo usuário.

Todos produzem um ``Axis1D`` completo; a FVMaker não precisa reconstruir
distâncias.

