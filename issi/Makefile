
compile:
	bison -d --verbose -t iakab.y
	./stringify.sh
	flex iakab.l
	gcc lex.yy.c iakab.tab.c tokens.tab.c issi.c main.c -ll -o issi
