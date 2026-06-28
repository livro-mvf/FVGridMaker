Arquitetura escalar
===================

A biblioteca usa uma convencao dupla: nomes simples para a API iniciante e
classes ``Basic*<T>`` para controle explicito do tipo escalar. Essa escolha
preserva clareza em sala de aula e permite experimentos com ``float`` ou
``long double`` quando o estudo numerico exigir.

Camada iniciante
----------------

Os nomes publicos sem sufixo significam ``double``:

.. code-block:: cpp

   fvgrid::Axis1D
   fvgrid::Uniform1D
   fvgrid::Random1D
   fvgrid::Roberts1D
   fvgrid::StructuredGrid2D
   fvgrid::Length
   fvgrid::XInit

Essa camada e estavel: ``Axis1D`` nao muda de tipo entre instalacoes.

Camada avancada
---------------

Para estudos de sensibilidade ou reproducao numerica, use aliases explicitos:

.. code-block:: cpp

   fvgrid::Axis1DFloat
   fvgrid::Axis1DLongDouble
   fvgrid::Uniform1DFloat
   fvgrid::StructuredGrid2DFloat

ou chame diretamente a familia template:

.. code-block:: cpp

   const auto axis = fvgrid::uniform_axis_1d<float>(100, 0.0f, 1.0f);

Os tipos que armazenam valores escalares exposicionam ``value_type``. Isso
facilita integracao com bibliotecas externas e testes genericos.

Constantes e tolerancias
------------------------

A implementacao template evita depender de constantes ``double`` quando o tipo
``T`` e parte do calculo. Em logica numerica templated, a biblioteca usa
``T{0}``, ``T{1}``, ``T{0.5}`` e tolerancias baseadas em
``std::numeric_limits<T>``.