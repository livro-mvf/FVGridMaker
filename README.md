# FVGridMaker

FVGridMaker is a C++20 library for building structured finite-volume grids.

The current release scope is divided into two API groups:

```text
Stable API:
  Core
  ErrorHandling
  OneDimensional
  Axis1D CSV output

Experimental API:
  TwoDimensional
  VTK 2D output
```

The one-dimensional API is the stable core of the library. It provides typed grid-generation inputs, one-dimensional axis storage, grid-pattern reconstruction rules, deterministic and random one-dimensional coordinate distributions, interval operations and CSV output.

The two-dimensional API is available as an experimental public API. It provides tensor-product structured grids built from two independent `Axis1D` objects, coordinate-system traits, physical metric evaluation and basic VTK output. Experimental APIs are compiled and tested, but their interface may still change before the next stable release.

FVGridMaker is developed with an emphasis on clear numerical semantics, data-oriented storage, explicit grid-pattern rules, robust error diagnostics and a small public API.

## Scope

FVGridMaker is intended to handle:

```text
structured finite-volume grids;
one-dimensional axes;
tensor-product multidimensional grids built from one-dimensional axes;
face coordinates;
cell-centre coordinates;
face-to-face spacings;
centre-to-centre spacings;
geometric validation;
secondary-coordinate reconstruction from primary coordinates;
custom one-dimensional axis construction;
basic output for inspection and post-processing.
```

FVGridMaker is not intended to handle:

```text
differential equations;
physical fields;
physical boundary conditions;
diffusion operators;
advection operators;
source-term linearisation;
matrix assembly;
linear solvers;
time integration;
Voronoi meshes;
Delaunay triangulations;
CGAL-based geometry;
unstructured meshes;
internal YAML parsing.
```

YAML support is not part of `FVGridMakerLib`. YAML may be used by examples or external applications, but the library itself must not depend on YAML, `yaml-cpp` or any other configuration parser.

## API status

### Stable public API

The stable public API currently includes:

```text
Core
ErrorHandling
Axis1D
GridPattern1D
Uniform1D
Random1D
Roberts1D
Custom1D
Operations1D
Axis1DCSVWriter
```

Stable public modules should remain source-compatible within the same minor release series unless a documented breaking change is introduced.

### Experimental public API

The experimental public API currently includes:

```text
CoordinateSystem2D
CoordinateMetrics2D
CoordinateMappingFactory2D
StructuredGrid2D
LegacyVTKRectilinearGrid2DWriter
```

Experimental modules are available to users and examples, but their API may still change before stabilisation.

### Future work

Future work may include:

```text
three-dimensional structured grids;
external YAML examples;
binary output;
additional coordinate mappings;
additional output formats;
stronger installation and packaging tests.
```

## Design principles

FVGridMaker follows these design rules:

```text
use C++20;
favour contiguous storage and data-oriented design;
expose read-only array views through std::span;
avoid runtime polymorphism in core grid and metric paths;
avoid virtual classes in the core API;
use concepts, traits, templates, factories and value composition for extension;
avoid enum-based catalogues for extensible concepts;
allow small closed enums only for non-extensible structural choices;
use textual, stable identifiers for errors and class identity;
separate grid geometry storage from grid-pattern reconstruction rules;
keep grid metrics explicit and stored in dedicated arrays;
keep examples small, executable and suitable for documentation;
keep configuration parsers outside the library core.
```

## Repository layout

The active implementation is located in:

```text
FVGridMakerLib/
```

The current source tree is organised around these modules:

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
        Roberts1D.h
        Uniform1D.h

      GridPattern1D/
        AxisGeometry1D.h
        CoordinateKind1D.h
        Coordinates1D.h
        Domain1D.h
        FaceCentered1D.h
        VolumeCentered1D.h

      Operations1D/
        AxisInterval1D.h
        Operations1D.h

    TwoDimensional/
      CoordinateSystem2D/
        CoordinateMappingFactory2D.h
        CoordinateMetrics2D.h
        CoordinateSystem2D.h

      StructuredGrid2D/
        StructuredGrid2D.h

    Output/
      CSV/
        Axis1DCSVWriter.h

      VTK/
        LegacyVTKRectilinearGrid2DWriter.h

  src/FVGridMaker/
    Core/
    ErrorHandling/
    OneDimensional/
    TwoDimensional/
    Output/
