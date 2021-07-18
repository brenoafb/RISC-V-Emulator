#include "riscv.h"

void riscv_init(riscv *r) {
  if (!r) return;

  r->mem = malloc(MEMSIZE);
  if (!r->mem) {
    printf("Error allocating memory\n");
  }
  r->pc = 0;
  r->ri = 0;
  r->sp = 0x3ffc;
  r->gp = 0x1800;
}

void riscv_init_text_data(riscv *r, char *text, size_t text_size, char *data, size_t data_size) {
  if (!r) return;
  riscv_init(r);
  memcpy(r->mem, text, text_size);
  memcpy(r->mem + DATASTART, data, data_size);
}

void riscv_deinit(riscv *r) {
  free(r->mem);
}

void riscv_exit(riscv *r, int exit_code) {
  if (DUMP) {
    dump_reg(r, 0);
    printf("\n");
    dump_mem(r, 0, MEMSIZE);
  }
  exit(exit_code);
}

void dump_mem(riscv *r, uint32_t addr, uint32_t wsize) {
  for (uint32_t i = 0; i < wsize; i++) {
    printf("0x%08x\t0x%08x\n", (addr+i) << 2, r->mem[addr+i]);
  }
}

void dump_reg(riscv *r, int decimal) {
  for (int i = 0; i < 32; i++) {
    if (decimal) {
      printf("x%02d\t%d", i, r->reg[i]);
    } else {
      printf("x%02d\t0x%08x", i, r->reg[i]);
    }
    if (i % 4 == 3) printf("\n");
    else            printf(" | ");
  }
}

void fetch(riscv *r) {
  if (!r) return;
  r->ri = ((uint32_t *) r->mem)[r->pc >> 2];
  r->pc += 4;
}

ifields decode(riscv *r) {
  return decode_instruction(r->ri);
}

int cycle(riscv *r) {
  if (VERBOSE) dump_reg(r, 0);
  if (VERBOSE) printf("pc\t0x%08x\n", r->pc);
  if (VERBOSE) printf("ir\t0x%08x\n", r->ri);
  fetch(r);
  if (r->ri == 0) {
    return 0;
  }
  ifields f = decode(r);
  execute(r, f);
  r->reg[0] = 0;
  if (VERBOSE) getc(stdin);
  return 1;
}

int32_t sext(uint32_t input, uint8_t b) {
  if (input & 0x8000000) return input;
  int32_t m = 1U << (b - 1);
  return (input ^ m) - m;
}


void add(riscv *r, uint8_t rd, uint8_t rs1, uint8_t rs2) {
  r->reg[rd] = r->reg[rs1] + r->reg[rs2];
  if (VERBOSE) printf("add x%d, x%d, x%d\n", rd, rs1, rs2);
}

void addi(riscv *r, uint8_t rd, uint8_t rs1, int32_t imm12_i) {
  // r->reg[rd] = r->reg[rs1] + sext(imm12_i, 12);
  r->reg[rd] = r->reg[rs1] + imm12_i;
  if (VERBOSE) printf("addi x%d, x%d, 0x%x\n", rd, rs1, imm12_i);
}

void mul(riscv *r, uint8_t rd, uint8_t rs1, uint8_t rs2) {
  r->reg[rd] = r->reg[rs1] * r->reg[rs2];
  if (VERBOSE) printf("add x%d, x%d, x%d\n", rd, rs1, rs2);
}

void and(riscv *r, uint8_t rd, uint8_t rs1, uint8_t rs2) {
  r->reg[rd] = r->reg[rs1] & r->reg[rs2];
  if (VERBOSE) printf("and x%d, x%d, x%d\n", rd, rs1, rs2);
}

void andi(riscv *r, uint8_t rd, uint8_t rs1, int32_t imm12_i) {
  r->reg[rd] = r->reg[rs1] & sext(imm12_i, 12);
  if (VERBOSE) printf("andi x%d, x%d, 0x%x\n", rd, rs1, imm12_i);
}

void auipc(riscv *r, uint8_t rd, int32_t imm20_u) {
  // need to add -4 since pc was incremented by 4 in fetch
  r->reg[rd] = (r->pc - 4) + (sext(imm20_u, 20) << 12);
  if (VERBOSE) printf("auipc x%d, 0x%x\n", rd, imm20_u);
}

