#ifndef MEMORY_H
#define MEMORY_H

#include "riscv.h"

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
