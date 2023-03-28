.PHONY: all run build clean format valgrind

all: build

build: main

run: main
	./main


main: main.c
	gcc -W -Wall -std=c11 -g -pedantic main.c -o main

clean:
	rm -f src/main

format:
	clang-format -i -style=file */.c 

valgrind:
	valgrind --leak-check=full ./src/main