void beq(riscv *r, uint8_t rs1, uint8_t rs2, int32_t offset) {
  if (r->reg[rs1] == r->reg[rs2])
    r->pc += sext(offset, 21) - 4;
  if (VERBOSE) printf("beq x%d, x%d, 0x%x\n", rs1, rs2, offset);
}

void bge(riscv *r, uint8_t rs1, uint8_t rs2, int32_t offset) {
  if (r->reg[rs1] > r->reg[rs2])
    r->pc += sext(offset, 21) - 4;
  if (VERBOSE) printf("bge x%d, x%d, 0x%x\n", rs1, rs2, offset);
}

void bgeu(riscv *r, uint8_t rs1, uint8_t rs2, int32_t offset) {
  if ((uint32_t) r->reg[rs1] > (uint32_t) r->reg[rs2])
    r->pc += sext(offset, 21) - 4;
  if (VERBOSE) printf("bgeu x%d, x%d, 0x%x\n", rs1, rs2, offset);
}

void blt(riscv *r, uint8_t rs1, uint8_t rs2, int32_t offset) {
  if (r->reg[rs1] < r->reg[rs2])
    r->pc += sext(offset, 21) - 4;
  if (VERBOSE) printf("blt x%d, x%d, 0x%x\n", rs1, rs2, offset);
}

void bltu(riscv *r, uint8_t rs1, uint8_t rs2, int32_t offset) {
  if ((uint32_t) r->reg[rs1] < (uint32_t) r->reg[rs2])
    r->pc += sext(offset, 21) - 4;
  if (VERBOSE) printf("bltu x%d, x%d, 0x%x\n", rs1, rs2, offset);
}

void bne(riscv *r, uint8_t rs1, uint8_t rs2, int32_t offset) {
  if (r->reg[rs1] != r->reg[rs2])
    r->pc += sext(offset, 21) - 4;
  if (VERBOSE) printf("bne x%d, x%d, 0x%x\n", rs1, rs2, offset);
}

void ecall(riscv *r) {
  // syscall is indicated by a7 = x17
  switch (r->reg[17]) {
  case 1:
    // print int at a0 = x10
    printf("%d", r->reg[10]);
    break;
  case 4:
    // print string at a0 = x10
    printf("%s", (char *) &(r->mem[r->reg[10]])); // address is in bytes
    break;
  case 5:
    // read int
    scanf("%d", &r->reg[10]);
    break;
  case 10:
    // exit
    printf("exit syscall\n");
    riscv_exit(r, 0);
    break;
  default:
    printf("ECALL error (%d)\n", r->reg[7]);
    break;
  }
  if (VERBOSE) printf("ecall\n");
}

void jal(riscv *r, uint8_t rd, int32_t offset) {
  r->reg[rd] = r->pc;
  r->pc += sext(offset, 21) - 4;
  if (VERBOSE) printf("jal x%d, 0x%x\n", rd, offset);
}

void jalr(riscv *r, uint8_t rd, uint8_t rs1, int32_t imm12_i) {
  uint32_t t = r->pc;
  int32_t offset = sext(imm12_i, 12);
  r->pc = (r->reg[rs1] + offset) & ~1;
  r->reg[rd] = t;
  if (VERBOSE) printf("jalr x%d, x%d\n", rd, rs1);
}

void lui(riscv *r, uint8_t rd, int32_t imm20_u) {
  r->reg[rd] = sext(imm20_u, 20) << 12;
  // r->reg[rd] = imm20_u << 12;
  if (VERBOSE) printf("lui x%d, 0x%x\n", rd, imm20_u);
}

void or(riscv *r, uint8_t rd, uint8_t rs1, uint8_t rs2) {
  r->reg[rd] = r->reg[rs1] | r->reg[rs2];
  if (VERBOSE) printf("or x%d, x%d, x%d\n", rd, rs1, rs2);
}

void ori(riscv *r, uint8_t rd, uint8_t rs1, int32_t imm12_i) {
  r->reg[rd] = r->reg[rs1] | imm12_i;
  if (VERBOSE) printf("ori x%d, x%d, 0x%x\n", rd, rs1, imm12_i);
}

