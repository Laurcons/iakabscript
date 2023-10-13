
debug:
	bison -d -t --verbose iakab.y -Wcounterexamples
	bash ./stringify.sh
	flex iakab.l
	gcc $(shell find . -name '*.c') -ll -Wall -o issi -Wno-incompatible-pointer-types -I. -D VERBOSE -g

release:
	bison -d iakab.y
	bash ./stringify.sh
	flex iakab.l
	gcc $(shell find . -name '*.c') -ll -Wall -o issi -Wno-incompatible-pointer-types -I.
