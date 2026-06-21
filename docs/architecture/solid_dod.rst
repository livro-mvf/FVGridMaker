SOLID e Data-Oriented Design
============================

Resultado da revisão
--------------------

**SRP**
   Geradores 1D criam coordenadas; ``Axis1D`` possui e valida um eixo;
   ``StructuredGrid2D`` compõe geometria; writers cuidam apenas de saída.

**OCP e DIP**
   Patterns e sistemas de coordenadas são requisitos estruturais expressos por
   ``concepts``. Novos traits são adicionados sem alterar a malha e sem base
   virtual.

**LSP e ISP**
   Não há hierarquia pública de grids. As interfaces são pequenos conjuntos de
   operações necessários à construção, evitando contratos monolíticos.

**DOD**
   Grandezas percorridas separadamente ficam em vetores contíguos. Eixos 2D
   continuam 1D; medidas de células e das duas famílias de faces usam arrays
   lineares row-major. ``std::span`` oferece leitura sem cópia.

Decisões deliberadas
--------------------

* ausência de ``std::vector<std::vector<T>>``;
* ausência de propriedade compartilhada e alocação por célula;
* ausência de ``virtual`` no caminho de geração;
* mapeamentos funcionais existem apenas durante a construção;
* ``PhysicalPoint2D`` usa AoS porque seus três componentes são consumidos
  juntos na exportação VTK; medidas escalares usam SoA.

