#include "../include/font_bitmap.h"
#include <SDL2/SDL2_gfxPrimitives.h>
// #include "../include/debugmalloc.h" // IWYU pragma: keep

void draw_text(SDL_Renderer *renderer, int center_x, int start_y, int scale, const char text[]) {
    const int font_R[] = {  0b11110, 
                            0b10001,
                            0b11110, 
                            0b10100, 
                            0b10001};
    const int font_E[] = {  0b11111, 
                            0b10000, 
                            0b11100, 
                            0b10000, 
                            0b11111};
    const int font_T[] = {  0b11111, 
                            0b00100, 
                            0b00100, 
                            0b00100, 
                            0b00100};
    const int font_O[] = {  0b01110, 
                            0b10001, 
                            0b10001, 
                            0b10001, 
                            0b01110};
    const int font_S[] = {  0b01111, 
                            0b10000, 
                            0b01110, 
                            0b00001, 
                            0b11110};
    const int font_N[] = {  0b10001, 
                            0b11001, 
                            0b10101, 
                            0b10011, 
                            0b10001};
    const int font_A[] = {  0b01110, 
                            0b10001, 
                            0b11111, 
                            0b10001, 
                            0b10001};
    const int font_K[] = {  0b10001, 
                            0b10010, 
                            0b11100, 
                            0b10010, 
                            0b10001};
    const int font_B[] = {  0b11110, 
                            0b10001, 
                            0b11110, 
                            0b10001, 
                            0b11110};
    const int font_C[] = {  0b01111, 
                            0b10000, 
                            0b10000, 
                            0b10000, 
                            0b01111};
    const int font_D[] = {  0b11110, 
                            0b10001, 
                            0b10001, 
                            0b10001, 
                            0b11110};
    const int font_W[] = {  0b10001, 
                            0b10001, 
                            0b10101, 
                            0b11011, 
                            0b10001};
    const int font_I[] = {  0b11111, 
                            0b00100, 
                            0b00100, 
                            0b00100, 
                            0b11111};
    const int font_P[] = {  0b11110, 
                            0b10001, 
                            0b11110, 
                            0b10000, 
                            0b10000};
    const int font_L[] = {  0b10000, 
                            0b10000, 
                            0b10000, 
                            0b10000, 
                            0b11111};
    const int font_Y[] = {  0b10001, 
                            0b10001, 
                            0b01110, 
                            0b00100, 
                            0b00100};
    const int font_1[] = {  0b00100, 
                            0b01100, 
                            0b00100, 
                            0b00100, 
                            0b01110};
    const int font_2[] = {  0b01110, 
                            0b00001, 
                            0b00110, 
                            0b01000, 
                            0b11111};
    const int font_exclamation[] = {    0b00100, 
                                        0b00100, 
                                        0b00100, 
                                        0b00000, 
                                        0b00100};

    const int *retro_snake[] = { font_R, font_E, font_T, font_R, font_O, NULL, font_S, font_N, font_A, font_K, font_E };
    const int *score_board[] = { font_S, font_C, font_O, font_R, font_E, NULL, font_B, font_O, font_A, font_R, font_D };
    const int *winner_text_1[] = { font_W, font_I, font_N, font_N, font_E, font_R, NULL, font_I, font_S, NULL, font_P, font_L, font_A, font_Y, font_E, font_R , NULL, font_1 , font_exclamation };
    const int *winner_text_2[] = { font_W, font_I, font_N, font_N, font_E, font_R, NULL, font_I, font_S, NULL, font_P, font_L, font_A, font_Y, font_E, font_R , NULL, font_2 , font_exclamation };
    const int *winner_text_draw[] = { font_D, font_R, font_A, font_W, font_exclamation };

    const int **letters = NULL;
    int num_letters = 11;

    if (strcmp(text, "Retro Snake") == 0) {
        letters = retro_snake;
    } else if (strcmp(text, "Score board") == 0) {
        letters = score_board;
    } else if (strcmp(text, "Winner is player 1!") == 0) {
        letters = winner_text_1;
        num_letters = 19;
    } else if (strcmp(text, "Winner is player 2!") == 0) {
        letters = winner_text_2;
        num_letters = 19;
    } else if (strcmp(text, "Draw!") == 0) {
        letters = winner_text_draw;
        num_letters = 5;
    } else {
        return;
    }

    if (!letters) return;

    int letter_width = 6 * scale; // 5 units per letter + 1 unit space
    int total_width = num_letters * letter_width; 
    int current_x = center_x - (total_width / 2);

    for (int l = 0; l < num_letters; l++) {
        if (letters[l] != NULL) {
            for (int row = 0; row < 5; row++) {
                for (int col = 0; col < 5; col++) {
                    if ((letters[l][row] >> (4 - col)) & 1) {
                        int bx1 = current_x + col * scale;
                        int by1 = start_y + row * scale;
                        boxRGBA(renderer, bx1, by1, bx1 + scale - 1, by1 + scale - 1, 240, 240, 240, 255);
                    }
                }
            }
        }
        current_x += letter_width;
    }
}