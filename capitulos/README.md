# Programas computacionais dos capítulos

Esta árvore contém somente programas vinculados ao texto do livro. Ela não
define API da FVGridMaker.

Um exercício executável deve:

- usar o prefixo `ex_`;
- mencionar capítulo e exercício no comentário inicial;
- explicar dados de entrada, sistema de coordenadas e resultado esperado;
- reutilizar a FVGridMaker em vez de reconstruir a geometria localmente;
- gravar saídas apenas no diretório de execução;
- compilar sem incluir arquivos privados de outros capítulos.

```bash
cmake -S . -B build-book -DBUILD_BOOK=ON \
      -DBUILD_EXAMPLES=OFF -DBUILD_TESTS=OFF
cmake --build build-book -j
```

Atualmente o exercício 4.4 é o programa de capítulo ativo. O arquivo do
capítulo 3 sem prefixo `ex_` é um protótipo histórico e não faz parte do build
validado.
