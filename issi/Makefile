
debug:
	bison -d -t --verbose iakab.y -Wcounterexamples
	bash ./stringify.sh
	flex iakab.l
	gcc *.c -ll -Wall -o issi -D VERBOSE -g -Wno-incompatible-pointer-types

release:
	bison -d iakab.y
	bash ./stringify.sh
	flex iakab.l
	gcc *.c -ll -Wall -o issi -Wno-incompatible-pointer-types
