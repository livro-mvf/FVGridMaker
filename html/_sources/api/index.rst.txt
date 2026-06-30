Referência da API C++
=====================

Esta seção organiza a superfície pública da FVGridMaker em duas camadas:

* páginas temáticas, escritas para leitura humana e revisão acadêmica;
* referência completa, gerada a partir dos headers públicos com Doxygen e
  Breathe.

As declarações C++ geradas aparecem em um único ponto, em
:doc:`complete`. Isso evita duplicação de símbolos no Sphinx e torna o build
estrito útil: qualquer aviso passa a indicar um problema real de documentação.

.. toctree::
   :maxdepth: 2

   core
   one_dimensional
   two_dimensional
   output
   files
   complete
