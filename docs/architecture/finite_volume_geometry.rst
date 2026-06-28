Geometria de volumes finitos
============================

FVGridMaker descreve a geometria necessaria a metodos de volumes finitos sem
introduzir campos, solvers ou condicoes de contorno. O objeto produzido pela
biblioteca responde a pergunta: quais sao as faces, centros, distancias e
medidas que uma discretizacao precisara consumir?

Eixo 1D
-------

Para ``N`` volumes finitos em uma direcao existem ``N + 1`` faces e ``N``
centros. A biblioteca mantem tambem dois conjuntos de distancias:

.. math::

   \Delta x_{f,p} = x_{f,p+1} - x_{f,p}

.. math::

   \Delta x_{c,i} = x_{c,i} - x_{c,i-1},

com os valores extremos de ``dx_centers`` representando as distancias entre o
contorno e o primeiro ou ultimo centro. Essa convencao e util para esquemas de
fluxo, gradientes e pos-processamento, mas nao impoe uma discretizacao
especifica.

Patterns de reconstrucao
------------------------

Um pattern declara qual coordenada e primaria. ``VolumeCentered1D`` recebe
faces e reconstrui centros; ``FaceCentered1D`` recebe centros e reconstrui
faces a partir do dominio. Os geradores de malha usam ``FaceCentered1D`` como
padrao porque a formulacao de volumes finitos normalmente avalia variaveis nos
centros das celulas.

Malha 2D
--------

``StructuredGrid2D`` e a composicao tensorial de dois eixos 1D. A malha nao
armazena ``x[i][j]`` e ``y[i][j]`` como matrizes independentes: ela mantem os
eixos, calcula pontos fisicos quando o sistema de coordenadas e aplicado e
armazena medidas em vetores contiguos.

A distincao central e:

* ``cell_logical_area(i, j)``: area computacional no espaco das coordenadas
  independentes;
* ``cell_measure(i, j)``: medida fisica fornecida pelo sistema de coordenadas.

Em coordenadas cartesianas esses valores coincidem. Em coordenadas polares,
cilindricas ou esfericas axisimetricas, eles representam grandezas diferentes.