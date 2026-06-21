Sistemas de coordenadas
=======================

O sistema de coordenadas é um trait verificado pelo ``CoordinateMapping2D``.
Não há enum global nem chamada virtual. O mapeamento é aplicado na construção e
a malha conserva somente dados prontos para consumo.

Traits fornecidos
-----------------

* ``CartesianCoordinates2D``;
* ``PolarCoordinates2D``;
* ``CylindricalAxisymmetricCoordinates2D``;
* ``SphericalAxisymmetricCoordinates2D``.

Sistema personalizado
---------------------

Para uma geometria nova, informe mapeamento, medida da célula e medidas das
duas famílias de faces:

.. code-block:: cpp

   const auto scaled = fvgrid::make_coordinate_mapping_2d(
       "Scaled", "u", "v",
       [](double u, double v) { return fvgrid::PhysicalPoint2D{2*u, 3*v, 0}; },
       [](fvgrid::CoordinateCell2D c) {
           return 6*(c.first_max-c.first_min)*(c.second_max-c.second_min);
       },
       [](double, double a, double b) { return 3*(b-a); },
       [](double, double a, double b) { return 2*(b-a); });

As fórmulas ficam no trait; ``StructuredGrid2D`` permanece fechado para
alterações e aberto a novos sistemas.