void sll(riscv *r, uint8_t rd, uint8_t rs1, uint8_t rs2) {
  r->reg[rd] = r->reg[rs1] << r->reg[rs2];
  if (VERBOSE) printf("sll x%d, x%d, x%d\n", rd, rs1, rs2);
}

void slli(riscv *r, uint8_t rd, uint8_t rs1, uint8_t shamt) {
  r->reg[rd] = r->reg[rs1] << shamt;
  if (VERBOSE) printf("slli x%d, x%d, 0x%x\n", rd, rs1, shamt);
}

void sra(riscv *r, uint8_t rd, uint8_t rs1, uint8_t rs2) {
  r->reg[rd] = r->reg[rs1] >> r->reg[rs2];
  if (VERBOSE) printf("sra x%d, x%d, x%d\n", rd, rs1, rs2);
}

void srai(riscv *r, uint8_t rd, uint8_t rs1, uint8_t shamt) {
  // TODO: Does this work?
  r->reg[rd] = r->reg[rs1] >> (int32_t) shamt;
  if (VERBOSE) printf("srai x%d, x%d, 0x%x\n", rd, rs1, shamt);
}

void srl(riscv *r, uint8_t rd, uint8_t rs1, uint8_t rs2) {
  r->reg[rd] = r->reg[rs1] >> ((uint32_t) r->reg[rs2]);
  if (VERBOSE) printf("srl x%d, x%d, x%d\n", rd, rs1, rs2);
}

void srli(riscv *r, uint8_t rd, uint8_t rs1, uint8_t shamt) {
  r->reg[rd] = r->reg[rs1] >> shamt;
  if (VERBOSE) printf("srli x%d, x%d, 0x%x\n", rd, rs1, shamt);
}

void sub(riscv *r, uint8_t rd, uint8_t rs1, uint8_t rs2) {
  r->reg[rd] = r->reg[rs1] - r->reg[rs2];
  if (VERBOSE) printf("sub x%d, x%d, x%d\n", rd, rs1, rs2);
}

void xor(riscv *r, uint8_t rd, uint8_t rs1, uint8_t rs2) {
  r->reg[rd] = r->reg[rs1] ^ r->reg[rs2];
  if (VERBOSE) printf("xor x%d, x%d, x%d\n", rd, rs1, rs2);
}

void xori(riscv *r, uint8_t rd, uint8_t rs1, int32_t imm12_i) {
  r->reg[rd] = r->reg[rs1] ^ sext(imm12_i, 12);
  if (VERBOSE) printf("xori x%d, x%d, 0x%x\n", rd, rs1, imm12_i);
}

void slt(riscv *r, uint8_t rd, uint8_t rs1, uint8_t rs2) {
  r->reg[rd] = r->reg[rs1] < r->reg[rs2];
  if (VERBOSE) printf("slt x%d, x%d, x%d\n", rd, rs1, rs2);
}

void sltu(riscv *r, uint8_t rd, uint8_t rs1, uint8_t rs2) {
  r->reg[rd] = (uint32_t) r->reg[rs1] < (uint32_t) r->reg[rs2];
  if (VERBOSE) printf("sltu x%d, x%d, x%d\n", rd, rs1, rs2);
}

void slti(riscv *r, uint8_t rd, uint8_t rs1, int32_t imm12_i) {
  r->reg[rd] = r->reg[rs1] < sext(imm12_i, 12);
  if (VERBOSE) printf("slti x%d, x%d, 0x%x\n", rd, rs1, imm12_i);
}

void sltiu(riscv *r, uint8_t rd, uint8_t rs1, int32_t imm12_i) {
  r->reg[rd] = (uint32_t) r->reg[rs1] < (uint32_t) sext(imm12_i, 12);
  if (VERBOSE) printf("sltiu x%d, x%d, 0x%x\n", rd, rs1, imm12_i);
}

int32_t lw(riscv *r, uint32_t address, int32_t k) {
  int32_t *arr = (int32_t *) r->mem;
  size_t addr = (address + k) >> 2;
  return arr[addr];
}

