#ifndef RISCV_H

#define RISCV_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "instructions.h"
#define NREG 32
#define MEMSIZE 4096

typedef struct riscv {
  int32_t mem[MEMSIZE];
  int32_t breg[NREG];
  uint32_t pc;
  uint32_t ri;
  uint32_t gp;
  uint32_t sp;
} riscv;

void riscv_init(riscv *r);

void riscv_init_data(riscv *r, int32_t *data);

void fetch(riscv *r, int32_t *text);

struct ifields decode(riscv *r);

// sign extension function
// b: input number of bits
int32_t sext(uint32_t input, uint8_t b);

void execute(riscv *r, struct ifields i);

void add(riscv *r, uint8_t rd, uint8_t rs1, uint8_t rs2);

void addi(riscv *r, uint8_t rd, uint8_t rs1, int32_t imm12_i);

void and(riscv *r, uint8_t rd, uint8_t rs1, uint8_t rs2);

void andi(riscv *r, uint8_t rd, uint8_t rs1, int32_t imm12_i);

void auipc(riscv *r, uint8_t rd, int32_t imm20_i);

void beq(riscv *r, uint8_t rs1, uint8_t rs2, int32_t offset);

void bge(riscv *r, uint8_t rs1, uint8_t rs2, int32_t offset);

void bgeu(riscv *r, uint8_t rs1, uint8_t rs2, int32_t offset);

void blt(riscv *r, uint8_t rs1, uint8_t rs2, int32_t offset);

void bltu(riscv *r, uint8_t rs1, uint8_t rs2, int32_t offset);

void bne(riscv *r, uint8_t rs1, uint8_t rs2, int32_t offset);

void ecall(riscv *r);

void jal(riscv *r, uint8_t rd, int32_t offset);

void jalr(riscv *r, uint8_t rd, uint8_t rs1);

void lui(riscv *r, uint8_t rd, int32_t imm20_i);

void or(riscv *r, uint8_t rd, uint8_t rs1, uint8_t rs2);

void ori(riscv *r, uint8_t rd, uint8_t rs1, int32_t imm12_i);

void sll(riscv *r, uint8_t rd, uint8_t rs1, uint8_t rs2);

void slli(riscv *r, uint8_t rd, uint8_t rs1, uint8_t shamt);

void sra(riscv *r, uint8_t rd, uint8_t rs1, uint8_t rs2);

void srai(riscv *r, uint8_t rd, uint8_t rs1, uint8_t shamt);

void srl(riscv *r, uint8_t rd, uint8_t rs1, uint8_t rs2);

void srli(riscv *r, uint8_t rd, uint8_t rs1, uint8_t shamt);

void sub(riscv *r, uint8_t rd, uint8_t rs1, uint8_t rs2);

void xor(riscv *r, uint8_t rd, uint8_t rs1, uint8_t rs2);

void xori(riscv *r, uint8_t rd, uint8_t rs1, int32_t imm12_i);

#endif
