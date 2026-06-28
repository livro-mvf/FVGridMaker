Sistemas de coordenadas
=======================

O sistema de coordenadas e um trait verificado por ``CoordinateMapping2DFor``.
Nao ha enum global nem chamada virtual. O mapeamento e aplicado na construcao e
a malha conserva os dados prontos para consumo.

Traits fornecidos
-----------------

.. list-table::
   :header-rows: 1

   * - Trait
     - Coordenadas logicas
     - Medida fisica
   * - ``CartesianCoordinates2D``
     - ``x, y``
     - area cartesiana
   * - ``PolarCoordinates2D``
     - ``r, theta``
     - area polar plana
   * - ``CylindricalAxisymmetricCoordinates2D``
     - ``r, z``
     - volume axisimetrico cilindrico
   * - ``SphericalAxisymmetricCoordinates2D``
     - ``r, theta``
     - volume axisimetrico esferico

Cada trait fornece nomes das coordenadas, conversao para ponto fisico, medida
da celula e medidas das duas familias de faces.

Sistema personalizado
---------------------

Para uma geometria nova, informe mapeamento, medida da celula e medidas das
duas familias de faces:

.. code-block:: cpp

   const auto scaled = fvgrid::make_coordinate_mapping_2d(
       "Scaled", "u", "v",
       [](double u, double v) {
           return fvgrid::PhysicalPoint2D{2*u, 3*v, 0};
       },
       [](fvgrid::CoordinateCell2D c) {
           return 6*(c.first_max-c.first_min)*(c.second_max-c.second_min);
       },
       [](double, double a, double b) { return 3*(b-a); },
       [](double, double a, double b) { return 2*(b-a); });

As formulas ficam no trait; ``StructuredGrid2D`` permanece fechado para
alteracoes e aberto a novos sistemas. Essa separacao e util em trabalhos
academicos porque a hipotese geometrica fica local e revisavel.

Precisao escalar
----------------

Os sistemas built-in sao templates. A API sem sufixo usa ``double``; a API
avancada permite ``float`` e ``long double`` quando a analise numerica exigir.