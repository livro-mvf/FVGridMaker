# Exemplos da FVGridMaker

Os exemplos suportados usam o prefixo `ex_` e são compilados automaticamente
com `BUILD_EXAMPLES=ON`. Cada programa deve ser independente, usar somente a
API pública e explicar no próprio código: objetivo, construção, grandezas
impressas e arquivos produzidos.

Ordem recomendada:

1. `OneDimensional/Axis1D`;
2. `OneDimensional/Distribution1D`;
3. `OneDimensional/Operations1D`;
4. `TwoDimensional/StructuredGrid2D`;
5. `TwoDimensional/CoordinateSystems2D`;
6. `Output/VTK`.

As pastas `Grid/` e `IO/` contêm protótipos históricos `Ex_*` que não entram
nos alvos oficiais. Novos exemplos não devem ser adicionados nessas pastas.

```bash
cmake -S . -B build-examples -DBUILD_EXAMPLES=ON -DBUILD_TESTS=OFF
cmake --build build-examples -j
cmake --build build-examples --target run_all_examples
```

