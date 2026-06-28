Nucleo, tipos e erros
=====================

O nucleo da biblioteca define nomes estaveis, tipos escalares, wrappers fortes
e erros estruturados. Esses tipos aparecem em praticamente todo exemplo e
formam a base do contrato academico da biblioteca: unidades logicas diferentes
nao devem ser confundidas por acidente.

Tipos fundamentais
------------------

.. list-table::
   :header-rows: 1

   * - Header
     - Conteudo documentado
   * - ``Core/Types.h``
     - aliases de inteiros, ``Size``, ``Index`` e ``Real = double``
   * - ``Core/StrongTypes.h``
     - ``NVol``, ``Seed`` e wrappers escalares como ``Length`` e ``XInit``
   * - ``Core/ID.h``
     - identificacao estavel de modulo, classe e ``class_id``
   * - ``Core/Version.h``
     - versao semantica e metadados de build/git

A API iniciante usa ``double`` por padrao. A API avancada usa os aliases
``Float`` e ``LongDouble`` ou as classes ``Basic*<T>`` documentadas na
referencia completa.

Erros e excecoes
----------------

.. list-table::
   :header-rows: 1

   * - Header
     - Papel
   * - ``ErrorHandling/ErrorDescriptor.h``
     - codigo, mensagem e categoria estaveis
   * - ``ErrorHandling/ErrorContext.h``
     - pares ``chave=valor`` para diagnostico
   * - ``ErrorHandling/ErrorRecord.h``
     - registro completo com origem e localizacao
   * - ``ErrorHandling/FVGridException.h``
     - excecao unica da biblioteca
   * - ``ErrorHandling/BuiltInErrors.h``
     - catalogo de erros de entrada, geometria, operacao e saida
   * - ``ErrorHandling/ThrowError.h``
     - funcoes ``throw_error`` e ``require`` usadas nas validacoes

Para trabalhos academicos, os codigos de erro permitem reproduzir e citar
falhas de validacao sem depender do texto exato de uma mensagem.

Ver tambem
----------

* :doc:`../user_guide/errors`
* :doc:`complete`