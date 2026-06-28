# Exemplos da FVGridMaker

Os exemplos suportados usam o prefixo `ex_` e são compilados com `BUILD_EXAMPLES=ON`. Cada programa deve ser pequeno, independente, usar somente a API pública e servir como material didático.

Ordem recomendada de leitura:

1. `OneDimensional/Axis1D`
2. `OneDimensional/Distribution1D`
3. `OneDimensional/Operations1D`
4. `TwoDimensional/StructuredGrid2D`
5. `TwoDimensional/CoordinateSystems2D`
6. `TwoDimensional/Operations2D`
7. `TwoDimensional/Quality2D`
8. `Output/VTK`

Comandos:

```bash
cmake -S . -B build-examples -DBUILD_EXAMPLES=ON -DBUILD_TESTS=OFF
cmake --build build-examples -j
cmake --build build-examples --target run_all_examples
```

Diretrizes para novos exemplos:

- declarar o objetivo no início do arquivo;
- mostrar a construção da malha e as grandezas impressas;
- usar a API iniciante (`uniform_axis_1d`) quando o foco não for tipos fortes;
- usar tipos fortes quando o exemplo discutir contrato de API;
- manter sementes fixas em exemplos aleatórios;
- evitar dependências externas.

As pastas históricas `Grid/` e `IO/` contêm protótipos `Ex_*` de uma API anterior. Elas não entram nos alvos oficiais e não devem receber exemplos novos.