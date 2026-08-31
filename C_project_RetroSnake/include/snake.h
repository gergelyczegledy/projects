#ifndef SNAKE_H_INCLUDED
#define SNAKE_H_INCLUDED

#include <SDL2/SDL.h>
#include "../include/structs.h"
#include <stdbool.h>

void move_snake(SNAKE_PART* kigyo,  int block_width);
void snake_direction(SNAKE_PART* kigyo,SDL_Event event,int player);
void grow_snake(SNAKE_PART *snake, int block_width);
bool snake_collision(SNAKE_PART *snake1, SNAKE_PART *snake2, int useable_width, int offset, int *winner);
void free_snake_memory(SNAKE_PART *snake);
void snake_init(SNAKE_PART **snake1, SNAKE_PART **snake2, int block_width, int offset, int snake_num);

#endif // SNAKE_H_INCLUDED
