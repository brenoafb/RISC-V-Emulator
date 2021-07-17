#include <stdio.h>
#include <stdlib.h>
#include "riscv.h"
#include "instructions.h"

#define BUFSIZE 4096

size_t read_file(char *filename, char arr[], size_t size) {
  FILE *file = fopen(filename, "rb");
  if (!file) {
    printf("Error reading data file \'%s\'\n", filename);
    return 0;
  }

  fseek(file, 0, SEEK_END);
  long fsize = ftell(file);
  fseek(file, 0, SEEK_SET);

  if (fsize > size) {
    printf("Warning: file is larger than provided buffer)\n");
  }

  size = size < fsize ? size : fsize;

  return fread(arr, sizeof(char), size, file);
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Usage: %s [text] [data]\n", argv[0]);
    return 0;
  }

  char *text_file = argv[1];
  char *data_file = argv[2];

  char text[BUFSIZE];
  char data[BUFSIZE];


  size_t text_size = read_file(text_file, text, MEMSIZE);
  size_t data_size = read_file(data_file, data, MEMSIZE);

  riscv r;
  riscv_init_text_data(&r, text, text_size, data, data_size);

  while (cycle(&r));

  return 0;
}
