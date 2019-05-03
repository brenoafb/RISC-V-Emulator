#include <stdio.h>
#include <stdlib.h>
#include "riscv.h"
#include "instructions.h"

#define LINESZ 1024

void read_file(char *filename) {
  FILE *file = fopen(filename, "rb");
  int32_t addr = 0;
  if (file) {
    int32_t instruction;
    while (fread(&instruction, sizeof(instruction), 1, file)) {
    addr += 4;
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) return 0;

  char *filename = argv[1];

  printf("|ifield| = %lu\n", sizeof(ifields));
  printf("|OPCODE| = %lu\n", sizeof(OPCODE));
  printf("|FUNCT3| = %lu\n", sizeof(FUNCT3));
  printf("|FUNCT7| = %lu\n", sizeof(FUNCT7));
  
  // read_file(filename);
  
  return 0;
}
