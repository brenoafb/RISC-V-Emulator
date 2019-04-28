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
  int32_t bit11 = (instruction & 0x80) >> 7;          // bit 7 of instruction
  int32_t bit12 = (instruction & 0x80000000) >> 31;   // bit 31 of instruction
  int32_t lo = (instruction & 0xf00) >> 8;            // bits 8 through 11
  int32_t up = (instruction & 0x7e000000) >> 25;       // bits 25 through 30

  return ((bit12 << 12) + (bit11 << 11) + (up << 5) + lo) << 1;
}

int32_t get_imm20_u(int32_t instruction);

int32_t get_imm21(int32_t instruction);

rtype decode_rtype(int32_t instruction) {
  rtype rt;
  rt.rd = instruction & 0xF80;
  rt.f3 = get_funct3(instruction);
  rt.op = get_opcode(instruction);
  rt.rs1 = get_rs1(instruction);
  rt.rs2 = get_rs2(instruction);
  return rt;
}
