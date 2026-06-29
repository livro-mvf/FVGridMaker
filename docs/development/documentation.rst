
Documentação
============

Esta página descreve como a documentação HTML do FVGridMaker é gerada e
qual papel cada ferramenta exerce no processo.

A documentação combina três camadas:

* páginas escritas manualmente em reStructuredText ou Markdown;
* referência da API C++ extraída dos headers públicos com Doxygen;
* integração entre Sphinx e Doxygen por meio do Breathe.

Como gerar a documentação HTML
------------------------------

Crie um ambiente Python para a documentação:

.. code-block:: bash

   python3 -m venv .venv-docs
   source .venv-docs/bin/activate
   python -m pip install --upgrade pip
   python -m pip install -r docs/requirements.txt

Configure um diretório de build dedicado:

.. code-block:: bash

   cmake -S . -B build_docs \
     -DBUILD_DOCUMENTATION=ON \
     -DBUILD_EXAMPLES=OFF \
     -DBUILD_TESTS=OFF

Gere a documentação:

.. code-block:: bash

   cmake --build build_docs --target docs -j

As páginas HTML são gravadas em:

.. code-block:: text

   build_docs/docs/html

Para abrir a página inicial no WSL:

.. code-block:: bash

   explorer.exe "$(wslpath -w build_docs/docs/html/index.html)"

ou, em um ambiente Linux com interface gráfica:

.. code-block:: bash

   xdg-open build_docs/docs/html/index.html

Papel do Sphinx
---------------

O Sphinx organiza as páginas da documentação, monta a navegação lateral,
processa as páginas ``.rst`` e ``.md`` e gera o HTML final.

O alvo CMake ``docs`` chama o alvo ``sphinx_html``. A geração é feita com
avisos tratados como erro, para evitar que referências quebradas ou páginas
malformadas passem despercebidas.

Papel do Doxygen
----------------

O Doxygen lê os headers públicos da biblioteca e gera a representação XML
da API C++. Esse XML é usado pelo Sphinx, por meio do Breathe, para montar
as páginas de referência da API.

Quando o Doxygen está instalado, o alvo ``doxygen_xml`` é executado antes
do Sphinx. Quando o Doxygen não está disponível, a documentação conceitual
ainda pode ser gerada, mas a referência automática da API fica incompleta.

Papel do Breathe
----------------

O Breathe conecta a saída XML do Doxygen ao Sphinx. Ele permite que as
páginas da documentação incluam classes, funções, aliases e estruturas C++
documentadas nos headers públicos.

Organização das páginas
-----------------------

A árvore principal da documentação é definida em:

.. code-block:: text

   docs/index.rst

As páginas de desenvolvimento ficam em:

.. code-block:: text

   docs/development/

A página ``testing.rst`` documenta a estratégia pública de testes. A página
``documentation.rst`` deve tratar apenas do processo de geração, manutenção
e organização da documentação.

Verificação rápida
------------------

Depois de qualquer alteração estrutural nos arquivos ``.rst``, gere a
documentação novamente:

.. code-block:: bash

   cmake --build build_docs --target docs -j

Se a geração terminar com ``build succeeded.``, a árvore Sphinx está
coerente.

