#include "../include/UI.h"

#include <SDL2/SDL2_gfxPrimitives.h>
#include "../include/font_bitmap.h"
#include <stdbool.h>

// #include "../include/debugmalloc.h" // IWYU pragma: keep

void sdl_init(char const *felirat, int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("SDL cannot initialize: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow(felirat, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szeles, magas, SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        SDL_Log("SDL cannot create window: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("SDL cannot create renderer: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(renderer);

    *pwindow = window;
    *prenderer = renderer;
}

Uint32 event_generator(Uint32 ms, void *param) {
    SDL_Event ev;
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    return ms;
}

static void rescale(SDL_Window *window, int *window_width, int block_num, double useable_area, int *useable_width, int *block_width, int *offset){
    *useable_width = (int)((*window_width) * useable_area);
    *block_width = *useable_width / block_num;
    *useable_width = (*block_width) * block_num;
    *offset = (*window_width - (*useable_width)) / 2;
    int min_window_width = 300;
    int max_window_width = 900;
    if (*window_width < min_window_width) {
        *window_width = min_window_width;
    } else if (*window_width > max_window_width) {
        *window_width = max_window_width;
    }
    SDL_SetWindowSize(window, *window_width, *window_width);
}

static void draw_border(int x1, int y1, int x2, int y2, int border_thickness, int space, SDL_Renderer *renderer) {
    int width = x2 - x1;
    int height = y2 - y1;
    if (width <= 0 || height <= 0 || border_thickness <= 0) return;
    int step = border_thickness + space;
    int num_x = (width + space) / step;
    int num_y = (height + space) / step;
    if (num_x < 1 || num_y < 1) return;
    int total_w = num_x * border_thickness + (num_x - 1) * space;
    int total_h = num_y * border_thickness + (num_y - 1) * space;

    int start_x = x1 + (width - total_w) / 2;
    int start_y = y1 + (height - total_h) / 2;
    for (int i = 0; i < num_x; i++) {
        int left_side = start_x + i * step;
        boxRGBA(renderer, left_side, start_y, left_side + border_thickness, start_y + border_thickness, 20, 20, 20, 255);
        boxRGBA(renderer, left_side, start_y + total_h - border_thickness, left_side + border_thickness, start_y + total_h, 20, 20, 20, 255);
    }
    for (int j = 1; j < num_y - 1; j++) {
        int top_side = start_y + j * step;
        boxRGBA(renderer, start_x, top_side, start_x + border_thickness, top_side + border_thickness, 20, 20, 20, 255);
        boxRGBA(renderer, start_x + total_w - border_thickness, top_side, start_x + total_w, top_side + border_thickness, 20, 20, 20, 255);
    }
}

int draw_menu(SDL_Renderer *renderer, SDL_Window *window, int *window_width, int block_num, double useable_area, int *useable_width, int *block_width, int *offset) {
    int btn_w = 220;
    int btn_h = 40;
    int btn_distance = 20;

    int center_x = *window_width / 2 - btn_w / 2;
    SDL_Rect btn1 = { center_x, *window_width / 2 - btn_distance - btn_h , btn_w, btn_h };
    SDL_Rect btn2 = { center_x, *window_width / 2 , btn_w, btn_h };
    SDL_Rect btn3 = { center_x, *window_width / 2 + btn_distance + btn_h, btn_w, btn_h };
    SDL_Rect buttons[3] = { btn1, btn2, btn3 };
    const char *labels[3] = { "1. Egyjatekos mod", "2. Ketjatekos mod", "3. Kilepes" };

    SDL_Event event;
    int selected_option = 0;
    while (1) {
        int mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);
        SDL_Point mouse_pos = { mouse_x, mouse_y };
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return 3;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                if (SDL_PointInRect(&mouse_pos, &btn1)) selected_option = 1;
                else if (SDL_PointInRect(&mouse_pos, &btn2)) selected_option = 2;
                else if (SDL_PointInRect(&mouse_pos, &btn3)) selected_option = 3;
            }
            else if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    *window_width = event.window.data1;
                    rescale(window, window_width, block_num, useable_area, useable_width, block_width, offset);
                    center_x = *window_width / 2 - btn_w / 2;
                    btn1.x = center_x;
                    btn1.y = *window_width / 2 - btn_distance - btn_h;
                    btn1.w = btn_w;
                    btn1.h = btn_h;
                    btn2.x = center_x;
                    btn2.y = *window_width / 2;
                    btn2.w = btn_w;
                    btn2.h = btn_h;
                    btn3.x = center_x;
                    btn3.y = *window_width / 2 + btn_distance + btn_h;
                    btn3.w = btn_w;
                    btn3.h = btn_h;
                    buttons[0] = btn1;
                    buttons[1] = btn2;
                    buttons[2] = btn3;
                }
            }
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_1: selected_option = 1; break;
                    case SDLK_2: selected_option = 2; break;
                    case SDLK_3: selected_option = 3; break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 148, 193, 4, 255);
        SDL_RenderClear(renderer);

        draw_text(renderer, *window_width / 2, *window_width * 0.15, 4, "Retro Snake");

        for (int i = 0; i < 3; i++) {
            bool is_hovered = SDL_PointInRect(&mouse_pos, &buttons[i]);
            bool is_clicked = (selected_option == (i + 1));
            Uint8 bg_alpha = is_clicked ? 160 : (is_hovered ? 90 : 30);
            
            boxRGBA(renderer, buttons[i].x, buttons[i].y, 
                    buttons[i].x + buttons[i].w, buttons[i].y + buttons[i].h, 
                    20, 20, 20, bg_alpha);

            if (is_hovered || is_clicked) {
                int thickness = is_clicked ? 5 : 3;
                draw_border(buttons[i].x, buttons[i].y, 
                            buttons[i].x + buttons[i].w, buttons[i].y + buttons[i].h, 
                            thickness, 3, renderer);
            }

            stringRGBA(renderer, buttons[i].x + 24, buttons[i].y + 16, labels[i], 255, 255, 255, 255);
        }

        SDL_RenderPresent(renderer);
        if (selected_option != 0) {
            SDL_Delay(100); 
            return selected_option;
        }
        SDL_Delay(16); // ~60 FPS cap
    }
}

