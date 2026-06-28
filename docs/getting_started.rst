Primeiros passos
================

FVGridMaker e uma biblioteca C++20 para construir a geometria de malhas
estruturadas de volumes finitos. Ela nao contem solver: o foco e produzir
faces, centros, distancias e medidas de celulas de forma validada e
reprodutivel.

Requisitos
----------

* compilador com C++20;
* CMake 3.15 ou superior;
* GoogleTest somente para os testes;
* Doxygen, Sphinx e os pacotes de ``docs/requirements.txt`` para o manual.

Compilacao
----------

Os modos de exemplos, capitulos, testes e documentacao sao builds separados.

.. code-block:: bash

   cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
   cmake --build build -j

   cmake -S . -B build-tests -DBUILD_TESTS=ON -DBUILD_EXAMPLES=OFF
   cmake --build build-tests -j
   ctest --test-dir build-tests --output-on-failure

   cmake -S . -B build_docs -DBUILD_DOCUMENTATION=ON \
         -DBUILD_EXAMPLES=OFF -DBUILD_TESTS=OFF
   cmake --build build_docs --target docs -j

Primeiro eixo
-------------

A forma mais curta usa a API iniciante. O tipo escalar e ``double`` e o pattern
padrao do gerador e ``FaceCentered1D``.

.. code-block:: cpp

   #include <FVGridMaker/FVGridMaker.h>

   const auto x = fvgrid::uniform_axis_1d(20, 0.0, 1.0);

   auto faces = x.faces();
   auto centers = x.centers();
   auto delta_faces = x.dx_faces();
   auto delta_centers = x.dx_centers();

A API com tipos fortes continua disponivel quando a assinatura precisa deixar
unidades e intencoes explicitas:

.. code-block:: cpp

   const auto x = fvgrid::Uniform1D::make(
       fvgrid::NVol{20},
       fvgrid::Length{1.0},
       fvgrid::XInit{0.0});

Malha 2D
--------

.. code-block:: cpp

   const auto x = fvgrid::uniform_axis_1d(20, 0.0, 1.0);
   const auto y = fvgrid::uniform_axis_1d(10, 0.0, 0.5);

   const fvgrid::StructuredGrid2D grid{x, y};

   const double logical_area = grid.cell_logical_area(0, 0);
   const double physical_measure = grid.cell_measure(0, 0);

   fvgrid::write_vtk(grid, "malha.vtk");

Tipos escalares avancados
-------------------------

Os nomes sem sufixo significam ``double``. Para estudos numericos especificos,
use aliases ou funcoes template:

.. code-block:: cpp

   const auto xf = fvgrid::uniform_axis_1d<float>(100, 0.0f, 1.0f);
   const auto xld = fvgrid::uniform_axis_1d<long double>(100, 0.0L, 1.0L);

   static_assert(std::same_as<decltype(xf)::value_type, float>);