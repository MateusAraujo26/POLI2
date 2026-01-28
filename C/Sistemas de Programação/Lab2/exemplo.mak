# Para saber o que a variavel abaixo faz, acesse https://www.gnu.org/software/make/manual/html_node/Choosing-the-Shell.html
SHELL=/bin/bash
# Existem varias outras variaveis especiais que podem ser configuradas em Makefiles https://www.gnu.org/software/make/manual/html_node/Special-Variables.html

# Este Makefile contém 4 regras, descritas abaixo.

# Esta primeira regra contém apenas um target: o arquivo food.txt.
# Para que o make consiga construir o arquivo, outros dois arquivos estão listados como dependências: um com a lista de frutas e outro com a lista de legumes.

# Se as dependências forem satisfeitas, o make executa as linhas de receita, que apenas imprime o conteúdo dos dois arquivos em um arquivo final, acrescentando linhas de cabeçalho.
# Importante: cada linha é executada em uma nova instância nova de um shell (o que significa, por exemplo, que variáveis definidas em uma linha não estarão disponíveis nas linhas seguintes).
# -------------------------------------------------------------------------
food.txt : fruits.txt vegetables.txt
	echo -e "Fruits:n" > food.txt
	cat fruits.txt >> food.txt
	echo -e "nVegetables:n" >> food.txt
	cat vegetables.txt >> food.txt

# Esta regra contém dois targets. Nenhum deles tem dependências, e  a receita apenas imprime uma mensagem dizendo para o usuário criar o arquivo correspondente.
# -------------------------------------------------------------------------
fruits.txt vegetables.txt:
	echo "Please create the file $@"

# Esta é uma regra auxiliar. O target `clean` não é um arquivo que será criado; a regra é basicamente um atalho para executar uma linha de comando. Nesse caso, a receita apaga o arquivo food.txt.
# -------------------------------------------------------------------------
clean:
	rm -f food.txt

# Esta é uma outra regra auxiliar, que imprime o conteúdo do arquivo de saída (o que significa que faz sentido listá-lo como dependência).
# -------------------------------------------------------------------------
print : food.txt
	cat food.txt                                     