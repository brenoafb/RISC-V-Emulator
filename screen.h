#ifndef SCREEN_H_

#define SCREEN_H_

#include <SDL2/SDL.h>

int setup_screen(SDL_Window **window, SDL_Renderer **renderer, int width, int height);

void draw_screen(uint8_t *buffer, SDL_Renderer *renderer, int width, int height);

#endif
