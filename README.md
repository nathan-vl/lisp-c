# Lisp-C

Uma linguagem da família Lisp feita em C.

## Instruções de Compilação

É necessário ter um compilador GCC instalado que suporte a versão do C11, além disso é necessário Makefile.

Para compilar no modo debug, basta executar `make debug` no diretório raiz que o executável estará disponível em `build/debug`.

Para compilar no modo otimizado, basta executar `make` no diretório raiz que o executável estará disponível em `build/app`.

## Documentação
Os procedimentos primitivos estão listados em `include/std.h`, e abaixo estão suas definições mais detalhadas:

- cons: Recebe 2 expressões e retorna um par. Para fazer uma lista utilizando cons basta o segundo elemento do par ser um cons recursivamente, até que o último seja uma lista vazia.
- define: Recebe um identificador e uma expressão, e define uma variável com o identificador informado.
- defmacro: Recebe um identificador, uma lista de parâmetros e uma definição e define um macro com os valores informados.
- if: Recebe uma expressão de condição, uma expressão a ser executada se a condição for verdadeira, e uma expressão a ser executada caso contrário
- include: Recebe uma string com um caminho para um arquivo a ser lido e importado.
- lambda: Recebe uma lista de parâmetros e uma definição e retorna uma função anônima.
- macroexpand: recebe uma lista a ser executada com um macro e retorna o macro expandido sem ser executado.
- print: Recebe uma expressão e mostra-a na tela.
- progn: Recebe uma lista de expressões, executa-as e retorna a última executada.
- quote: Recebe uma expressão e retorna-a sem executá-la.
- set!: Recebe um identificador e uma expressão, e define uma variável com o identificador informado.
- while: Recebe uma condição e uma expressão. A condição é executada e cada sub-expressão dentro da expressão é executada até que a condição seja falsa.
- Os procedimentos `<`, `>`, `<=` e `>=` recebem dois números e retornam verdadeiro ou falso de acordo com sua função matemática.
- =: Recebe duas expressões e informa se são iguais. Exceto listas que nesse caso sempre é retornado falso.
- not: Recebe uma expressão e retorna seu valor booleano negado. Apenas o `#f` conta como falso.
- Os procedimentos `and` e `or` recebem N expressões e realizam suas respectivas operações booleanas, contando com short-circuiting.
- Os procedimentos `+`, `-`, `*` e `\` recebem N números e retornam a sua operação entre eles. No caso da subtração, se apenas um número for informado, é retornada a negação deste número.
- pow: Recebe dois números e retorna a exponenciação.
- %: Recebe dois números e retorna o resto da resto da divisão entre eles.
