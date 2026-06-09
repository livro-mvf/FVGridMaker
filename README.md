# FVGridMaker

FVGridMaker is a C++20 library for building structured finite-volume grids. The current implementation focuses on one-dimensional structured grids and establishes the core infrastructure that will later support two-dimensional and three-dimensional grid construction, metric evaluation, external configuration examples and export utilities.

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
- pattern-aware uniform one-dimensional grid generation through `Uniform1D`;
- pattern-aware random one-dimensional grid generation through `Random1D`;
- generic custom one-dimensional axis generation through `Custom1D`;
- volume-centred reconstruction from faces through `VolumeCentered1D`;
- face-centred reconstruction from centres through `FaceCentered1D`;
- examples and tests for the implemented components.

`Uniform1D` and `Random1D` generate primary coordinates according to the selected grid pattern. `VolumeCentered1D` receives face coordinates and reconstructs centre coordinates. `FaceCentered1D` receives centre coordinates and reconstructs face coordinates using the physical domain.

YAML support is not part of `FVGridMakerLib`. YAML may be used by examples or external applications, but the library itself must not depend on YAML, `yaml-cpp` or any other configuration parser.

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
- keep examples small, executable and suitable for documentation;
- keep configuration parsers, including YAML, outside the library core.

## Repository layout

The active implementation is located in:

```text
FVGridMakerLib/
```

The previous implementation, when present, is kept only for reference in:

```text
FVGridMakerLibOld/
```

The current source tree is organised as:

```text
FVGridMakerLib/
  include/FVGridMaker/
    FVGridMaker.h

    Core/
      ID.h
      StrongTypes.h
      Types.h
      Version.h

    ErrorHandling/
      ErrorCatalog.h
      ErrorCodes.h
      ErrorDescriptor.h
      ErrorRecord.h
      FVGridException.h
      ThrowError.h

    OneDimensional/
      Axis1D/
        Axis1D.h

      Distribution1D/
        Custom1D.h
        Random1D.h
        Uniform1D.h

      GridPattern1D/
        AxisGeometry1D.h
        CoordinateKind1D.h
        Coordinates1D.h
        Domain1D.h
        FaceCentered1D.h
        VolumeCentered1D.h

  src/FVGridMaker/
    FVGridMaker.cc

    Core/
      ID.cc
      Version.cc

    ErrorHandling/
      FVGridException.cc
      ThrowError.cc

    OneDimensional/
      Axis1D/
        Axis1D.cpp

      Distribution1D/
        Custom1D.cc
        Random1D.cc
        Uniform1D.cc

      GridPattern1D/
        FaceCentered1D.cpp
        VolumeCentered1D.cpp
```

Examples are located in:

```text
examples/
```

Tests are located in:

```text
tests/
```

## Core module

The `Core` module currently provides:

- `Real`;
- `Index`;
- `Size`;
- fixed-width integer aliases;
- strongly typed configuration values;
- version constants and accessors;
- immutable class identity through `ID`.

Important strong types include:

```cpp
fvgrid::NVol
fvgrid::Length
fvgrid::XInit
fvgrid::XFinal
fvgrid::MinSpacing
fvgrid::Seed
```

These types are intentionally not aliases of raw numerical types. They prevent accidental argument swaps in grid-generation functions.

Example:

```cpp
const fvgrid::Axis1D axis = fvgrid::Uniform1D::make(
    fvgrid::NVol{10},
    fvgrid::Length{1.0},
    fvgrid::XInit{0.0}
);
```

## Class identity

Every relevant class-like component should expose a stable identity.

The identity is represented by:

```cpp
fvgrid::ID
```

An `ID` stores:

```text
module
class_name
class_id
```

For example, `Axis1D` identifies itself as:

```text
module     : OneDimensional
class name : Axis1D
class id   : fvgrid.1d.axis.Axis1D
```

This identity is used by the error-handling system to identify where an error occurred.

## Error handling

FVGridMaker uses descriptor-based error handling without enum-based catalogues.

The main components are:

```cpp
fvgrid::ErrorDescriptor
fvgrid::ErrorRecord
fvgrid::FVGridException
fvgrid::throw_error()
fvgrid::require()
```

