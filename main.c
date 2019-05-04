#include <stdio.h>
#include <stdlib.h>
#include "riscv.h"
#include "instructions.h"

#define BUFSIZE 128

void print_riscv(riscv *r) {
  for (int i = 0; i < 32; i++) {
    printf("x%02d\t0x%08x", i, r->breg[i]);
    if (i % 4 == 3) printf("\n");
    else            printf(" | ");
  }
  printf("pc\tx%08x\n", r->pc);
}

void read_file(char *filename, int32_t arr[], size_t size) {
  FILE *file = fopen(filename, "rb");
  if (!file) {
    printf("Error reading data file \'%s\'\n", filename);
    return;
  }

  fread(arr, sizeof(int32_t), size, file);
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Usage: %s [text] [data]\n", argv[0]);
    return 0;
  }

  char *text_file = argv[1];
  char *data_file = argv[2];

  int32_t data[MEMSIZE];
  int32_t text[MEMSIZE];
  
  read_file(data_file, data, MEMSIZE);
  read_file(text_file, text, MEMSIZE);
  
  riscv r;
  riscv_init_data(&r, data);
  
  char buffer[BUFSIZE];
  for (int i = 0; i < MEMSIZE; i++) {
    if (VERBOSE) print_riscv(&r);
    fetch(&r, text);
    ifields f = decode(&r);
    execute(&r, f);
    r.breg[0] = 0;
    if (VERBOSE) scanf("%s", buffer);
  }

  return 0;
}
