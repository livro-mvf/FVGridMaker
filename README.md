# FVGridMaker

FVGridMaker is a C++20 library for building structured finite-volume grids. The current implementation focuses on one-dimensional structured grids and establishes the core infrastructure that will later support two-dimensional and three-dimensional grid construction, metric evaluation, input parsing and export utilities.

The library is being developed with an emphasis on clear numerical semantics, data-oriented storage, explicit grid-pattern rules and robust error diagnostics.

## Current scope

FVGridMaker currently provides the foundation for structured finite-volume grid generation.

Implemented components include:

- project bootstrap and CMake integration;
- core scalar, index and strongly typed configuration values;
- immutable class/component identity through `ID`;
- descriptor-based error handling without enum-based catalogues;
- one-dimensional grid-pattern descriptors;
- one-dimensional axis storage through `Axis1D`;
- volume-centred uniform one-dimensional grid generation through `Uniform1D`;
- examples and tests for the implemented components.

The current `Uniform1D` implementation is intentionally restricted to the volume-centred construction path.

## Design principles

FVGridMaker follows these design rules:

- use C++20;
- keep public APIs independent of external geometry backends;
- favour contiguous storage and data-oriented design;
- avoid runtime polymorphism in core grid and metric paths;
- avoid enum-based closed catalogues for extensible concepts;
- use textual, stable identifiers for errors and class identity;
- separate grid geometry storage from grid-pattern reconstruction rules;
- keep grid metrics explicit and stored in dedicated arrays;
- keep examples small, executable and suitable for documentation.

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
        Uniform1D.h

      GridPattern1D/
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

## One-dimensional grid patterns

The current grid-pattern descriptors are:

```cpp
fvgrid::VolumeCentered1D
fvgrid::FaceCentered1D
```

At this stage, `VolumeCentered1D` has operational behaviour.

For volume-centred construction:

```text
Uniform1D generates face coordinates.
VolumeCentered1D reconstructs centre coordinates from face coordinates.
Axis1D stores the completed geometry and computes metrics.
```

The rule implemented by `VolumeCentered1D` is:

```text
xcenter[i] = 0.5 * (xface[i] + xface[i + 1])
```

`FaceCentered1D` currently stores metadata and has a reconstruction path under development. Full face-centred uniform grid generation is intentionally deferred to a later milestone.

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

`Uniform1D` generates a volume-centred uniform one-dimensional grid.

Current construction rule:

```text
Uniform1D generates faces.
VolumeCentered1D reconstructs centres.
Axis1D stores geometry and metrics.
```

Minimal example:

```cpp
#include <iostream>

#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

int main() {
    const fvgrid::Axis1D axis = fvgrid::Uniform1D::make(
        fvgrid::NVol{10},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0}
    );

    std::cout << axis << '\n';

    return 0;
}
```

For `NVol{10}`, `Length{1.0}` and `XInit{0.0}`, the generated faces are:

```text
0.0, 0.1, 0.2, ..., 1.0
```

and the centres are reconstructed as:

```text
0.05, 0.15, 0.25, ..., 0.95
```

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
```

The `Uniform1D` example prints the generated one-dimensional grid as a table with:

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
make run_tst_ErrorRecord
make run_tst_FVGridException
make run_tst_ThrowError
make run_tst_ErrorCatalog
make run_tst_GridPattern1D
make run_tst_Axis1D
make run_tst_Uniform1D
```

The complete test suite can be run with:

```bash
ctest --output-on-failure
```

## Current implementation status

Implemented and validated:

```text
Core
ErrorHandling
GridPattern1D metadata
VolumeCentered1D centre reconstruction
Axis1D geometry and metric storage
Uniform1D volume-centred generation
Minimal examples
Unit tests
```

Current `Uniform1D` status:

```text
volume-centred path : implemented
face-centred path   : deferred
non-uniform path    : deferred
random path         : deferred
YAML input          : deferred
2D grids            : deferred
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
1. Stabilise Axis1D stream output and tests.
2. Add explicit Custom1D construction from complete data.
3. Add a dedicated face-centred construction path.
4. Add non-uniform 1D distributions.
5. Add operations on compatible 1D grids.
6. Add coordinate-system metric modules.
7. Add structured 2D grid composition.
8. Add YAML input support.
9. Add export utilities.
```

## License

FVGridMaker is distributed under the MIT License.

See:

```text
LICENSE.md
```

for details.