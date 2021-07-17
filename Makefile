CC=gcc
CFLAGS=-g -Wall -Wextra
DEGS=instructions.h riscv.h
OBJ=instructions.o riscv.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main.out: $(OBJ)
	$(CC) -o $@ main.c $^ $(CFLAGS)

all: main

.PHONY: clean

clean:
	rm -f *.o *.out
