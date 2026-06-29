Arquitetura escalar
===================

A biblioteca usa uma convenção dupla: nomes simples para a API iniciante e
classes ``Basic*<T>`` para controle explícito do tipo escalar. Essa escolha
preserva clareza em sala de aula e permite experimentos com ``float`` ou
``long double`` quando o estudo numérico exigir.

Camada iniciante
----------------

Os nomes públicos sem sufixo significam ``double``:

.. code-block:: cpp

   fvgrid::Axis1D
   fvgrid::Uniform1D
   fvgrid::Random1D
   fvgrid::Roberts1D
   fvgrid::StructuredGrid2D
   fvgrid::Length
   fvgrid::XInit

Essa camada é estável: ``Axis1D`` não muda de tipo entre instalações.

Camada avançada
---------------

Para estudos de sensibilidade ou reprodução numérica, use aliases explícitos:

.. code-block:: cpp

   fvgrid::Axis1DFloat
   fvgrid::Axis1DLongDouble
   fvgrid::Uniform1DFloat
   fvgrid::StructuredGrid2DFloat

ou chame diretamente a família template:

.. code-block:: cpp

   const auto axis = fvgrid::uniform_axis_1d<float>(100, 0.0f, 1.0f);

Os tipos que armazenam valores escalares expõem ``value_type``. Isso facilita a
integração com bibliotecas externas e testes genéricos.

Constantes e tolerâncias
------------------------

A implementação template evita depender de constantes ``double`` quando o tipo
``T`` é parte do cálculo. Em lógica numérica templated, a biblioteca usa
``T{0}``, ``T{1}``, ``T{0.5}`` e tolerâncias baseadas em
``std::numeric_limits<T>``.
