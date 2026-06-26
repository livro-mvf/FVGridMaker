# FVGridMaker — Project Tree

This document records the active project tree and the architectural status of the current implementation.

## 1. Tree principles

The project tree follows these decisions:

```text
1. The project works with structured grids, not unstructured meshes.
2. The fundamental one-dimensional unit is Axis1D.
3. Axis1D stores completed geometry: faces, centers, dx_faces and dx_centers.
4. A structured 2D grid is composed of two independent Axis1D objects.
5. Distribution1D generates or organizes primary coordinates.
6. GridPattern1D reconstructs secondary coordinates.
7. Each GridPattern1D declares exactly one accepted primary-coordinate kind.
8. Axis1D computes general geometric metrics after faces and centers exist.
9. Operations1D operates on already-built axes.
10. CoordinateSystem2D interprets logical axes and computes physical measures.
11. Output is peripheral.
12. YAML is allowed only in examples or external applications.
13. GoogleTest appears only in tests.
14. No peripheral module should create a reverse dependency into the core.
15. No extensible category should be implemented with an enum.
```

Preferred vocabulary:

```text
Grid
Axis
Axis1D
StructuredGrid
Distribution
GridPattern
CoordinateSystem
Operations
Output
```

## 2. Active tree

```text
FVGridMaker/
├── CMakeLists.txt
├── README.md
├── LICENSE.md
├── FVGridMaker_cronograma_por_gates.md
├── FVGridMaker_requisitos.md
├── FVGridMaker_tree.md
├── .gitignore
├── .clang-format
│
├── FVGridMakerLib/
│   ├── include/
│   │   └── FVGridMaker/
│   │       ├── FVGridMaker.h
│   │       │
│   │       ├── Core/
│   │       │   ├── ID.h
│   │       │   ├── StrongTypes.h
│   │       │   ├── Types.h
│   │       │   └── Version.h
│   │       │
│   │       ├── ErrorHandling/
│   │       │   ├── BuiltInErrors.h
│   │       │   ├── ErrorContext.h
│   │       │   ├── ErrorDescriptor.h
│   │       │   ├── ErrorRecord.h
│   │       │   ├── ErrorTraits.h
│   │       │   ├── FVGridException.h
│   │       │   └── ThrowError.h
│   │       │
│   │       ├── OneDimensional/
│   │       │   ├── Axis1D/
│   │       │   │   ├── Axis1D.h
│   │       │   │   └── Detail/
│   │       │   │       └── Axis1DRows.h
│   │       │   │
│   │       │   ├── Distribution1D/
│   │       │   │   ├── Custom1D.h
│   │       │   │   ├── Random1D.h
│   │       │   │   ├── Roberts1D.h
│   │       │   │   └── Uniform1D.h
│   │       │   │
│   │       │   ├── GridPattern1D/
│   │       │   │   ├── AxisGeometry1D.h
│   │       │   │   ├── CoordinateKind1D.h
│   │       │   │   ├── Coordinates1D.h
│   │       │   │   ├── Domain1D.h
│   │       │   │   ├── FaceCentered1D.h
│   │       │   │   └── VolumeCentered1D.h
│   │       │   │
│   │       │   └── Operations1D/
│   │       │       ├── AxisInterval1D.h
│   │       │       └── Operations1D.h
│   │       │
│   │       ├── TwoDimensional/
│   │       │   ├── CoordinateSystem2D/
│   │       │   │   ├── CoordinateMappingFactory2D.h
│   │       │   │   ├── CoordinateMetrics2D.h
│   │       │   │   └── CoordinateSystem2D.h
│   │       │   │
│   │       │   └── StructuredGrid2D/
│   │       │       └── StructuredGrid2D.h
│   │       │
│   │       └── Output/
│   │           ├── CSV/
│   │           │   └── Axis1DCSVWriter.h
│   │           │
│   │           └── VTK/
│   │               └── LegacyVTKRectilinearGrid2DWriter.h
│   │
│   └── src/
│       └── FVGridMaker/
│           ├── FVGridMaker.cc
│           │
│           ├── Core/
│           │   ├── ID.cc
│           │   └── Version.cc
│           │
│           ├── ErrorHandling/
│           │   └── FVGridException.cc
│           │
│           ├── OneDimensional/
│           │   ├── Axis1D/
│           │   │   └── Axis1D.cpp
│           │   │
│           │   ├── Distribution1D/
│           │   │   ├── Custom1D.cc
│           │   │   ├── Random1D.cc
│           │   │   ├── Roberts1D.cc
│           │   │   └── Uniform1D.cc
│           │   │
│           │   ├── GridPattern1D/
│           │   │   ├── FaceCentered1D.cpp
│           │   │   └── VolumeCentered1D.cpp
│           │   │
│           │   └── Operations1D/
│           │       └── Operations1D.cc
│           │
│           ├── TwoDimensional/
│           │   └── StructuredGrid2D/
│           │       └── StructuredGrid2D.cc
│           │
│           └── Output/
│               ├── CSV/
│               │   └── Axis1DCSVWriter.cc
│               │
│               └── VTK/
│                   └── LegacyVTKRectilinearGrid2DWriter.cc
│
├── examples/
│   ├── Ex_Minimal/
│   │   └── ex_Minimal.cc
│   │
│   ├── ErrorHandling/
│   │   └── ex_ErrorHandling.cpp
│   │
│   ├── OneDimensional/
│   │   ├── Axis1D/
│   │   │   └── ex_Axis1D.cc
│   │   │
│   │   └── Distribution1D/
│   │       ├── ex_Custom1D.cc
│   │       ├── ex_Random1D.cc
│   │       ├── ex_Roberts1D.cc
│   │       └── ex_Uniform1D.cc
│   │
│   ├── Output/
│   │   ├── CSV/
│   │   │   └── ex_Axis1DCSVWriter.cc
│   │   │
│   │   └── VTK/
│   │       └── ex_LegacyVTKRectilinearGrid2DWriter.cc
│   │
│   └── TwoDimensional/
│       └── StructuredGrid2D/
│           └── ex_StructuredGrid2D.cc
│
├── tests/
│   ├── Core/
│   │   ├── tst_ID.cc
│   │   ├── tst_StrongTypes.cc
│   │   ├── tst_Types.cc
│   │   └── tst_Version.cc
│   │
│   ├── ErrorHandling/
│   │   ├── tst_BuiltInErrors.cc
│   │   ├── tst_ErrorContext.cc
│   │   ├── tst_ErrorDescriptor.cc
│   │   ├── tst_ErrorRecord.cc
│   │   ├── tst_ErrorTraits.cc
│   │   ├── tst_FVGridException.cc
│   │   └── tst_ThrowError.cc
│   │
│   ├── OneDimensional/
│   │   ├── Axis1D/
│   │   │   └── tst_Axis1D.cc
│   │   │
│   │   ├── Distribution1D/
│   │   │   ├── tst_Custom1D.cc
│   │   │   ├── tst_Random1D.cc
│   │   │   ├── tst_Roberts1D.cc
│   │   │   └── tst_Uniform1D.cc
│   │   │
│   │   ├── GridPattern1D/
│   │   │   ├── tst_Coordinates1D.cc
│   │   │   ├── tst_Domain1D.cc
│   │   │   └── tst_GridPattern1D.cc
│   │   │
│   │   └── Operations1D/
│   │       └── tst_Operations1D.cc
│   │
│   ├── Output/
│   │   ├── CSV/
│   │   │   └── tst_Axis1DCSVWriter.cc
│   │   │
│   │   └── VTK/
│   │       └── tst_LegacyVTKRectilinearGrid2DWriter.cc
│   │
│   └── TwoDimensional/
│       └── StructuredGrid2D/
│           └── tst_StructuredGrid2D.cc
│
└── capitulos/
    └── <book examples and exercises>
```

## 3. Module notes

### Core

`Core` contains fundamental types, strong types, version metadata and component identity.

### ErrorHandling

`ErrorHandling` contains typed built-in error tags, descriptors, traits, contextual key/value diagnostics, error records, the base exception and header-only throwing helpers.

### OneDimensional

`OneDimensional` contains the stable 1D API:

```text
Axis1D;
Distribution1D;
GridPattern1D;
Operations1D.
```

### Output

`Output/CSV` is stable for `Axis1D`.

`Output/VTK` is experimental and currently targets two-dimensional structured grids.

### TwoDimensional

`TwoDimensional` is experimental. It contains coordinate-system support and structured 2D grids built from two independent `Axis1D` objects.

## 4. Regeneration rule

This tree should be updated whenever a public header, source file, example or test is added, removed or renamed.

Files generated by the build system must not be listed as source-tree files.
