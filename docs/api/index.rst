Referencia da API C++
=====================

Esta seção organiza a superficie publica de FVGridMaker em duas camadas:

* paginas tematicas, escritas para leitura humana e revisao academica;
* referencia completa, gerada a partir dos headers publicos com Doxygen e
  Breathe.

As declaracoes C++ geradas aparecem em um unico ponto, em
:doc:`complete`. Isso evita duplicacao de simbolos no Sphinx e torna o build
estrito util: qualquer aviso passa a indicar um problema real de documentacao.

.. toctree::
   :maxdepth: 2

   core
   one_dimensional
   two_dimensional
   output
   files
   complete