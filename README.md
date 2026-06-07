# FVGridMaker

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE.md)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://isocpp.org/std/the-standard)

FVGridMaker is a C++20 structured-grid library for finite-volume applications. The project starts from a deliberately narrow bootstrap core: a clean build system, a minimal public API, runnable examples, GoogleTest-based tests, and a disciplined migration path from the previous prototype implementation.

The long-term goal is to provide structured one-dimensional and multidimensional grids assembled from independent 1D axes, with coordinate-system-aware geometric measures such as cell areas, face lengths, volumes and metric factors.

---

## Current bootstrap scope

The current implementation intentionally provides only the foundation layer:

- project structure;
- CMake build;
- Google-style `clang-format` configuration;
- MIT licence file;
- minimal public header;
- minimal library source;
- one runnable example;
- one GoogleTest unit test;
- automatic test execution through CTest;
- `make run_*` targets for examples and tests.

The validated bootstrap files are:

```text
FVGridMakerLib/include/FVGridMaker/FVGridMaker.h
FVGridMakerLib/src/FVGridMaker/FVGridMaker.cc
examples/Ex_Minimal/ex_Minimal.cc
tests/tst_Minimal.cc