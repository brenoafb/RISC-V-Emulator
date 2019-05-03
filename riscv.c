#include "riscv.h"

void riscv_init(riscv *r) {
  if (!r) return;

  r->pc = 0;
  r->ri = 0;
  r->sp = 0x3ffc;
  r->gp = 0x1800;
}

void riscv_init_data(riscv *r, int32_t *data) {
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

int32_t sext(uint32_t input, uint8_t b) {
  int32_t m = 1U << (b - 1);  
  return (input ^ m) - m;
}


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

void auipc(riscv *r, uint8_t rd, int32_t imm20_u) {
  r->breg[rd] = r->breg[rd] + sext(imm20_u, 20);
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

void lui(riscv *r, uint8_t rd, int32_t imm20_u) {
  r->breg[rd] = sext(imm20_u, 20) << 12;
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

void slti(riscv *r, uint8_t rd, uint8_t rs1, int32_t imm12_i) {
  r->breg[rd] = r->breg[rs1] < sext(imm12_i, 12);
}

void sltiu(riscv *r, uint8_t rd, uint8_t rs1, int32_t imm12_i) {
  r->breg[rd] = (uint32_t) r->breg[rs1] < (uint32_t) sext(imm12_i, 12);
}

int32_t lw(riscv *r, uint32_t address, int32_t k) {
  return r->mem[(address + k) >> 2];
}

int32_t lh(riscv *r, uint32_t address, int32_t k) {
  int16_t *arr = (int16_t *) r->mem;
  return arr[(address+k) >> 1];
}

int32_t lhu(riscv *r, uint32_t address, int32_t k) {
  uint16_t *arr = (uint16_t *) r->mem;
  return arr[(address+k) >> 1];
}

int32_t lb(riscv *r, uint32_t address, int32_t k) {
  int8_t *arr = (int8_t *) r->mem;
  return arr[address+k];
}

int32_t lbu(riscv *r, uint32_t address, int32_t k) {
  uint8_t *arr = (uint8_t *) r->mem;
  return arr[address+k];
}

void sw(riscv *r, uint32_t address, int32_t k, int32_t d) {
  r->mem[(address+k) >> 2] = d;
}

void sh(riscv *r, uint32_t address, int32_t k, int16_t d) {
  int16_t *arr = (int16_t *) r->mem;
  arr[(address+k) >> 1] = d;
}

void sb(riscv *r, uint32_t address, int32_t k, int8_t d) {
  int8_t *arr = (int8_t *) r->mem;
  arr[address+k] = d;
}

void execute(riscv *r, ifields i) {
  uint64_t s = signature(i);

  switch (s) {
  case S_ADD:
    add(r, i.rd, i.rs1, i.rs2);
    break;
  case S_SUB:
    sub(r, i.rd, i.rs1, i.rs2);
    break;
  case S_XOR:
    xor(r, i.rd, i.rs1, i.rs2);
    break;
  case S_OR:
    or(r, i.rd, i.rs1, i.rs2);
    break;
  case S_SLL:
    sll(r, i.rd, i.rs1, i.rs2);
    break;
  case S_SRL:
    srl(r, i.rd, i.rs1, i.rs2);
    break;
  case S_SRA:
    srl(r, i.rd, i.rs1, i.rs2);
    break;

  case S_ADDI: 
    addi(r, i.rd, i.rs1, i.imm12_i);
    break;
  case S_SLTI:  
    slti(r, i.rd, i.rs1, i.imm12_i);
    break;
  case S_SLTIU:  
    sltiu(r, i.rd, i.rs1, i.imm12_i);
    break;
  case S_XORI:   
    xori(r, i.rd, i.rs1, i.imm12_i);
    break;
  case S_ORI:    
    ori(r, i.rd, i.rs1, i.imm12_i);
    break;
  case S_ANDI:   
    andi(r, i.rd, i.rs1, i.imm12_i);
    break;
  case S_SLLI:   
    slli(r, i.rd, i.rs1, i.imm12_i);
    break;
  case S_SRLI:   
    srli(r, i.rd, i.rs1, i.imm12_i);
    break;
  case S_SRAI:   
    srai(r, i.rd, i.rs1, i.imm12_i);
    break;

  case S_SB:
    sb(r, r->breg[i.rs1], 0, r->breg[i.rs2]);
    break;
  case S_SH:
    sh(r, r->breg[i.rs1], 0, r->breg[i.rs2]);
    break;
  case S_SW:
    sw(r, r->breg[i.rs1], 0, r->breg[i.rs2]);
    break;

  case S_LB:
    r->breg[i.rd] = lb(r, r->breg[i.rs1], sext(i.imm12_i, 12));
    break;
  case S_LH:
    r->breg[i.rd] = lh(r, r->breg[i.rs1], sext(i.imm12_i, 12));
    break;
  case S_LW:
    r->breg[i.rd] = lw(r, r->breg[i.rs1], sext(i.imm12_i, 12));
    break;
  case S_LBU:
    r->breg[i.rd] = lbu(r, r->breg[i.rs1], sext(i.imm12_i, 12));
    break;
  case S_LHU:
    r->breg[i.rd] = lhu(r, r->breg[i.rs1], sext(i.imm12_i, 12));
    break;
    
  case S_BEQ:
    beq(r, i.rs1, i.rs2, i.imm13);
    break;

  case S_BNE: 
    bne(r, i.rs1, i.rs2, i.imm13);
    break;

  case S_BLT: 
    blt(r, i.rs1, i.rs2, i.imm13);
    break;

  case S_BGE:
    bge(r, i.rs1, i.rs2, i.imm13);
    break;

  case S_BLTU:
    bltu(r, i.rs1, i.rs2, i.imm13);
    break;

  case S_BGEU:
    bltu(r, i.rs1, i.rs2, i.imm13);
    break;

  case S_LUI:
    lui(r, i.rd, i.imm20_u);
    break;
    
  case S_AUIPC:
    auipc(r, i.rd, i.imm20_u);
    break;
    
  case S_JAL:
    jal(r, i.rd, i.imm13);
    break;
    
  case S_JALR:
    jalr(r, i.rd, i.rs1);
    break;
  }

}
