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

void execute(riscv *r, ifields i);
