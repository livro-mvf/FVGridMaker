# FVGridMaker

FVGridMaker is a C++20 library for building structured finite-volume grids. The current implementation focuses on one-dimensional structured grids and establishes the core infrastructure that will later support two-dimensional and three-dimensional grid construction, metric evaluation, input parsing and export utilities.

The library is being developed with an emphasis on clear numerical semantics, data-oriented storage, explicit grid-pattern rules and robust error diagnostics.

## Current scope

FVGridMaker currently provides the foundation for structured finite-volume grid generation.

Implemented components include:

- project bootstrap and CMake integration;
- core scalar, index and strongly typed configuration values;
- immutable class/component identity through `ID`;
- descriptor-based error handling without enum-based error catalogues;
- one-dimensional grid-pattern descriptors;
- typed one-dimensional primary-coordinate input through `Coordinates1D`;
- one-dimensional domain description through `Domain1D`;
- one-dimensional axis storage through `Axis1D`;
- volume-centred uniform one-dimensional grid generation through `Uniform1D`;
- generic custom one-dimensional axis generation through `Custom1D`;
- volume-centred reconstruction from faces through `VolumeCentered1D`;
- face-centred reconstruction from centres through `FaceCentered1D`;
- examples and tests for the implemented components.

The current `Uniform1D` implementation is intentionally restricted to the volume-centred construction path. The face-centred reconstruction rule exists in `FaceCentered1D` and is already used by `Custom1D`, but a dedicated face-centred distribution path remains a later milestone.

## Design principles

FVGridMaker follows these design rules:

- use C++20;
- keep public APIs independent of external geometry backends;
- favour contiguous storage and data-oriented design;
- avoid runtime polymorphism in core grid and metric paths;
- avoid enum-based closed catalogues for extensible concepts;
- allow small closed enums only for non-extensible structural choices;
- use textual, stable identifiers for errors and class identity;
- separate grid geometry storage from grid-pattern reconstruction rules;
- keep grid metrics explicit and stored in dedicated arrays;
- keep examples small, executable and suitable for documentation.

## Repository layout

The active implementation is located in:

```text
FVGridMakerLib/