#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "riscv.h"
#include "instructions.h"

#define BUFSIZE 4096
#define WIDTH 128
#define HEIGHT 128

#define SCREEN_START 0x3000

#define TICK 1000

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

int setup_screen(SDL_Window *window, SDL_Renderer *renderer) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not be initialized. SDL_Error: %s\n", SDL_GetError());
    return 0;
  }

  SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);


  if (window == NULL) {
    printf("Window could not be created. SDL_Error: %s\n", SDL_GetError());
    return 0;
  }

  if (renderer == NULL) {
    printf("Window could not be created. SDL_Error: %s\n", SDL_GetError());
    return 0;
  }

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  return 1;
}

void draw_screen(uint8_t *buffer, SDL_Renderer *renderer) {
  for (int i = 0; i < WIDTH*HEIGHT; i++) {
    int x = i % HEIGHT;
    int y = i / WIDTH;
    uint8_t pixel = buffer[SCREEN_START+i];
    /*
      Bit    7  6  5  4  3  2  1  0
      Data   R  R  R  G  G  G  B  B
    */
    int r = (0xff * (pixel >> 5)) / 0x7;
    int g = (0xff * ((pixel >> 2) & 0x7)) / 0x7;
    int b = (0xff * (pixel & 0x3)) / 0x3;
    // printf("0x%02x (%d, %d, %d)\n", pixel, r, g, b);
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderDrawPoint(renderer, x, y);
  }
  SDL_RenderPresent(renderer);
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

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not be initialized. SDL_Error: %s\n", SDL_GetError());
    return 0;
  }

  SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);


  if (window == NULL) {
    printf("Window could not be created. SDL_Error: %s\n", SDL_GetError());
    return 0;
  }

  if (renderer == NULL) {
    printf("Window could not be created. SDL_Error: %s\n", SDL_GetError());
    return 0;
  }

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  // if (!setup_screen(window, renderer)) {
  //   return 0;
  // }

  int counter = 0;
  while (cycle(&r)) {
    if (counter % TICK == 0) {
      draw_screen(r.mem, renderer);
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
