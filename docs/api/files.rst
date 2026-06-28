Inventario dos headers publicos
===============================

Esta pagina funciona como mapa de arquivos da API publica. A referencia
completa gerada em :doc:`complete` extrai as declaracoes desses headers; os
arquivos ``.tpp`` aparecem porque contem implementacoes template necessarias ao
uso header-only das classes ``Basic*<T>``.

Core e erros
------------

.. code-block:: text

   FVGridMaker/Core/Types.h
   FVGridMaker/Core/StrongTypes.h
   FVGridMaker/Core/ID.h
   FVGridMaker/Core/Version.h
   FVGridMaker/ErrorHandling/BuiltInErrors.h
   FVGridMaker/ErrorHandling/ErrorContext.h
   FVGridMaker/ErrorHandling/ErrorDescriptor.h
   FVGridMaker/ErrorHandling/ErrorRecord.h
   FVGridMaker/ErrorHandling/ErrorTraits.h
   FVGridMaker/ErrorHandling/FVGridException.h
   FVGridMaker/ErrorHandling/ThrowError.h
   FVGridMaker/FVGridMaker.h

Malhas 1D
---------

.. code-block:: text

   FVGridMaker/OneDimensional/Axis1D/Axis1D.h
   FVGridMaker/OneDimensional/Axis1D/Axis1D.tpp
   FVGridMaker/OneDimensional/Axis1D/Detail/Axis1DRows.h
   FVGridMaker/OneDimensional/Distribution1D/Custom1D.h
   FVGridMaker/OneDimensional/Distribution1D/Random1D.h
   FVGridMaker/OneDimensional/Distribution1D/Random1D.tpp
   FVGridMaker/OneDimensional/Distribution1D/Roberts1D.h
   FVGridMaker/OneDimensional/Distribution1D/Roberts1D.tpp
   FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h
   FVGridMaker/OneDimensional/Distribution1D/Uniform1D.tpp
   FVGridMaker/OneDimensional/GridPattern1D/AxisGeometry1D.h
   FVGridMaker/OneDimensional/GridPattern1D/CentersFromFaces1D.h
   FVGridMaker/OneDimensional/GridPattern1D/ConstantWeight1D.h
   FVGridMaker/OneDimensional/GridPattern1D/CoordinateTags1D.h
   FVGridMaker/OneDimensional/GridPattern1D/Coordinates1D.h
   FVGridMaker/OneDimensional/GridPattern1D/Domain1D.h
   FVGridMaker/OneDimensional/GridPattern1D/FaceCentered1D.h
   FVGridMaker/OneDimensional/GridPattern1D/FacesFromCenters1D.h
   FVGridMaker/OneDimensional/GridPattern1D/GridPatternConcept1D.h
   FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h
   FVGridMaker/OneDimensional/Operations1D/AxisInterval1D.h
   FVGridMaker/OneDimensional/Operations1D/Operations1D.h
   FVGridMaker/OneDimensional/Operations1D/Operations1D.tpp
   FVGridMaker/OneDimensional/Quality1D/Quality1D.h
   FVGridMaker/OneDimensional/Quality1D/Quality1D.tpp

Malhas 2D
---------

.. code-block:: text

   FVGridMaker/TwoDimensional/CoordinateSystem2D/CoordinateSystem2D.h
   FVGridMaker/TwoDimensional/CoordinateSystem2D/CoordinateMetrics2D.h
   FVGridMaker/TwoDimensional/CoordinateSystem2D/CoordinateMappingFactory2D.h
   FVGridMaker/TwoDimensional/Operations2D/LogicalBox2D.h
   FVGridMaker/TwoDimensional/Operations2D/Operations2D.h
   FVGridMaker/TwoDimensional/Operations2D/Operations2D.tpp
   FVGridMaker/TwoDimensional/Quality2D/Quality2D.h
   FVGridMaker/TwoDimensional/Quality2D/Quality2D.tpp
   FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.h
   FVGridMaker/TwoDimensional/StructuredGrid2D/StructuredGrid2D.tpp

Saida
-----

.. code-block:: text

   FVGridMaker/Output/CSV/Axis1DCSVWriter.h
   FVGridMaker/Output/CSV/Axis1DCSVWriter.tpp
   FVGridMaker/Output/VTK/LegacyVTKRectilinearGrid2DWriter.h
   FVGridMaker/Output/VTK/LegacyVTKRectilinearGrid2DWriter.tpp
   FVGridMaker/Output/VTK/LegacyVTKStructuredGrid2DWriter.h
   FVGridMaker/Output/VTK/LegacyVTKStructuredGrid2DWriter.tpp