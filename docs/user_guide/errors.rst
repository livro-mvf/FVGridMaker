Erros e invariantes
===================

Entradas inválidas geram ``FVGridException`` contendo código estável,
categoria e origem. Exemplos: coordenadas não crescentes, número incorreto de
centros, comprimento não positivo, índice fora do intervalo e falha de I/O.

Objetos geométricos são validados na construção. Erros de programação não são
convertidos silenciosamente em malhas parcialmente válidas.

