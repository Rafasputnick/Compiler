.PHONY: all run build clean format valgrind
.ONESHELL :
path = src

all: build

run: src/main.out
    cd $(path)
    ./main.out

build: src/main.out

src/main.out: src/main.c
    cd $(path)
    gcc -W -Wall -std=c11 -g -pedantic main.c -o main.out

clean:
    rm -f src/main.out

format:
    clang-format -i -style=file */.c 

valgrind:
    valgrind --leak-check=full ./src/main.out