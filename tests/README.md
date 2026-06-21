# Testes da FVGridMaker

A suíte oficial usa arquivos `tst_*.cc`, `tst_*.cpp` ou `tst_*.cxx`. Cada
arquivo gera um executável independente e é registrado no CTest.

Os diretórios antigos com arquivos `ut_*` pertencem a uma API anterior e não
são compilados. Eles permanecem apenas como histórico; não devem ser usados
como evidência de cobertura da biblioteca atual.

Critérios para novos testes:

- um caso nominal e pelo menos um caso de erro por invariante público;
- conferir conteúdo e ordem dos vetores, não apenas seus tamanhos;
- usar sementes fixas em testes aleatórios;
- testar índices nos limites e imediatamente fora deles;
- testar NaN/infinito para entradas reais;
- limpar arquivos temporários produzidos por writers.

```bash
cmake -S . -B build-tests -DBUILD_TESTS=ON -DBUILD_EXAMPLES=OFF
cmake --build build-tests -j
ctest --test-dir build-tests --output-on-failure
```