int32_t lh(riscv *r, uint32_t address, int32_t k) {
  int16_t *arr = (int16_t *) r->mem;
  size_t addr = (address + k) >> 1;
  return arr[addr];
}

int32_t lhu(riscv *r, uint32_t address, int32_t k) {
  uint16_t *arr = (uint16_t *) r->mem;
  size_t addr = (address + k) >> 1;
  return arr[addr];
}

int32_t lb(riscv *r, uint32_t address, int32_t k) {
  int8_t *arr = (int8_t *) r->mem;
  size_t addr = (address + k);
  return arr[addr];
}

int32_t lbu(riscv *r, uint32_t address, int32_t k) {
  uint8_t *arr = (uint8_t *) r->mem;
  size_t addr = (address + k);
  return arr[addr];
}

void sw(riscv *r, uint32_t address, int32_t k, int32_t d) {
  int32_t *arr = (int32_t *) r->mem;
  size_t addr = (address + k) >> 2;
  arr[addr] = d;
}

void sh(riscv *r, uint32_t address, int32_t k, int16_t d) {
  int16_t *arr = (int16_t *) r->mem;
  size_t addr = (address + k) >> 1;
  arr[addr] = d;
}

void sb(riscv *r, uint32_t address, int32_t k, int8_t d) {
  int8_t *arr = (int8_t *) r->mem;
  size_t addr = (address + k);
  arr[addr] = d;
}

void iltype(riscv *r, struct ifields i) {
  // loads
  switch (i.f3) {
  case LB3:
    if (VERBOSE) printf("lb x%d, 0x%x(x%d)\n", i.rd, i.imm12_i, i.rs1);
    r->reg[i.rd] = lb(r, r->reg[i.rs1], sext(i.imm12_i, 12));
    break;
  case LH3:
    if (VERBOSE) printf("lh x%d, 0x%x(x%d)\n", i.rd, i.imm12_i, i.rs1);
    r->reg[i.rd] = lh(r, r->reg[i.rs1], sext(i.imm12_i, 12));
    break;
  case LW3:
    if (VERBOSE) printf("lw x%d, 0x%x(x%d)\n", i.rd, i.imm12_i, i.rs1);
    r->reg[i.rd] = lw(r, r->reg[i.rs1], sext(i.imm12_i, 12));
    break;
  case LBU3:
    if (VERBOSE) printf("lbu x%d, 0x%x(x%d)\n", i.rd, i.imm12_i, i.rs1);
    r->reg[i.rd] = lbu(r, r->reg[i.rs1], sext(i.imm12_i, 12));
    break;
  case LHU3:
    if (VERBOSE) printf("lhu x%d, 0x%x(x%d)\n", i.rd, i.imm12_i, i.rs1);
    r->reg[i.rd] = lhu(r, r->reg[i.rs1], sext(i.imm12_i, 12));
    break;
  default:
    printf("Unknown ILType f3: 0x%x\n", i.f3);
    break;
  }
}

void btype(riscv *r, struct ifields i) {
  switch (i.f3) {
  case BEQ3:
    beq(r, i.rs1, i.rs2, i.imm13);
    break;
  case BNE3:
    bne(r, i.rs1, i.rs2, i.imm13);
    break;
  case BLT3:
    blt(r, i.rs1, i.rs2, i.imm13);
    break;
  case BGE3:
    bge(r, i.rs1, i.rs2, i.imm13);
    break;
  case BLTU3:
    bltu(r, i.rs1, i.rs2, i.imm13);
    break;
  case BGEU3:
    bgeu(r, i.rs1, i.rs2, i.imm13);
    break;
  default:
    printf("Unknown BType f3: 0x%x\n", i.f3);
  }
}

