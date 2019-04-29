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

int32_t get_imm20_u(int32_t instruction) {
  return (instruction & (F2M + F3M + F4M)) << F2S;
}

int32_t get_imm21(int32_t instruction) {
  int32_t bits1_4= (uint32_t) instruction & 0xe00000;
  int32_t bits5_10 = (uint32_t) instruction & 0x7e000000;
  int32_t bit11 = (uint32_t) instruction & (1 << 20);
  int32_t bits12_19 = (uint32_t) instruction & (F2M + F3M);
  int32_t bit20 = (uint32_t) instruction & (1 << 31);

  return (bits1_4 << 1)
       + (bits5_10 << 5)
       + (bit11 << 11)
       + (bits12_19 << 12)
       + (bit20 << 20);
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

itype decode_itype(int32_t instruction) {
  itype it;
  it.op = get_opcode(instruction);
  it.rd = get_rd(instruction);
  it.f3 = get_funct3(instruction);
  it.rs1 = get_rs1(instruction);
  it.imm12_i = get_imm12_i(instruction);

  return it;
}

isitype decode_isitype(int32_t instruction) {
  isitype isit;
  isit.op = get_opcode(instruction);
  isit.rd = get_rd(instruction);
  isit.f3 = get_funct3(instruction);
  isit.rs1 = get_rs1(instruction);
  isit.shamt = get_shamt(instruction);
  isit.f7 = get_funct7(instruction);
  
  return isit;
}

stype decode_stype(int32_t instruction) {
  stype st;
  st.op = get_opcode(instruction);
  st.f3 = get_funct3(instruction);
  st.rs1 = get_rs1(instruction);
  st.rs2 = get_rs2(instruction);
  st.imm12_s = get_imm12_s(instruction);

  return st;
}

sbtype decode_sbtype(int32_t instruction) {
  sbtype sbt;
  sbt.op = get_opcode(instruction);
  sbt.f3 = get_funct3(instruction);
  sbt.rs1 = get_rs1(instruction);
  sbt.rs2 = get_rs2(instruction);
  sbt.imm13 = get_imm13(instruction);
  
  return sbt;
}

ujtype decode_ujtype(int32_t instruction) {
  ujtype ujt;
  ujt.op = get_opcode(instruction);
  ujt.rd = get_rd(instruction);
  ujt.imm21 = get_imm21(instruction);

  return ujt;
}

utype decode_utype(int32_t instruction) {
  utype ut;
  ut.op = get_opcode(instruction);
  ut.rd = get_rd(instruction);
  ut.imm20_u = get_imm20_u(instruction);

  return ut;

}
