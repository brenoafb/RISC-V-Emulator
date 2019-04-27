#include "memory.h"

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
