lexer:
	flex lex.l

parser:
	bison -d parse.y
build: lexer parser

parser_counterexamples:
	bison -d parse.y -Wcounterexamples
compile:
	gcc lex.yy.c parse.tab.c -o a.out

all: build compile
	./a.out