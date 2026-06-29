Núcleo, tipos e erros
=====================

O núcleo da biblioteca define nomes estáveis, tipos escalares, wrappers fortes
e erros estruturados. Esses tipos aparecem em praticamente todo exemplo e
formam a base do contrato acadêmico da biblioteca: unidades lógicas diferentes
não devem ser confundidas por acidente.

Tipos fundamentais
------------------

.. list-table::
   :header-rows: 1

   * - Arquivo
     - Conteúdo documentado
   * - ``Core/Types.h``
     - aliases de inteiros, ``Size``, ``Index`` e ``Real = double``
   * - ``Core/StrongTypes.h``
     - ``NVol``, ``Seed`` e wrappers escalares como ``Length`` e ``XInit``
   * - ``Core/ID.h``
     - identificação estável de módulo, classe e ``class_id``
   * - ``Core/Version.h``
     - versão semântica e metadados de build/git

A API iniciante usa ``double`` por padrão. A API avançada usa os aliases
``Float`` e ``LongDouble`` ou as classes ``Basic*<T>`` documentadas na
referência completa.

Erros e exceções
----------------

.. list-table::
   :header-rows: 1

   * - Arquivo
     - Papel
   * - ``ErrorHandling/ErrorDescriptor.h``
     - código, mensagem e categoria estáveis
   * - ``ErrorHandling/ErrorContext.h``
     - pares ``chave=valor`` para diagnóstico
   * - ``ErrorHandling/ErrorRecord.h``
     - registro completo com origem e localização
   * - ``ErrorHandling/FVGridException.h``
     - exceção única da biblioteca
   * - ``ErrorHandling/BuiltInErrors.h``
     - catálogo de erros de entrada, geometria, operação e saída
   * - ``ErrorHandling/ThrowError.h``
     - funções ``throw_error`` e ``require`` usadas nas validações

Para trabalhos acadêmicos, os códigos de erro permitem reproduzir e citar
falhas de validação sem depender do texto exato de uma mensagem.

Ver também
----------

* :doc:`../user_guide/errors`
* :doc:`complete`
