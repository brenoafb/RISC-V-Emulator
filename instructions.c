#include "instructions.h"

OPCODE get_opcode(int32_t instruction) {
  // mask first 6 bits of instruction
  return instruction & 0x3f;
}

FUNCT3 get_funct3(int32_t instruction) {
  // mask bits 12 through 14 (inclusive)
  return (instruction & 0xe000) >> 12;
}

FUNCT7 get_funct7(int32_t instruction) {
  // mask bits 25 through 31 (inclusive)
  return (instruction & 0xfe000000) >> 25;
}

uint8_t get_rs1(int32_t instruction) {
  // mask bits 15 through 19 (inclusive)
  return (instruction  & 0xf8000) >> 15;
}

uint8_t get_rs2(int32_t instruction) {
  // mask bits 20 through 24 (inclusive)
  return (instruction  & 0x1f00000) >> 20;
}

uint8_t get_rd(int32_t instruction) {
  // mask bits 7 through 11 (inclusive)
  return (instruction  & 0xf80) >> 7;
}

rtype decode_rtype(int32_t instruction) {
  rtype rt;
  rt.rd = instruction & 0xF80;
  rt.f3 = get_funct3(instruction);
  rt.op = get_opcode(instruction);
  rt.rs1 = get_rs1(instruction);
  rt.rs2 = get_rs2(instruction);
  return rt;
}
