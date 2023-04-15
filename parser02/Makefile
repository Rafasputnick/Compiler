CC = gcc
CCFLAGS = -Wall -g -O0
LFLAGS = -lm

.PHONY = all build clean

all: build

build: main

main: main.c tinyexpr.o
	$(CC) $(CCFLAGS) $^ -o $@ $(LFLAGS)

.c.o:
	$(CC) -c $(CCFLAGS) $< -o $@

clean:
	rm -f *.o *.exe main