Built-in error descriptors are defined in:

```cpp
fvgrid::error_catalog
```

Built-in textual error codes are defined in:

```cpp
fvgrid::error_code
```

The design is intentionally based on `std::string_view` identifiers, not enums. This allows external projects to define their own error codes and descriptors without modifying FVGridMaker internals.

A diagnostic record stores:

```text
code
message
category
source ID
source location
```

Example use:

```cpp
fvgrid::require(
    condition,
    fvgrid::error_catalog::kInvalidArgument,
    fvgrid::ID{
        "Examples",
        "MyExample",
        "fvgrid.examples.MyExample"
    }
);
```

When an error is thrown, `FVGridException` carries the full `ErrorRecord`.

## One-dimensional coordinate input

Custom one-dimensional axis generation uses typed primary-coordinate input.

The structural coordinate kind is represented by:

```cpp
fvgrid::CoordinateKind1D
```

The two possible values are:

```text
Faces
Centers
```

This enum is deliberately small and closed. It does not enumerate grid patterns. It only states whether the user-provided coordinates are face coordinates or centre coordinates.

The public wrapper is:

```cpp
fvgrid::Coordinates1D
```

Typical use:

```cpp
fvgrid::Coordinates1D::faces(
    std::vector<fvgrid::Real>{0.0, 0.2, 0.7, 1.0}
);
```

or:

```cpp
fvgrid::Coordinates1D::centers(
    std::vector<fvgrid::Real>{0.1, 0.45, 0.85}
);
```

The physical domain, when required, is represented by:

```cpp
fvgrid::Domain1D
```

For example:

```cpp
fvgrid::Domain1D::from_length(
    fvgrid::XInit{0.0},
    fvgrid::Length{1.0}
);
```

or:

```cpp
fvgrid::Domain1D::from_bounds(
    fvgrid::XInit{0.0},
    fvgrid::XFinal{1.0}
);
```

## One-dimensional grid patterns

The current grid-pattern descriptors are:

```cpp
fvgrid::VolumeCentered1D
fvgrid::FaceCentered1D
```

Each pattern declares exactly one accepted primary-coordinate kind through:

```cpp
input_kind()
```

The rule is:

```text
VolumeCentered1D:
  input kind = Faces
  input      = face coordinates
  output     = centre coordinates

FaceCentered1D:
  input kind = Centers
  input      = centre coordinates
  output     = face coordinates
```

For volume-centred construction:

```text
VolumeCentered1D reconstructs centre coordinates from face coordinates.
Axis1D stores the completed geometry and computes metrics.
```

The rule implemented by `VolumeCentered1D` is:

```text
xcenter[i] = 0.5 * (xface[i] + xface[i + 1])
```

For face-centred reconstruction:

```text
FaceCentered1D reconstructs face coordinates from centre coordinates and
the physical domain bounds.
Axis1D stores the completed geometry and computes metrics.
```

The rule implemented by `FaceCentered1D` is:

```text
face[0]     = xmin
face[nvol]  = xmax
face[i]     = 0.5 * (center[i - 1] + center[i])
```

Completed temporary geometry is represented by:

```cpp
fvgrid::AxisGeometry1D
```

This type is an intermediate construction object. The final persistent storage is `Axis1D`.

## Axis1D

`Axis1D` is the basic one-dimensional finite-volume axis container.

It stores completed one-dimensional geometry:

```text
faces
centres
dx_faces
dx_centers
```

The arrays have the following sizes:

```text
faces      : nvol + 1
centres    : nvol
dx_faces   : nvol
dx_centers : nvol + 1
```

The metric definitions are geometric and independent of the construction pattern once both face and centre coordinates are known:

```text
dx_faces[i] = faces[i + 1] - faces[i]
```

and

```text
dx_centers[0]    = centres[0] - faces[0]
dx_centers[i]    = centres[i] - centres[i - 1], for 1 <= i < nvol
dx_centers[nvol] = faces[nvol] - centres[nvol - 1]
```

`Axis1D` also validates:

- face count;
- centre count;
- strictly increasing faces;
- strictly increasing centres;
- centre coordinates inside the physical domain.

`Axis1D` supports formatted stream output:

