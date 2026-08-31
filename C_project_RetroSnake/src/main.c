#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "../include/structs.h"
#include "../include/snake.h"
#include "../include/UI.h"
#include "../include/file_handling.h"
// #include "../include/debugmalloc.h" // IWYU pragma: keep

int main(int argc, char *argv[]){
    Uint32 refresh_rate_ms = 125;
    int window_width = 500;
    int block_num = 21;
    double useable_area = 0.8;
    
    int useable_width = (int)(window_width * useable_area);
    int block_width = useable_width / block_num;
    useable_width = block_width * block_num;
    int offset = (window_width - (useable_width)) / 2;

    SDL_Window *window;
    SDL_Renderer *renderer;
    sdl_init("Snake", window_width, window_width, &window, &renderer);

    int score = 0;
    int winner = 0; // 0: draw, 1: player 1 wins, 2: player 2 wins
    int user_input = draw_menu(renderer, window, &window_width, block_num, useable_area, &useable_width, &block_width, &offset);
    
    if (user_input == 1 || user_input == 2){
        srand(time(0));
        SNAKE_PART *snake1 = NULL;
        SNAKE_PART *snake2 = NULL;
        snake_init(&snake1, &snake2, block_width, offset, user_input);
        int apple_x = (rand() % block_num) * block_width + offset;
        int apple_y = (rand() % block_num) * block_width + offset;
        APPLE apple = {apple_x, apple_y, block_width / 2 - 2};

        bool quit = false;
        SDL_TimerID id = SDL_AddTimer(refresh_rate_ms, event_generator, NULL);
        SDL_Event event;
        while (!quit) {
            SDL_WaitEvent(&event);
            switch (event.type) {
                case SDL_USEREVENT:
                    move_snake(snake1, block_width);
                    if (snake1->x == apple.x && snake1->y == apple.y) {
                        apple.x = (rand() % block_num) * block_width + offset;
                        apple.y = (rand() % block_num) * block_width + offset;
                        grow_snake(snake1, block_width);
                        if (user_input == 1)
                            score += 1;
                    }
                    if (user_input == 2){
                        move_snake(snake2, block_width);
                        if (snake2->x == apple.x && snake2->y == apple.y) {
                            apple.x = (rand() % block_num) * block_width + offset;
                            apple.y = (rand() % block_num) * block_width + offset;
                            grow_snake(snake2, block_width);
                        }
                    }
                    if (snake_collision(snake1, snake2, useable_width, offset, &winner))
                        quit = true;

                    draw_a_game_frame(renderer, window_width, useable_width, offset, block_width, user_input, snake1, snake2, apple, score);
                    break;
                case SDL_KEYDOWN:
                    snake_direction(snake1, event, 1);
                    snake_direction(snake2, event, 2);
                    break;
                case SDL_QUIT:
                    quit = true;
                    break;
            }
        }

        free_snake_memory(snake1);
        free_snake_memory(snake2);

        int top10[10] = {0};
        save_score(score, top10);

        if (user_input == 1)
            draw_scoreboard(renderer, window, score, &window_width, block_num, useable_area, &useable_width, &block_width, &offset, top10);
        else
            draw_winner(renderer, window_width, winner);
        SDL_RemoveTimer(id);
        SDL_Quit();
    }
    return 0;
}