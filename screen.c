#include "screen.h"

int setup_screen(SDL_Window **window, SDL_Renderer **renderer, int width, int height) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not be initialized. SDL_Error: %s\n", SDL_GetError());
    return 0;
  }

  SDL_CreateWindowAndRenderer(width, height, 0, window, renderer);


  if (window == NULL) {
    printf("Window could not be created. SDL_Error: %s\n", SDL_GetError());
    return 0;
  }

  if (renderer == NULL) {
    printf("Window could not be created. SDL_Error: %s\n", SDL_GetError());
    return 0;
  }

  SDL_RenderClear(*renderer);

  return 1;
}

void draw_screen(uint8_t *buffer, SDL_Renderer *renderer, int width, int height) {
  for (int i = 0; i < width*height; i++) {
    int x = i % height;
    int y = i / width;
    uint8_t pixel = buffer[i];
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
