#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "riscv.h"
#include "instructions.h"
#include "screen.h"

#define BUFSIZE 4096
#define WIDTH 128
#define HEIGHT 128

#define SCREEN_START 0x5000

#define TICK 10

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

  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  SDL_Event event;

  if (!setup_screen(&window, &renderer, WIDTH, HEIGHT)) {
    return 0;
  }

  int counter = 0;
  while (cycle(&r)) {
    if (counter % TICK == 0) {
      draw_screen(r.mem + SCREEN_START, renderer, WIDTH, HEIGHT);
    }
    if (SDL_PollEvent(&event) && event.type == SDL_QUIT) {
      break;
    }
    counter++;
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  riscv_deinit(&r);

  return 0;
}