static void draw_snake(SNAKE_PART *snake,SDL_Renderer *renderer, int block_width){
    SNAKE_PART *iterator = snake;
    while (iterator != NULL){
        boxRGBA(renderer, iterator->x, iterator->y, iterator->x+block_width - 2, iterator->y+block_width - 2, 113, 124, 107, 255);
        iterator = iterator->next;
    }
}

static void draw_apple(APPLE apple, SDL_Renderer *renderer, int block_width){
    filledCircleRGBA(renderer, apple.x + block_width / 2, apple.y + block_width / 2, apple.r, 113, 124, 107, 255);
}

static void draw_header(SDL_Renderer *renderer, int window_width, int score){
    int line_upper_left_x = window_width * 0.1;
    int line_upper_right_x = window_width * 0.9;
    int line_y = window_width * 0.075;
    lineRGBA(renderer, line_upper_left_x, line_y, line_upper_right_x, line_y, 50, 50, 50, 255);

    char *ingame_title = "Retro Snake";
    stringRGBA(renderer, line_upper_left_x, line_y - 10, ingame_title, 240, 240, 240, 255);
    char score_str[32];
    snprintf(score_str, sizeof(score_str), "Score: %d", score);
    int text_width = strlen(score_str) * 8;
    int score_x = line_upper_right_x - text_width;
    stringRGBA(renderer, score_x, line_y - 8 - 2, score_str, 240, 240, 240, 255);
}

static void draw_board(SDL_Renderer *renderer, int window_width, int useable_width, int offset, int score) {
    int border_thickness = 5;
    int space = 2;

    int upper_left_x = offset - border_thickness * 2;
    int upper_left_y = offset - border_thickness * 2;
    int bottom_right_x = offset + useable_width + border_thickness * 2;
    int bottom_right_y = offset + useable_width + border_thickness * 2;
    draw_border(upper_left_x, upper_left_y, bottom_right_x, bottom_right_y, border_thickness, space, renderer);
}

void draw_a_game_frame(SDL_Renderer *renderer, int window_width, int useable_width, int offset, int block_width, int player_num, SNAKE_PART *snake1, SNAKE_PART *snake2, APPLE apple, int score) {
    SDL_SetRenderDrawColor(renderer, 148, 193, 4, 255);
    SDL_RenderClear(renderer);
    draw_header(renderer, window_width, score);
    draw_board(renderer, window_width, useable_width, offset, score);
    draw_apple(apple, renderer, block_width);
    draw_snake(snake1, renderer, block_width);
    if (player_num == 2) {
        draw_snake(snake2, renderer, block_width);
    }
    SDL_RenderPresent(renderer);
}

static void draw_scoreboard_content(SDL_Renderer *renderer, int window_width, int score, const int top10[]) {
    SDL_SetRenderDrawColor(renderer, 148, 193, 4, 255);
    SDL_RenderClear(renderer);

    draw_header(renderer, window_width, score);
    draw_text(renderer, window_width / 2, (int)(window_width * 0.2), 4, "Score board");

    int start_x = (int)(window_width * 0.5) - 110;
    int start_y = (int)(window_width * 0.35);
    int line_height = 20;
    int display_count = 0;

    for (int i = 9; i >= 0; i--) {
        if (top10[i] != 0) {
            char text[64];
            int rank = 10 - i;
            snprintf(text, sizeof(text), "%d. helyezes: %d blokk hossz", rank, top10[i]);
            
            int current_y = start_y + (display_count * line_height);
            stringRGBA(renderer, start_x, current_y, text, 240, 240, 240, 255);
            display_count++;
        }
    }
}

void draw_scoreboard(SDL_Renderer *renderer, SDL_Window *window, int score, int *window_width, int block_num, double usable_area, int *useable_width, int *block_width, int *offset, const int top10[]) {
    draw_scoreboard_content(renderer, *window_width, score, top10);

    SDL_RenderPresent(renderer);

    bool running = true;
    SDL_Event event;
    while (running) {
        int mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);
        
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    *window_width = event.window.data1;
                    rescale(window, window_width, block_num, usable_area, useable_width, block_width, offset);
                    draw_scoreboard_content(renderer, *window_width, score, top10);
                    SDL_RenderPresent(renderer);
                }
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
            }
        }
        SDL_Delay(16);
    }
}

void draw_winner(SDL_Renderer *renderer, int window_width, int winner) {
    SDL_SetRenderDrawColor(renderer, 148, 193, 4, 255);
    SDL_RenderClear(renderer);

    draw_header(renderer, window_width, 0);
    char winner_text[20];
    if (winner == 1) {
        snprintf(winner_text, sizeof(winner_text), "Winner is player 1!");
    } else if (winner == 2) {
        snprintf(winner_text, sizeof(winner_text), "Winner is player 2!");
    } else if (winner == 0) {
        snprintf(winner_text, sizeof(winner_text), "Draw!");
    }
    draw_text(renderer, window_width / 2, (int)(window_width * 0.5), 3, winner_text);

    SDL_RenderPresent(renderer);

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
            }
        }
        SDL_Delay(33);
    }
}