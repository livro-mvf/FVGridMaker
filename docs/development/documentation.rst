Manutenção da documentação
==========================

A documentação oficial combina texto manual em Sphinx com referência C++ gerada
a partir dos headers públicos.

Camadas
-------

* ``docs/user_guide``: explica o uso da biblioteca;
* ``docs/architecture``: registra decisões, invariantes e semântica numérica;
* ``docs/api``: organiza a API e inclui a referência Doxygen completa;
* ``README.md``: porta de entrada curta para o repositório;
* READMEs em subdiretórios: instruções locais de exemplos, testes e scripts.

Regra para API C++
------------------

A expansão de declarações Doxygen deve ficar centralizada em
``docs/api/complete.rst``. As páginas temáticas de ``docs/api`` não devem usar
``doxygenclass`` ou ``doxygenfile`` para os mesmos símbolos, porque isso gera
declarações duplicadas no domínio C++ do Sphinx.

Build
-----

.. code-block:: bash

   cmake -S . -B build_docs -DBUILD_DOCUMENTATION=ON \
         -DBUILD_EXAMPLES=OFF -DBUILD_TESTS=OFF
   cmake --build build_docs --target docs -j

O alvo usa Sphinx com ``-W --keep-going`` e ``-E``. Assim, warnings de
documentação quebram o build e o ambiente é reconstruído de forma limpa.

Critérios para novas páginas
----------------------------

* explicar a intenção numérica antes de listar funções;
* distinguir área lógica de medida física;
* declarar quando um comportamento é padrão e quando é escolha explícita;
* usar exemplos pequenos que compilem contra a API pública;
* evitar duplicar a referência completa da API;
* preferir tabelas para inventários de arquivos ou responsabilidades.