
compile:
	bison -d iakab.y
	flex iakab.l
	gcc lex.yy.c iakab.tab.c issi.c main.c -ll -o issi
