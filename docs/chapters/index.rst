Programas dos capítulos
=======================

Programas didáticos ficam em ``capitulos/`` e usam o prefixo ``ex_``. Eles são
compilados separadamente para que exemplos do livro não introduzam dependências
na biblioteca.

Conteúdo atual
--------------

* Capítulo 3: protótipo histórico de malha aleatória;
* Capítulo 4, exercício 4.4: malha 1D simétrica.

.. code-block:: bash

   cmake -S . -B build-book -DBUILD_BOOK=ON \
         -DBUILD_EXAMPLES=OFF -DBUILD_TESTS=OFF
   cmake --build build-book -j

Somente arquivos ``ex_*.cpp`` ou ``ex_*.cc`` são material executável validado.
Cada novo capítulo deve explicar objetivo, entrada, construção da malha, saída
esperada e relação com as equações do texto.

