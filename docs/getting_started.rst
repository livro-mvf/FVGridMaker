Primeiros passos
================

Requisitos
----------

* compilador com C++20;
* CMake 3.15 ou superior;
* GoogleTest somente para os testes;
* Doxygen, Sphinx e os pacotes de ``docs/requirements.txt`` para o manual.

Compilação
----------

Os modos de exemplos, capítulos e testes são builds separados.

.. code-block:: bash

   cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
   cmake --build build -j

   cmake -S . -B build-tests -DBUILD_TESTS=ON -DBUILD_EXAMPLES=OFF
   cmake --build build-tests -j
   ctest --test-dir build-tests --output-on-failure

Primeiro eixo
-------------

.. code-block:: cpp

   #include <FVGridMaker/FVGridMaker.h>

   const auto x = fvgrid::Uniform1D::make(
       fvgrid::NVol{20}, fvgrid::Length{1.0}, fvgrid::XInit{0.0});

   auto faces = x.faces();
   auto centers = x.centers();
   auto delta_faces = x.dx_faces();
   auto delta_centers = x.dx_centers();

Malha 2D
--------

.. code-block:: cpp

   const auto y = fvgrid::Uniform1D::make(
       fvgrid::NVol{10}, fvgrid::Length{0.5}, fvgrid::XInit{0.0});
   const fvgrid::StructuredGrid2D grid{x, y};

   std::cout << grid;
   fvgrid::write_vtk(grid, "malha.vtk");

