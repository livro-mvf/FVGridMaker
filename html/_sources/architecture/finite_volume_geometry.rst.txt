Geometria de volumes finitos
============================

FVGridMaker descreve a geometria necessária a métodos de volumes finitos sem
introduzir campos, solvers ou condições de contorno. O objeto produzido pela
biblioteca responde à pergunta: quais são as faces, centros, distâncias e
medidas que uma discretização precisará consumir?

Eixo 1D
-------

Para ``N`` volumes finitos em uma direção existem ``N + 1`` faces e ``N``
centros. A biblioteca mantém também dois conjuntos de distâncias:

.. math::

   \Delta x_{f,p} = x_{f,p+1} - x_{f,p}

.. math::

   \Delta x_{c,i} = x_{c,i} - x_{c,i-1},

com os valores extremos de ``dx_centers`` representando as distâncias entre o
contorno e o primeiro ou último centro. Essa convenção é útil para esquemas de
fluxo, gradientes e pós-processamento, mas não impõe uma discretização
específica.

Patterns de reconstrução
------------------------

Um pattern declara qual coordenada é primária. ``VolumeCentered1D`` recebe
faces e reconstrói centros; ``FaceCentered1D`` recebe centros e reconstrói
faces a partir do domínio. Os geradores de malha usam ``FaceCentered1D`` como
padrão porque a formulação de volumes finitos normalmente avalia variáveis nos
centros das células.

Malha 2D
--------

``StructuredGrid2D`` é a composição tensorial de dois eixos 1D. A malha não
armazena ``x[i][j]`` e ``y[i][j]`` como matrizes independentes: ela mantém os
eixos, calcula pontos físicos quando o sistema de coordenadas é aplicado e
armazena medidas em vetores contíguos.

A distinção central é:

* ``cell_logical_area(i, j)``: área computacional no espaço das coordenadas
  independentes;
* ``cell_measure(i, j)``: medida física fornecida pelo sistema de coordenadas.

Em coordenadas cartesianas esses valores coincidem. Em coordenadas polares,
cilíndricas ou esféricas axisimétricas, eles representam grandezas diferentes.
