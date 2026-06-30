Eixos 1D
========

``Axis1D`` é o proprietário dos dados geométricos de uma direção. Ele armazena
faces, centros, distâncias face--face e distâncias centro--centro em vetores
contíguos expostos como ``std::span<const T>``.

Convenções geométricas
----------------------

Para ``N`` volumes existem:

* ``N + 1`` faces;
* ``N`` centros;
* ``N`` valores em ``dx_faces()``;
* ``N + 1`` valores em ``dx_centers()``.

A biblioteca valida que cada centro fica dentro da sua própria célula:

.. math::

   x_{f,p} < x_{c,p} < x_{f,p+1}.

Essa validação evita uma classe comum de erros em estudos numéricos: centros
globalmente crescentes, mas geometricamente fora da célula local.

Geradores
---------

.. list-table::
   :header-rows: 1

   * - Gerador
     - Uso principal
     - Padrão sem pattern explícito
   * - ``Uniform1D``
     - espaçamento uniforme
     - ``FaceCentered1D``
   * - ``Random1D``
     - partição pseudoaleatória reprodutível
     - ``FaceCentered1D``
   * - ``Roberts1D``
     - concentração suave junto aos extremos
     - ``FaceCentered1D``
   * - ``Custom1D``
     - coordenadas fornecidas pelo usuário
     - pattern informado pelo usuário

Exemplo iniciante:

.. code-block:: cpp

   const auto axis = fvgrid::uniform_axis_1d(100, 0.0, 1.0);

Exemplo com tipos fortes:

.. code-block:: cpp

   const auto axis = fvgrid::Uniform1D::make(
       fvgrid::NVol{100},
       fvgrid::Length{1.0},
       fvgrid::XInit{0.0});

Patterns
--------

``FaceCentered1D`` interpreta centros como coordenadas primárias e reconstrói
faces usando o domínio. ``VolumeCentered1D`` interpreta faces como coordenadas
primárias e reconstrói centros.

Para pedir volume centrado explicitamente:

.. code-block:: cpp

   const auto axis = fvgrid::Uniform1D::make(
       fvgrid::NVol{100},
       fvgrid::Length{1.0},
       fvgrid::XInit{0.0},
       fvgrid::VolumeCentered1D{});

A construção direta ``Axis1D{{0.0, 0.5, 1.0}}`` também é volume centrado,
porque o vetor passado ali é explicitamente um vetor de faces.

Tipos escalares
---------------

A API sem sufixo usa ``double``. A API avançada preserva o mesmo contrato com
outro tipo escalar:

.. code-block:: cpp

   const auto axis_f = fvgrid::uniform_axis_1d<float>(50, 0.0f, 1.0f);
   const auto axis_ld = fvgrid::uniform_axis_1d<long double>(50, 0.0L, 1.0L);

Todos os objetos escalares relevantes expõem ``value_type``.

Operações
---------

``Operations1D`` oferece interseção, teste de compatibilidade de patterns e
recorte de eixos. O recorte preserva ``VolumeCentered1D`` ou ``FaceCentered1D``
quando ambos os dados de entrada permitem uma reconstrução válida.

Qualidade
---------

``Quality1D`` resume comprimentos mínimos, máximos, razões globais e razões
entre células adjacentes. Esses relatórios são úteis para justificar a malha em
um texto acadêmico antes de apresentar resultados numéricos.