```cpp
std::cout << axis << '\n';
```

## Uniform1D

`Uniform1D` generates uniform primary coordinates and delegates secondary-coordinate reconstruction to the selected grid pattern.

The default construction remains volume-centred:

```cpp
const fvgrid::Axis1D axis = fvgrid::Uniform1D::make(
    fvgrid::NVol{10},
    fvgrid::Length{1.0},
    fvgrid::XInit{0.0}
);
```

This is equivalent to:

```cpp
const fvgrid::Axis1D axis = fvgrid::Uniform1D::make(
    fvgrid::NVol{10},
    fvgrid::Length{1.0},
    fvgrid::XInit{0.0},
    fvgrid::VolumeCentered1D{}
);
```

For `VolumeCentered1D`, `Uniform1D` generates face coordinates and `VolumeCentered1D` reconstructs centres.

For `FaceCentered1D`, `Uniform1D` generates centre coordinates and `FaceCentered1D` reconstructs faces:

```cpp
const fvgrid::Axis1D axis = fvgrid::Uniform1D::make(
    fvgrid::NVol{10},
    fvgrid::Length{1.0},
    fvgrid::XInit{0.0},
    fvgrid::FaceCentered1D{}
);
```

## Random1D

`Random1D` generates random primary coordinates and delegates secondary-coordinate reconstruction to the selected grid pattern.

The default construction is volume-centred:

```cpp
const fvgrid::Axis1D axis = fvgrid::Random1D::make(
    fvgrid::NVol{8},
    fvgrid::Length{1.0},
    fvgrid::XInit{0.0},
    fvgrid::Seed{1234}
);
```

A minimum spacing may be imposed:

```cpp
const fvgrid::Axis1D axis = fvgrid::Random1D::make(
    fvgrid::NVol{8},
    fvgrid::Length{1.0},
    fvgrid::XInit{0.0},
    fvgrid::Seed{1234},
    fvgrid::MinSpacing{0.05}
);
```

The explicit pattern-aware overloads are:

```cpp
const fvgrid::Axis1D axis = fvgrid::Random1D::make(
    fvgrid::NVol{8},
    fvgrid::Length{1.0},
    fvgrid::XInit{0.0},
    fvgrid::Seed{1234},
    fvgrid::MinSpacing{0.05},
    fvgrid::VolumeCentered1D{}
);
```

and

```cpp
const fvgrid::Axis1D axis = fvgrid::Random1D::make(
    fvgrid::NVol{8},
    fvgrid::Length{1.0},
    fvgrid::XInit{0.0},
    fvgrid::Seed{1234},
    fvgrid::MinSpacing{0.05},
    fvgrid::FaceCentered1D{}
);
```

For `VolumeCentered1D`, `Random1D` generates random face coordinates.

For `FaceCentered1D`, `Random1D` generates random centre coordinates inside the domain.

In both cases, the final geometry is stored in `Axis1D`.

## Custom1D

`Custom1D` generates a one-dimensional axis from user-provided primary coordinates.

It does not know concrete grid patterns. Instead, it requires the selected pattern to declare:

```cpp
input_kind()
complete_geometry(...)
```

`Custom1D` checks whether the provided coordinate kind is compatible with the selected pattern.

Volume-centred custom example:

```cpp
const fvgrid::Axis1D axis = fvgrid::Custom1D::make(
    fvgrid::Coordinates1D::faces(
        std::vector<fvgrid::Real>{0.0, 0.2, 0.7, 1.0}
    ),
    fvgrid::VolumeCentered1D{}
);
```

Face-centred custom example:

```cpp
const fvgrid::Axis1D axis = fvgrid::Custom1D::make(
    fvgrid::Coordinates1D::centers(
        std::vector<fvgrid::Real>{0.1, 0.45, 0.85}
    ),
    fvgrid::FaceCentered1D{},
    fvgrid::Domain1D::from_length(
        fvgrid::XInit{0.0},
        fvgrid::Length{1.0}
    )
);
```

Invalid combinations are rejected, for example:

```text
Coordinates1D::centers(...) + VolumeCentered1D
Coordinates1D::faces(...)   + FaceCentered1D
```

## YAML examples