```

Examples are located in:

```text
examples/
```

Tests are located in:

```text
tests/
```

Book-related examples and exercises, when present, are located in:

```text
capitulos/
```

## Core module

The `Core` module provides:

```text
Real;
Index;
Size;
fixed-width integer aliases;
strongly typed configuration values;
version constants and accessors;
immutable component identity through ID.
```

Important strong types include:

```cpp
fvgrid::NVol
fvgrid::Length
fvgrid::XInit
fvgrid::XFinal
fvgrid::MinSpacing
fvgrid::Beta
fvgrid::Seed
```

These types are intentionally not aliases of raw numerical types. They reduce accidental argument swaps in grid-generation functions.

Example:

```cpp
const fvgrid::Axis1D axis = fvgrid::Uniform1D::make(
    fvgrid::NVol{10},
    fvgrid::Length{1.0},
    fvgrid::XInit{0.0}
);
```

## Class identity

Every relevant class-like component that can be the source of an error should expose a stable identity.

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

FVGridMaker uses descriptor-based error handling without enum-based error catalogues.

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

The design is intentionally based on textual identifiers, not enums. This allows external projects to define their own error codes and descriptors without modifying FVGridMaker internals.

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

The current one-dimensional grid-pattern descriptors are:

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
pattern_name
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

```text
face count;
centre count;
finite face coordinates;
finite centre coordinates;
strictly increasing faces;
strictly increasing centres;
centre coordinates inside the physical domain.
```

`Axis1D` supports formatted stream output:

```cpp
std::cout << axis << '\n';
```

## Uniform1D

`Uniform1D` generates uniform primary coordinates and delegates secondary-coordinate reconstruction to the selected grid pattern.

The default construction is volume-centred:

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

For `VolumeCentered1D`, `Random1D` generates random face coordinates.

For `FaceCentered1D`, `Random1D` generates random centre coordinates inside the domain.

`Random1D` uses seed-based reproducibility. Its random construction partitions the domain length into positive random spacings, optionally constrained by a minimum spacing.

## Roberts1D

`Roberts1D` generates one-dimensional stretched axes using a Roberts-type coordinate transformation.

The default construction is volume-centred:

```cpp
const fvgrid::Axis1D axis = fvgrid::Roberts1D::make(
    fvgrid::NVol{16},
    fvgrid::Length{1.0},
    fvgrid::XInit{0.0},
    fvgrid::Beta{1.5}
);
```

`Beta` controls the stretching intensity and must be greater than one.

Explicit grid patterns are also supported:

```cpp
const fvgrid::Axis1D axis = fvgrid::Roberts1D::make(
    fvgrid::NVol{16},
    fvgrid::Length{1.0},
    fvgrid::XInit{0.0},
    fvgrid::Beta{1.5},
    fvgrid::FaceCentered1D{}
);
```

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

## Operations1D

`Operations1D` provides geometric operations on completed `Axis1D` objects.

Current operations include:

```text
same_pattern()
require_same_pattern()
domain_interval()
intersection()
require_interval_intersection()
unique_sorted_coordinates()
clip_faces_to_interval()
```

`Operations1D` does not generate primary distributions. It operates on axes that have already been built and validated.

## Axis1D CSV output

`Axis1DCSVWriter` writes an `Axis1D` to CSV for inspection and post-processing.

Typical use:

```cpp
fvgrid::Axis1DCSVWriter::write(axis, "axis.csv");
```

The output contains face coordinates, centre coordinates and one-dimensional spacings.

## Experimental 2D API

The experimental 2D API builds structured two-dimensional grids from two independent `Axis1D` objects.

The main class is:

```cpp
fvgrid::StructuredGrid2D
```

A Cartesian grid can be built from two axes:

```cpp
const fvgrid::Axis1D x_axis = fvgrid::Uniform1D::make(
    fvgrid::NVol{4},
    fvgrid::Length{1.0},
    fvgrid::XInit{0.0}
);

const fvgrid::Axis1D y_axis = fvgrid::Uniform1D::make(
    fvgrid::NVol{3},
    fvgrid::Length{2.0},
    fvgrid::XInit{0.0}
);

const fvgrid::StructuredGrid2D grid{x_axis, y_axis};
```

Coordinate-system traits may be used to define the physical interpretation of the two independent coordinates. Current experimental coordinate systems include Cartesian, polar, axisymmetric and user-defined functional mappings.

The 2D API is available, but it is still classified as experimental.

## Experimental VTK output

The VTK writer is currently classified as experimental.

It is intended for exporting structured two-dimensional grids for inspection in external tools such as ParaView. Its exact public contract should be finalised before it is promoted to the stable API.

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

Examples are enabled by default.

A typical example build is:

```bash
cmake -S . -B build-examples -DBUILD_EXAMPLES=ON -DBUILD_TESTS=OFF
cmake --build build-examples
```

If the generated project contains `run_all_examples`, all examples can be run with:

```bash
cmake --build build-examples --target run_all_examples
```

## Running tests

Tests are disabled by default. Enable them explicitly:

```bash
cmake -S . -B build-tests -DBUILD_TESTS=ON -DBUILD_EXAMPLES=OFF
cmake --build build-tests
ctest --test-dir build-tests --output-on-failure
```

If the generated project contains `run_all_tests`, all test executables can also be run with:

```bash
cmake --build build-tests --target run_all_tests
```

## Development conventions

Source files should use the standard FVGridMaker header:

```cpp
// ----------------------------------------------------------------------------
// File: <FileName>
// Project: FVGridMaker
// Version: <version>
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

Do not introduce enum-based error codes. For new error types, add textual codes and descriptors instead.

## Public API inclusion rule

A header should be included in `FVGridMaker.h` only after it has been classified as one of:

```text
Stable public API
Experimental public API
```

A stable public API header should have:

```text
tests;
at least one usage path in examples or documentation;
stable naming;
stable error behaviour;
no virtual class requirement;
no YAML dependency;
no external geometry backend dependency.
```

An experimental public API header should compile and be testable, but its interface may still change.

## Roadmap

Near-term planned work:

```text
stabilise versioning;
finalise pattern_name semantics;
complete 2D API review;
complete VTK writer review;
add installation and consumer-project tests;
update manual and registration documentation;
prepare a release candidate.
```

## License

MIT License.
