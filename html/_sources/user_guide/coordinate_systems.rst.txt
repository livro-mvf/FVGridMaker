Sistemas de coordenadas
=======================

O sistema de coordenadas é um trait verificado por ``CoordinateMapping2DFor``.
Não há enum global nem chamada virtual. O mapeamento é aplicado na construção e
a malha conserva os dados prontos para consumo.

Traits fornecidos
-----------------

.. list-table::
   :header-rows: 1

   * - Trait
     - Coordenadas lógicas
     - Medida física
   * - ``CartesianCoordinates2D``
     - ``x, y``
     - área cartesiana
   * - ``PolarCoordinates2D``
     - ``r, theta``
     - área polar plana
   * - ``CylindricalAxisymmetricCoordinates2D``
     - ``r, z``
     - volume axisimétrico cilíndrico
   * - ``SphericalAxisymmetricCoordinates2D``
     - ``r, theta``
     - volume axisimétrico esférico

Cada trait fornece nomes das coordenadas, conversão para ponto físico, medida
da célula e medidas das duas famílias de faces.

Sistema personalizado
---------------------

Para uma geometria nova, informe mapeamento, medida da célula e medidas das
duas famílias de faces:

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

As fórmulas ficam no trait; ``StructuredGrid2D`` permanece fechado para
alterações e aberto a novos sistemas. Essa separação é útil em trabalhos
acadêmicos porque a hipótese geométrica fica local e revisável.

Precisão escalar
----------------

Os sistemas built-in são templates. A API sem sufixo usa ``double``; a API
avançada permite ``float`` e ``long double`` quando a análise numérica exigir.
