Eixos 1D
========

``Axis1D`` e o proprietario dos dados geometricos de uma direcao. Ele armazena
faces, centros, distancias face--face e distancias centro--centro em vetores
contiguos expostos como ``std::span<const T>``.

Convencoes geometricas
----------------------

Para ``N`` volumes existem:

* ``N + 1`` faces;
* ``N`` centros;
* ``N`` valores em ``dx_faces()``;
* ``N + 1`` valores em ``dx_centers()``.

A biblioteca valida que cada centro fica dentro da sua propria celula:

.. math::

   x_{f,p} < x_{c,p} < x_{f,p+1}.

Essa validacao evita uma classe comum de erros em estudos numericos: centros
globalmente crescentes, mas geometricamente fora da celula local.

Geradores
---------

.. list-table::
   :header-rows: 1

   * - Gerador
     - Uso principal
     - Padrao sem pattern explicito
   * - ``Uniform1D``
     - espacamento uniforme
     - ``FaceCentered1D``
   * - ``Random1D``
     - particao pseudoaleatoria reprodutivel
     - ``FaceCentered1D``
   * - ``Roberts1D``
     - concentracao suave junto aos extremos
     - ``FaceCentered1D``
   * - ``Custom1D``
     - coordenadas fornecidas pelo usuario
     - pattern informado pelo usuario

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

``FaceCentered1D`` interpreta centros como coordenadas primarias e reconstrui
faces usando o dominio. ``VolumeCentered1D`` interpreta faces como coordenadas
primarias e reconstrui centros.

Para pedir volume centrada explicitamente:

.. code-block:: cpp

   const auto axis = fvgrid::Uniform1D::make(
       fvgrid::NVol{100},
       fvgrid::Length{1.0},
       fvgrid::XInit{0.0},
       fvgrid::VolumeCentered1D{});

A construcao direta ``Axis1D{{0.0, 0.5, 1.0}}`` tambem e volume centrada,
porque o vetor passado ali e explicitamente um vetor de faces.

Tipos escalares
---------------

A API sem sufixo usa ``double``. A API avancada preserva o mesmo contrato com
outro tipo escalar:

.. code-block:: cpp

   const auto axis_f = fvgrid::uniform_axis_1d<float>(50, 0.0f, 1.0f);
   const auto axis_ld = fvgrid::uniform_axis_1d<long double>(50, 0.0L, 1.0L);

Todos os objetos escalares relevantes expõem ``value_type``.

Operacoes
---------

``Operations1D`` oferece intersecao, teste de compatibilidade de patterns e
recorte de eixos. O recorte preserva ``VolumeCentered1D`` ou ``FaceCentered1D``
quando ambos os dados de entrada permitem uma reconstrucao valida.

Qualidade
---------

``Quality1D`` resume comprimentos minimos, maximos, razoes globais e razoes
entre celulas adjacentes. Esses relatorios sao uteis para justificar a malha em
um texto academico antes de apresentar resultados numericos.