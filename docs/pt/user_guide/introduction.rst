
Motivação
==========

Bibliotecas numéricas em geral concentram-se nos algoritmos de solução,
mas deixam em segundo plano a etapa anterior: a geração e a organização
das malhas usadas nas simulações. A **FVGridMaker** nasce justamente para
preencher esse espaço, oferecendo uma infraestrutura moderna em C++20
para construção de malhas estruturadas voltadas ao método de volumes
finitos.

A ideia central é separar, de forma clara, três camadas:

- **Geometria da malha**: volumes, faces, áreas e distâncias.
- **Topologia de vizinhança**: quem é vizinho de quem, iteradores de
  borda e de interior.
- **Integração com o código do usuário**: permite-se o acesso somente à
  geometria e topologia, o restante do código (solvers,
  montadores, etc.) serão implementados de forma independente.

Escopo da biblioteca
--------------------

A **FVGridMaker** tem como propósito:

- Malhas estruturadas em 1D e 2D (com 3D no horizonte de desenvolvimento).
- Sistemas de coordenadas:

  - cartesiano,
  - cilíndrico,
  - esférico,
  - possibilidades de implementação de novos sistemas via traits.  

- Armazenamento de informações geométricas clássicas:

  - volumes de controle,
  - áreas de faces,
  - distâncias entre centros,
  - coordenadas de centros de célula e de face.

- Iteradores especializados para:

  - volumes internas,
  - bordas esquerda, direita, inferior e superior.

A biblioteca está sendo escrita em C++20 (ou superior), utilizando apenas a
biblioteca padrão do C++. Sempre que possível, recursos de paralelismo
(`std::execution`) são empregados na geração da malha e no cálculo de
quantidades geométricas.

Público-alvo
------------

Este manual foi pensado para pessoas que:

- já conhecem o método de volumes finitos (ao menos em nível básico);
- desejam separar a geração de malha da implementação do solver;
- preferem uma interface C++ simples de usar, mesmo que o código
  interno seja mais sofisticado.

Não é necessário ser especialista em C++, mas é recomendável alguma
familiaridade com:

- uso básico de CMake,
- organização de projetos em múltiplos arquivos,
- conceitos elementares de orientação a objetos e templates.

Filosofia de design
-------------------

A **FVGridMaker** é desenvolvida com um conjunto claro de princípios de
engenharia de software:

- **Princípios SOLID e código limpo**  
  O código foi estruturado desde o início de acordo com os princípios do
  SOLID e com boas práticas de código limpo, privilegiando componentes
  pequenos, coesos e com responsabilidades bem delimitadas.

- **Data-Oriented Design (DOD) e layout SoA**  
  Sempre que possível, a organização dos dados segue uma abordagem
  orientada a dados (DOD), com ênfase em *Structure of Arrays*  (SoA):
  cada grandeza geométrica é armazenada em vetores independentes e
  contíguos em memória, favorecendo localidade de cache, vetorização e
  uso eficiente de algoritmos paralelos.

- **Evitar herança e polimorfismo dinâmico**  
  A biblioteca evita hierarquias de herança profundas e polimorfismo via
  métodos `virtual`. Em vez disso, prefere composição, templates, traits,
  policies e tipos leves para desacoplar comportamentos e permitir
  extensibilidade sem custo em tempo de execução.

- **Sem dependências externas na biblioteca**  
  O código de produção utiliza exclusivamente a biblioteca padrão do C++.
  A única dependência externa é o Google Test (`gtest`), usado apenas na
  suíte de testes e não exposto para quem utiliza a biblioteca.

- **Forte separação entre manual e API**  
  O manual do usuário (este documento) explica conceitos e fluxos de uso,
  com exemplos comentados. A referência de API, gerada automaticamente,
  lista classes e funções com detalhes de assinatura.

- **Simplicidade na interface pública**  
  A interface para quem usa a biblioteca deve ser um conjunto pequeno e
  previsível de funções e tipos:

  - funções de fábrica de malhas (uniformes, não uniformes, etc.),
  - tipos leves que expõem apenas o necessário (centros, volumes,
    vizinhos, iteradores).

- **Uso intensivo da biblioteca padrão e paralelização**  
  A biblioteca utiliza contêineres e algoritmos da STL, incluindo
  `std::execution`, `std::unique_ptr` e outros recursos modernos, evitando
  reimplementações desnecessárias. Cálculos que podem ser paralelizados
  de forma segura e determinística fazem uso de políticas de execução
  paralela da STL sempre que vantajoso.


Organização deste manual
------------------------

Este manual está organizado em blocos temáticos para facilitar a navegação:

1. :doc:`Getting Started <pt/install>`  
   Apresenta uma visão rápida de como:

   - compilar e vincular a **FVGridMaker** em um projeto simples,
   - criar a primeira malha 1D,
   - inspecionar volumes e centros de célula.

2. Conceitos básicos de malha (capítulos futuros)  
   
   Explicações mais detalhadas sobre:
   
   - convenções de índices,
   - definição de volumes de controle,
   - iteração sobre interior e bordas.

3. Tópicos avançados (capítulos futuros)  
   
   Integração com:
   
   - solvers externos (por exemplo, PETSc),
   - exportação de dados para formatos de visualização,
   - estratégias de paralelização.

4. Referência de API  
   Gerada automaticamente a partir do código, com a listagem completa
   dos tipos, funções e namespaces públicos.

A leitura recomendada é começar pelo *Getting Started* e, em seguida,
pular para os exemplos que mais se aproximam do tipo de problema que
você deseja resolver.
