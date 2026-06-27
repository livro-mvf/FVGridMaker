# Testes da FVGridMaker

A suÃ­te oficial usa arquivos `tst_*.cc`, `tst_*.cpp` ou `tst_*.cxx`. Cada
arquivo gera um executÃ¡vel independente e Ã© registrado no CTest.

Os diretÃ³rios antigos com arquivos `ut_*` pertencem a uma API anterior e nÃ£o
sÃ£o compilados. Eles permanecem apenas como histÃ³rico; nÃ£o devem ser usados
como evidÃªncia de cobertura da biblioteca atual.

CritÃ©rios para novos testes:

- um caso nominal e pelo menos um caso de erro por invariante pÃºblico;
- conferir conteÃºdo e ordem dos vetores, nÃ£o apenas seus tamanhos;
- usar sementes fixas em testes aleatÃ³rios;
- testar Ã­ndices nos limites e imediatamente fora deles;
- testar NaN/infinito para entradas reais;
- em 2D, cobrir padrões incompatíveis, pontos físicos finitos, métricas analíticas, operações lógicas e writers VTK separados;
- limpar arquivos temporÃ¡rios produzidos por writers.

```bash
cmake -S . -B build-tests -DBUILD_TESTS=ON -DBUILD_EXAMPLES=OFF
cmake --build build-tests -j
ctest --test-dir build-tests --output-on-failure
```
