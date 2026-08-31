#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED

#include <SDL2/SDL.h>
#include "../include/structs.h"

void sdl_init(char const *felirat, int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer);
Uint32 event_generator(Uint32 ms, void *param);
int draw_menu(SDL_Renderer *renderer, SDL_Window *window, int *window_width, int block_num, double useable_area, int *useable_width, int *block_width, int *offset);
void draw_a_game_frame(SDL_Renderer *renderer, int window_width, int useable_width, int offset, int block_width, int player_num, SNAKE_PART *snake1, SNAKE_PART *snake2, APPLE apple, int score);
void draw_scoreboard(SDL_Renderer *renderer, SDL_Window *window, int score, int *window_width, int block_num, double usable_area, int *useable_width, int *block_width, int *offset, const int top10[]);
void draw_winner(SDL_Renderer *renderer, int window_width, int winner);

#endif // UI_H_INCLUDED
