CC=gcc
CFLAGS=-g -Wall -Wextra -lSDL2
DEGS=instructions.h riscv.h
OBJ=instructions.o riscv.o

all: main.out

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main.out: main.c $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o *.out