void storetype(riscv *r, struct ifields i) {
  switch (i.f3) {
  case SB3:
    if (VERBOSE) printf("sb x%d, 0x%x(x%d)\n", i.rs1, i.imm12_i, i.rs2);
    sb(r, r->reg[i.rs1], sext(i.imm12_s, 12), r->reg[i.rs2]);
    break;
  case SH3:
    if (VERBOSE) printf("sh x%d, 0x%x(x%d)\n", i.rs1, i.imm12_i, i.rs2);
    sh(r, r->reg[i.rs1], sext(i.imm12_s, 12), r->reg[i.rs2]);
    break;
  case SW3:
    if (VERBOSE) printf("sw x%d, 0x%x(x%d)\n", i.rs1, i.imm12_i, i.rs2);
    sw(r, r->reg[i.rs1], sext(i.imm12_s, 12), r->reg[i.rs2]);
    break;
  default:
    printf("Unknown StoreType f3: 0x%x\n", i.f3);
    break;
  }
}

void ilatype(riscv *r, struct ifields i) {
  switch (i.f3) {
  case ADDI3:
    addi(r, i.rd, i.rs1, i.imm12_i);
    break;
  case ORI3:
    ori(r, i.rd, i.rs1, i.imm12_i);
    break;
  case SLTI3:
    slti(r, i.rd, i.rs1, i.imm12_i);
    break;
  case XORI3:
    xori(r, i.rd, i.rs1, i.imm12_i);
    break;
  case ANDI3:
    andi(r, i.rd, i.rs1, i.imm12_i);
    break;
  case SLTIU:
    sltiu(r, i.rd, i.rs1, i.imm12_i);
    break;
  case SLLI3:
    slli(r, i.rd, i.rs1, i.imm12_i);
    break;
  case SRI3:
    switch (i.f7) {
    case SRLI7:
      srli(r, i.rd, i.rs1, i.imm12_i);
      break;
    case SRAI7:
      srai(r, i.rd, i.rs1, i.imm12_i);
      break;
    default:
      printf("Unknown ILAType f3, f7: 0x%x, 0x%x\n", i.f3, i.f7);
      break;
    }
    break;
  default:
    printf("Unknown ILAType f3: 0x%x\n", i.f3);
    break;
  }
}

void regtype(riscv *r, struct ifields i) {
  switch (i.f3) {
  case ADDSUBMUL3:
    switch (i.f7) {
      case ADD7:
        add(r, i.rd, i.rs1, i.rs2);
        break;
      case SUB7:
        sub(r, i.rd, i.rs1, i.rs2);
        break;
      case MULDIV7:
        mul(r, i.rd, i.rs1, i.rs2);
        break;
      default:
        printf("(0x%08x) Unknown RegType f3, f7: 0x%x, 0x%x\n", r->pc-4, i.f3, i.f7);
        break;
    }
    break;

  case SLL3:
    sll(r, i.rd, i.rs1, i.rs2);
    break;

  case SLT3:
    slt(r, i.rd, i.rs1, i.rs2);
    break;

  case STLU3:
    sltu(r, i.rd, i.rs1, i.rs2);
    break;

  case XOR3:
    xor(r, i.rd, i.rs1, i.rs2);
    break;

  case SR3:
    switch (i.f7) {
      case SRA7:
      srl(r, i.rd, i.rs1, i.rs2);
      break;
      case SRL7:
      srl(r, i.rd, i.rs1, i.rs2);
      break;
      default:
      printf("(0x%08x) Unknown RegType f3, f7: 0x%x, 0x%x\n", r->pc-4, i.f3, i.f7);
      break;
    }
    break;

  case OR3:
    or(r, i.rd, i.rs1, i.rs2);
    break;

  case AND3:
    and(r, i.rd, i.rs1, i.rs2);
    break;
  }
}

void execute(riscv *r, ifields i) {
  switch(i.op) {
  case LUI:
    lui(r, i.rd, i.imm20_u);
    break;
  case AUIPC:
    auipc(r, i.rd, i.imm20_u);
    break;
  case ILType:
    iltype(r, i);
    break;
  case BType:
    btype(r, i);
    break;
  case JAL:
    jal(r, i.rd, i.imm21);
    break;
  case JALR:
    jalr(r, i.rd, i.rs1, i.imm12_i);
    break;
  case StoreType:
    storetype(r, i);
    break;
  case ILAType:
    ilatype(r, i);
    break;
  case RegType:
    regtype(r, i);
    break;
  case ECALL:
    ecall(r);
    break;
  default:
    printf("Unknown opcode: 0x%x\n", i.op);
    break;
  }
}
