#ifndef RISCV_H
#define RISCV_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "instructions.h"
#define NREG 32
#define MEMSIZE 0x8000
#define DATASTART 0x2000
#define VERBOSE 1
#define DUMP 0

typedef struct riscv {
  uint8_t mem[MEMSIZE];
  int32_t breg[NREG];
  uint32_t pc;
  uint32_t ri;
  uint32_t gp;
  uint32_t sp;
} riscv;

void riscv_init(riscv *r);

void riscv_init_text_data(riscv *r, char *text, size_t text_size, char *data, size_t data_size);

void riscv_exit(riscv *r, int exit_code);

void dump_mem(riscv *r, uint32_t addr, uint32_t wsize);

void dump_reg(riscv *r, int decimal);

void fetch(riscv *r);

struct ifields decode(riscv *r);

void cycle(riscv *r);

// sign extension function
// b: input number of bits
int32_t sext(uint32_t input, uint8_t b);

void execute(riscv *r, struct ifields i);

void iltype(riscv *r, struct ifields i);

void btype(riscv *r, struct ifields i);

void storetype(riscv *r, struct ifields i);

void ilatype(riscv *r, struct ifields i);

void regtype(riscv *r, struct ifields i);

void add(riscv *r, uint8_t rd, uint8_t rs1, uint8_t rs2);

void addi(riscv *r, uint8_t rd, uint8_t rs1, int32_t imm12_i);

void and(riscv *r, uint8_t rd, uint8_t rs1, uint8_t rs2);

void andi(riscv *r, uint8_t rd, uint8_t rs1, int32_t imm12_i);

void auipc(riscv *r, uint8_t rd, int32_t imm20_u);

void beq(riscv *r, uint8_t rs1, uint8_t rs2, int32_t offset);

void bge(riscv *r, uint8_t rs1, uint8_t rs2, int32_t offset);

void bgeu(riscv *r, uint8_t rs1, uint8_t rs2, int32_t offset);

void blt(riscv *r, uint8_t rs1, uint8_t rs2, int32_t offset);

void bltu(riscv *r, uint8_t rs1, uint8_t rs2, int32_t offset);

void bne(riscv *r, uint8_t rs1, uint8_t rs2, int32_t offset);

void ecall(riscv *r);

void jal(riscv *r, uint8_t rd, int32_t offset);

void jalr(riscv *r, uint8_t rd, uint8_t rs1, int32_t imm12_i);

void lui(riscv *r, uint8_t rd, int32_t imm20_u);

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

void slt(riscv *r, uint8_t rd, uint8_t rs1, uint8_t rs2);

void sltu(riscv *r, uint8_t rd, uint8_t rs1, uint8_t rs2);

void slti(riscv *r, uint8_t rd, uint8_t rs1, int32_t imm12_i);

void sltiu(riscv *r, uint8_t rd, uint8_t rs1, int32_t imm12_i);

// Load instructions
int32_t lw(riscv *r, uint32_t address, int32_t k);

int32_t lh(riscv *r, uint32_t address, int32_t k);

int32_t lhu(riscv *r, uint32_t address, int32_t k);

int32_t lb(riscv *r, uint32_t address, int32_t k);

int32_t lbu(riscv *r, uint32_t address, int32_t k);

// Store instructions
void sw(riscv *r, uint32_t address, int32_t k, int32_t d);

void sh(riscv *r, uint32_t address, int32_t k, int16_t d);

void sb(riscv *r, uint32_t address, int32_t k, int8_t d);

#endif
