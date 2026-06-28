# Testes da FVGridMaker

A suíte oficial usa arquivos `tst_*.cc`, `tst_*.cpp` ou `tst_*.cxx`. Cada arquivo gera um executável independente e é registrado no CTest.

Comandos:

```bash
cmake -S . -B build-tests -DBUILD_TESTS=ON -DBUILD_EXAMPLES=OFF
cmake --build build-tests -j
ctest --test-dir build-tests --output-on-failure
```

Critérios para novos testes:

- cobrir caso nominal e pelo menos um erro por invariante público;
- conferir conteúdo e ordem dos vetores, não apenas tamanhos;
- testar `float`, `double` e `long double` quando o componente for template;
- usar tolerâncias dependentes do tipo escalar;
- usar sementes fixas em testes aleatórios;
- testar índices nos limites e imediatamente fora deles;
- em 1D, validar `faces[p] < centers[p] < faces[p + 1]`;
- em 2D, validar ordem row-major, medidas positivas e distinção entre `cell_logical_area` e `cell_measure`;
- limpar arquivos temporários produzidos por writers.

Os diretórios antigos com arquivos `ut_*` pertencem a uma API anterior e não são compilados pela suíte atual. Eles permanecem como histórico e não devem ser usados como evidência de cobertura.