#include <stdio.h>
#include <stdint.h>
#define NREG 32
#define MEMSIZE 4096

typedef struct riscv {
  int32_t mem[MEMSIZE];
  int32_t breg[NREG];
  int32_t pc;
  int32_t ri;
  int32_t gp;
  int32_t sp;
} riscv;
