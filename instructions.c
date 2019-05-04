#include "instructions.h"

OPCODE get_opcode(int32_t instruction) {
  // mask first 6 bits of instruction
  return (instruction & F0M) >> F0S;
}

FUNCT3 get_funct3(int32_t instruction) {
  // mask bits 12 through 14 (inclusive)
  return (instruction & F2M) >> F2S;
}

FUNCT7 get_funct7(int32_t instruction) {
  // mask bits 25 through 31 (inclusive)
  return (instruction & F5M) >> F5S;
}

uint8_t get_rs1(int32_t instruction) {
  // mask bits 15 through 19 (inclusive)
  return (instruction & F3M) >> F3S;
}

uint8_t get_rs2(int32_t instruction) {
  // mask bits 20 through 24 (inclusive)
  return (instruction & F4M) >> F4S;
}

uint8_t get_rd(int32_t instruction) {
  // mask bits 7 through 11 (inclusive)
  return (instruction & F1M) >> F1S;
}

uint8_t get_shamt(int32_t instruction) {
  // mask bits 20 through 24 (inclusive)
  return (instruction & F4M) >> F4S;
}

int32_t get_imm12_i(int32_t instruction) {
  int32_t up = (instruction & F5M) >> F4S;
  int32_t lo = (instruction & F4M) >> F4S;
  return up + lo;
}

int32_t get_imm12_s(int32_t instruction) {
  int32_t up = (instruction & F5M) >> F4S;
  int32_t lo = (instruction & F1M) >> F1S;
  return up + lo;
}

int32_t get_imm13(int32_t instruction) {
  int32_t bit11 = (instruction & (1 << 7)) >> 7; 
  int32_t bit12 = (instruction & (1 << 31)) >> 31;
  int32_t lo = (instruction & (0xf << 8)) >> 8;    
  int32_t up = (instruction & (0x3f << 25)) >> 25;

  return (bit12 << 12) + (bit11 << 11) + (up << 5) + (lo << 1);
}

int32_t get_imm20_u(int32_t instruction) {
  return (instruction & (F2M + F3M + F4M)) >> F2S;
}

int32_t get_imm21(int32_t instruction) {
  uint32_t bits1_10 = (uint32_t)  (instruction & 0x7fe00000) >> (uint32_t) 21;
  uint32_t bits12_19 = (uint32_t) (instruction & 0x000ff000) >> (uint32_t) 12;
  uint32_t bit11 = (uint32_t)     (instruction & 0x00100000) >> (uint32_t) 20;
  uint32_t bit20 = (uint32_t)     (instruction & 0x80000000) >> (uint32_t) 31;

  return (bits1_10 << 1)
       + (bit11 << 11)
       + (bits12_19 << 12)
       + (bit20 << 20);
}

ifields decode_instruction(int32_t instruction) {
  ifields i;

  i.op = get_opcode(instruction);
  i.f3 = get_funct3(instruction);
  i.f7 = get_funct7(instruction);

  i.rd = get_rd(instruction);
  i.rs1 = get_rs1(instruction);
  i.rs2 = get_rs2(instruction);

  i.shamt = get_shamt(instruction);
  i.imm12_i = get_imm12_i(instruction);
  i.imm12_s = get_imm12_s(instruction);
  i.imm13 = get_imm13(instruction);
  i.imm20_u = get_imm20_u(instruction);
  i.imm21 = get_imm21(instruction);
  
  return i;
}

uint64_t signature(ifields i) {
  return i.op
    + (i.f3 << 7)
    + (i.f7 << 10);
}
