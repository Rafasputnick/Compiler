.PHONY: all run build clean format valgrind

all: build

build: main

run: main
	./nre entrada.nar


main: main.c
	gcc -W -Wall -std=c11 -O0 -g -pedantic main.c -o nre

nar:
	gcc -W -Wall -std=c11 -O0 -g -pedantic binary.c -o binary
	./binary
	mv ./entrada.bin ./entrada.nar

clean:
	rm -f main

format:
	clang-format -i -style=file */.c 

valgrind:
	valgrind --leak-check=full ./nre ./entrada.nar