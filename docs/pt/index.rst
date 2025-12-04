.. FVGridMaker documentação principal

FVGridMaker
===========

Bem-vindo à documentação da biblioteca **FVGridMaker**.

A FVGridMaker é uma biblioteca desenvolvida em C++20 para geração de malhas estruturadas.
Ela foi pensada para ser empregada em códigos científicos que resolvem equações diferenciais empregando o método de Volumes Finitos.

Na elaboração da biblioteca, definiu-se que ela deveria possuir as seguintes características principais:

- desempenho em código científico,
- uso extensivo da STL e paralelização em C++ moderno,
- extensibilidade via traits, policies e factories,
- integração simples com códigos existentes.


Seções principais
-----------------

.. grid:: 2
   :gutter: 2

   .. grid-item-card:: Visão geral
      :link: user_guide/introduction
      :link-type: doc

      Objetivos do projeto, contexto de uso, requisitos de compilação
      e estado atual do desenvolvimento.

   .. grid-item-card:: Instalação
      :link: user_guide/install
      :link-type: doc

      Como obter o código, configurar o CMake, compilar a biblioteca,
      rodar testes e gerar a documentação.

   .. grid-item-card:: Guia do usuário
      :link: user_guide/index
      :link-type: doc

      Exemplos práticos de criação de malhas 1D, iteração sobre volumes,
      exportação para VTK/CSV e tratamento de erros.

   .. grid-item-card:: Referência da API C++
      :link: api
      :link-type: doc

      Documentação automática dos headers C++ (Doxygen + Breathe).

Sumário detalhado
-----------------

.. toctree::
   :maxdepth: 2
   :caption: Conteúdo

   user_guide/introduction
   user_guide/install
   