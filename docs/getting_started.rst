Primeiros passos
================

FVGridMaker é uma biblioteca C++20 para construir a geometria de malhas
estruturadas de volumes finitos. Ela não contém solver: o foco é produzir
faces, centros, distâncias e medidas de células de forma validada e
reprodutível.

Requisitos
----------

* compilador com C++20;
* CMake 3.15 ou superior;
* GoogleTest somente para os testes;
* Doxygen, Sphinx e os pacotes de ``docs/requirements.txt`` para o manual.

Compilação
----------

Os modos de exemplos, capítulos, testes e documentação são builds separados.

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

A forma mais curta usa a API iniciante. O tipo escalar é ``double`` e o pattern
padrão do gerador é ``FaceCentered1D``.

.. code-block:: cpp

   #include <FVGridMaker/FVGridMaker.h>

   const auto x = fvgrid::uniform_axis_1d(20, 0.0, 1.0);

   auto faces = x.faces();
   auto centers = x.centers();
   auto delta_faces = x.dx_faces();
   auto delta_centers = x.dx_centers();

A API com tipos fortes continua disponível quando a assinatura precisa deixar
unidades e intenções explícitas:

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

Tipos escalares avançados
-------------------------

Os nomes sem sufixo significam ``double``. Para estudos numéricos específicos,
use aliases ou funções template:

.. code-block:: cpp

   const auto xf = fvgrid::uniform_axis_1d<float>(100, 0.0f, 1.0f);
   const auto xld = fvgrid::uniform_axis_1d<long double>(100, 0.0L, 1.0L);

   static_assert(std::same_as<decltype(xf)::value_type, float>);
