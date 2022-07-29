
compile:
	bison -d --verbose -t iakab.y
	bash ./stringify.sh
	flex iakab.l
	gcc *.c -ll -o issi
