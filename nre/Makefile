.PHONY: all run build clean format valgrind

all: build

build: main

run: main
	./main entrada.bin


main: main.c
	gcc -W -Wall -std=c11 -O0 -g -pedantic main.c -o main

nar: binary.bin  
	mv ./entrada.bin ./entrada.nar

binary.bin: binary.c
	gcc -W -Wall -std=c11 -O0 -g -pedantic binary.c -o binary
	./binary

clean:
	rm -f main

format:
	clang-format -i -style=file */.c 

valgrind:
	valgrind --leak-check=full ./main ./entrada.nar