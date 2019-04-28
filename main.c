#include <stdio.h>
#include <stdlib.h>
#include "riscv.h"
#include "instructions.h"

int main(int argc, char *argv[]) {
  if (argc < 2) return 0;

  for (int i = 1; i < argc; i++) {
    int32_t instruction = strtol(argv[i], 0, 16);
    printf("%x\n", instruction);
    printf("OPCODE: 0x%06x\n", get_opcode(instruction));
    printf("RD: 0x%05hhx\n", get_rd(instruction));
    printf("FUNCT3: 0x%03x\n", get_funct3(instruction));
    printf("RS1: 0x%05hhx\n", get_rs1(instruction));
    printf("RS2: 0x%05hhx\n", get_rs2(instruction));
    // printf("FUNCT7: 0x%07x\n", get_funct7(instruction));
  }
  
  return 0;
}
