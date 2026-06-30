Erros e invariantes
===================

Entradas inválidas geram ``FVGridException`` contendo código estável,
categoria, origem e localização. A biblioteca prefere falhar cedo a produzir
uma malha parcialmente válida.

Exemplos de erros validados
---------------------------

* coordenadas não finitas;
* faces ou centros não crescentes;
* centro fora da própria célula;
* número incorreto de faces, centros ou distâncias;
* comprimento não positivo;
* ``NVol`` igual a zero;
* parâmetro ``Beta`` inválido;
* pattern incompatível entre eixos;
* interseção vazia em operação que exige área;
* falha de abertura ou escrita de arquivo.

Formato do diagnóstico
----------------------

Cada exceção carrega um ``ErrorRecord`` com:

* ``code``: identificador estável para testes e relatórios;
* ``category``: grupo do erro;
* ``source``: módulo, classe e ``class_id``;
* ``context``: pares ``chave=valor`` quando disponíveis;
* ``location``: local de origem no código.

Uso acadêmico
-------------

Em exemplos de livro ou artigos internos, prefira registrar o código do erro e
a condição geométrica que o gerou. Isso torna a validação reproduzível mesmo se
a redação da mensagem mudar no futuro.
