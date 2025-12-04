Instalação
==========

Esta seção descreve como obter, configurar e compilar a biblioteca
**FVGridMaker**.

Obter o código
--------------

Clone o repositório:

.. code-block:: bash

   git clone https://github.com/SEU-USUARIO/FVGridMaker.git
   cd FVGridMaker

Configurar o build
------------------

Crie um diretório de build separado:

.. code-block:: bash

   mkdir build
   cd build
   cmake ..

Compilar a biblioteca e testes
------------------------------

Para compilar a biblioteca e todos os testes:

.. code-block:: bash

   cmake --build .

Executar os testes (se configurado no CMake):

.. code-block:: bash

   ctest

Gerar a documentação
--------------------

Para gerar a documentação (Doxygen + Sphinx) usando o alvo ``docs``:

.. code-block:: bash

   cmake --build . --target docs

A documentação HTML será gerada em:

.. code-block:: text

   build/docs_build/

Se desejar copiar a documentação para um diretório de publicação
(por exemplo, ``documentation/`` na raiz do projeto), use o alvo:

.. code-block:: bash

   cmake --build . --target docs_publish
