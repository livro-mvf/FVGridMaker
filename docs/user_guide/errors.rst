Erros e invariantes
===================

Entradas invalidas geram ``FVGridException`` contendo codigo estavel,
categoria, origem e localizacao. A biblioteca prefere falhar cedo a produzir
uma malha parcialmente valida.

Exemplos de erros validados
---------------------------

* coordenadas nao finitas;
* faces ou centros nao crescentes;
* centro fora da propria celula;
* numero incorreto de faces, centros ou distancias;
* comprimento nao positivo;
* ``NVol`` igual a zero;
* parametro ``Beta`` invalido;
* pattern incompatível entre eixos;
* intersecao vazia em operacao que exige area;
* falha de abertura ou escrita de arquivo.

Formato do diagnostico
----------------------

Cada excecao carrega um ``ErrorRecord`` com:

* ``code``: identificador estavel para testes e relatorios;
* ``category``: grupo do erro;
* ``source``: modulo, classe e ``class_id``;
* ``context``: pares ``chave=valor`` quando disponiveis;
* ``location``: local de origem no codigo.

Uso academico
-------------

Em exemplos de livro ou artigos internos, prefira registrar o codigo do erro e
a condicao geometrica que o gerou. Isso torna a validacao reproduzivel mesmo se
a redacao da mensagem mudar no futuro.