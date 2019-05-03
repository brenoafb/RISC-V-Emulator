#include "riscv.h"

void riscv_init(riscv *r) {
  if (!r) return;

  r->pc = 0;
  r->ri = 0;
  r->sp = 0x3ffc;
  r->gp = 0x1800;
}

void riscv_init_text(riscv *r, int32_t *data) {
  if (!r) return;

  riscv_init(r);
  memcpy(r->mem, data, MEMSIZE);
}

void fetch(riscv *r, int32_t *text) {
  if (!r || !text) return;
  r->ri = text[(r->pc++) << 2];
}

ifields decode(riscv *r) {
  return decode_instruction(r->ri);
}

// b: input number of bits
int32_t sext(uint32_t input, uint8_t b) {
  int32_t m = 1U << (b - 1);  
  return (input ^ m) - m;
}

void execute(riscv *r, ifields i);

void add(riscv *r, uint8_t rd, uint8_t rs1, uint8_t rs2) {
  r->breg[rd] = r->breg[rs1] + r->breg[rs2];
}

void addi(riscv *r, uint8_t rd, uint8_t rs1, int32_t imm12_i) {
  r->breg[rd] = r->breg[rs1] + sext(imm12_i, 12);
}

void and(riscv *r, uint8_t rd, uint8_t rs1, uint8_t rs2) {
  r->breg[rd] = r->breg[rs1] & r->breg[rs2];
}

void andi(riscv *r, uint8_t rd, uint8_t rs1, int32_t imm12_i) {
  r->breg[rd] = r->breg[rs1] & sext(imm12_i, 12);
}

void auipc(riscv *r, uint8_t rd, int32_t imm20_i) {
  r->breg[rd] = r->breg[rd] + sext(imm20_i, 20);
}

void beq(riscv *r, uint8_t rs1, uint8_t rs2, int32_t offset) {
  if (r->breg[rs1] == r->breg[rs2])
    r->pc += sext(offset, 21);
}

void bge(riscv *r, uint8_t rs1, uint8_t rs2, int32_t offset) {
  if (r->breg[rs1] > r->breg[rs2])
    r->pc += sext(offset, 21);
}

void bgeu(riscv *r, uint8_t rs1, uint8_t rs2, int32_t offset) {
  if ((uint32_t) r->breg[rs1] > (uint32_t) r->breg[rs2])
    r->pc += sext(offset, 21);
}

void blt(riscv *r, uint8_t rs1, uint8_t rs2, int32_t offset) {
  if (r->breg[rs1] < r->breg[rs2])
    r->pc += sext(offset, 21);
}

void bltu(riscv *r, uint8_t rs1, uint8_t rs2, int32_t offset) {
  if ((uint32_t) r->breg[rs1] < (uint32_t) r->breg[rs2])
    r->pc += sext(offset, 21);
}

void bne(riscv *r, uint8_t rs1, uint8_t rs2, int32_t offset) {
  if (r->breg[rs1] != r->breg[rs2])
    r->pc += sext(offset, 21);
}

void ecall(riscv *r);

void jal(riscv *r, uint8_t rd, int32_t offset) {
  r->breg[rd] = r->pc;
  r->pc += sext(offset, 21);
}

void jalr(riscv *r, uint8_t rd, uint8_t rs1) {
  uint32_t t = r->pc;
  int32_t offset = r->breg[rs1];
  r->pc = (r->breg[rs1] + offset) & ~1;
  r->breg[rd] = t;
}

void lui(riscv *r, uint8_t rd, int32_t imm20_i) {
  r->breg[rd] = sext(imm20_i, 20) << 12;
}

void or(riscv *r, uint8_t rd, uint8_t rs1, uint8_t rs2) {
  r->breg[rd] = r->breg[rs1] | r->breg[rs2];
}

void ori(riscv *r, uint8_t rd, uint8_t rs1, int32_t imm12_i) {
  r->breg[rd] = r->breg[rs1] | imm12_i;
}

void sll(riscv *r, uint8_t rd, uint8_t rs1, uint8_t rs2) {
  r->breg[rd] = r->breg[rs1] << r->breg[rs2];
}

void slli(riscv *r, uint8_t rd, uint8_t rs1, uint8_t shamt) {
  r->breg[rd] = r->breg[rs1] << shamt;
}

void sra(riscv *r, uint8_t rd, uint8_t rs1, uint8_t rs2) {
  r->breg[rd] = r->breg[rs1] >> r->breg[rs2];
}

void srai(riscv *r, uint8_t rd, uint8_t rs1, uint8_t shamt) {
  // TODO: Does this work?
  r->breg[rd] = r->breg[rs1] >> (int32_t) shamt;
}

void srl(riscv *r, uint8_t rd, uint8_t rs1, uint8_t rs2) {
  r->breg[rd] = r->breg[rs1] >> ((uint32_t) r->breg[rs2]);
}

void srli(riscv *r, uint8_t rd, uint8_t rs1, uint8_t shamt) {
  r->breg[rd] = r->breg[rs1] >> shamt;
}

void sub(riscv *r, uint8_t rd, uint8_t rs1, uint8_t rs2) {
  r->breg[rd] = r->breg[rs1] - r->breg[rs2];
}

void xor(riscv *r, uint8_t rd, uint8_t rs1, uint8_t rs2) {
  r->breg[rd] = r->breg[rs1] ^ r->breg[rs2];
}

void xori(riscv *r, uint8_t rd, uint8_t rs1, int32_t imm12_i) {
  r->breg[rd] = r->breg[rs1] ^ sext(imm12_i, 12);
}