YAML is not part of `FVGridMakerLib`.

Future YAML examples must live under `examples/` and must behave like external applications. They may depend on `yaml-cpp`, but only their own example targets may link against it.

A YAML example should parse a configuration file and translate it into public FVGridMaker API calls, such as:

```cpp
fvgrid::Uniform1D::make(...)
fvgrid::Random1D::make(...)
fvgrid::Custom1D::make(...)
fvgrid::Coordinates1D::faces(...)
fvgrid::Coordinates1D::centers(...)
fvgrid::Domain1D::from_length(...)
fvgrid::Domain1D::from_bounds(...)
```

The library target `FVGridMaker` must not include or link against YAML dependencies.

## Building

From the repository root:

```bash
cmake -S . -B build
cmake --build build
```

Or, from inside the build directory:

```bash
cmake ..
make
```

## Running examples

Available examples include:

```bash
make run_ex_Minimal
make run_ex_ErrorHandling
make run_ex_Axis1D
make run_ex_Uniform1D
make run_ex_Random1D
make run_ex_Custom1D
```

All examples can be run with:

```bash
make run_all_examples
```

The `Uniform1D`, `Random1D` and `Custom1D` examples print generated one-dimensional grids as tables with:

```text
i
xface[i]
xcenter[i]
dxface[i]
dxcenter[i]
```

## Running tests

Individual tests can be executed through the generated `run_tst_*` targets.

Important current tests include:

```bash
make run_tst_ID
make run_tst_Types
make run_tst_StrongTypes
make run_tst_Version
make run_tst_ErrorCodes
make run_tst_ErrorCatalog
make run_tst_ErrorRecord
make run_tst_FVGridException
make run_tst_ThrowError
make run_tst_Coordinates1D
make run_tst_Domain1D
make run_tst_GridPattern1D
make run_tst_Axis1D
make run_tst_Uniform1D
make run_tst_Random1D
make run_tst_Custom1D
```

The complete test suite can be run with:

```bash
ctest --output-on-failure
```

or through the project target:

```bash
make run_all_tests
```

## Current implementation status

Implemented and validated:

```text
Core
ErrorHandling
GridPattern1D metadata
CoordinateKind1D
Coordinates1D
Domain1D
AxisGeometry1D
VolumeCentered1D centre reconstruction
FaceCentered1D face reconstruction
Axis1D geometry and metric storage
Uniform1D pattern-aware generation
Random1D pattern-aware generation
Custom1D primary-coordinate construction
Minimal examples
Unit tests
```

Current `Uniform1D` status:

```text
volume-centred default path       : implemented
explicit VolumeCentered1D path    : implemented
explicit FaceCentered1D path      : implemented
```

Current `Random1D` status:

```text
volume-centred default path       : implemented
explicit VolumeCentered1D path    : implemented
explicit FaceCentered1D path      : implemented
seed-based reproducibility        : implemented
minimum spacing control           : implemented
```

Current `Custom1D` status:

```text
input from faces with VolumeCentered1D   : implemented
input from centers with FaceCentered1D   : implemented
coordinate-kind validation              : implemented
domain-aware face reconstruction         : implemented
```

## Development conventions

Source files should use the standard FVGridMaker header:

```cpp
// ----------------------------------------------------------------------------
// File: <FileName>
// Project: FVGridMaker
// Version: 0.1.0
// Description: <short description>
// Author: FVGridMaker Team
// License: MIT
// ----------------------------------------------------------------------------
```

Include sections should follow this order:

```cpp
// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
```

Code and comments are written in English.

## Error-handling convention

New FVGridMaker classes that can report errors should expose an identity:

```cpp
[[nodiscard]] static constexpr fvgrid::ID id() noexcept;
```

Errors should be raised through:

```cpp
fvgrid::require(
    condition,
    fvgrid::error_catalog::<descriptor>,
    ClassName::id()
);
```

Do not introduce enum-based error codes.

For new error types, add textual codes and descriptors instead.

## Roadmap

Near-term planned work:

```text
1. Add Operations1D.
2. Add coordinate-system metric modules.
3. Add structured 2D grid composition.
4. Add output/export utilities.
5. Add external YAML examples.
```

## License

MIT License.