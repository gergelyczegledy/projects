#ifndef FONT_BITMAP_H_INCLUDED
#define FONT_BITMAP_H_INCLUDED

#include <SDL2/SDL_render.h>

void draw_text(SDL_Renderer *renderer, int center_x, int start_y, int scale, const char text[]);

#endif // FONT_BITMAP_H_INCLUDED