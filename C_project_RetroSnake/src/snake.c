#include "../include/snake.h"

// #include "../include/debugmalloc.h" // IWYU pragma: keep

void snake_direction(SNAKE_PART *snake, SDL_Event event, int player){
    if (snake != NULL){
        Dir prev_dir = snake->next->dir;
        if (player == 1){
            switch (event.key.keysym.sym){
                case SDLK_w:
                    if (prev_dir != DOWN)
                        snake->dir = UP;
                    break;
                case SDLK_s:
                    if (prev_dir != UP)
                        snake->dir = DOWN;
                    break;
                case SDLK_a:
                    if (prev_dir != RIGHT)
                        snake->dir = LEFT;
                    break;
                case SDLK_d:
                    if (prev_dir != LEFT)
                        snake->dir = RIGHT;
                    break;
            }
        }
        if (player == 2){
            switch (event.key.keysym.sym){
                case SDLK_UP:
                    if (prev_dir != DOWN)
                        snake->dir = UP;
                    break;
                case SDLK_DOWN:
                    if (prev_dir != UP)
                        snake->dir = DOWN;
                    break;
                case SDLK_LEFT:
                    if (prev_dir != RIGHT)
                        snake->dir = LEFT;
                    break;
                case SDLK_RIGHT:
                    if (prev_dir != LEFT)
                        snake->dir = RIGHT;
                    break;
            }
        }
    }
}

void move_snake(SNAKE_PART* kigyo, int block_width){
    SNAKE_PART previous_part = *kigyo;
    switch(kigyo->dir)
    {
        case LEFT:
            kigyo->x = kigyo->x-block_width;
            break;
        case RIGHT:
            kigyo->x = kigyo->x+block_width;
            break;
        case UP:
            kigyo->y = kigyo->y-block_width;
            break;
        case DOWN:
            kigyo->y = kigyo->y+block_width;
            break;
    }

    SNAKE_PART *iterator = kigyo->next;
    SNAKE_PART temp;
    while (iterator != NULL){
        temp = *iterator;
        iterator->x = previous_part.x;
        iterator->y = previous_part.y;
        iterator->dir = previous_part.dir;
        previous_part = temp;
        iterator = iterator->next;
    }
}


void grow_snake(SNAKE_PART *snake, int block_width){
    SNAKE_PART *last_part = snake;
    while (last_part->next != NULL){
        last_part = last_part->next;
    };

    SNAKE_PART *new_part = (SNAKE_PART*) malloc(sizeof(SNAKE_PART));
    last_part->next = new_part;
    new_part->next = NULL;

    switch (last_part->dir){
        case LEFT:
            new_part->x = last_part->x+block_width;
            new_part->y = last_part->y;
            new_part->dir = last_part->dir;
            break;
        case RIGHT:
            new_part->x = last_part->x-block_width;
            new_part->y = last_part->y;
            new_part->dir = last_part->dir;
            break;
        case UP:
            new_part->x = last_part->x;
            new_part->y = last_part->y+block_width;
            new_part->dir = last_part->dir;
            break;
        case DOWN:
            new_part->x = last_part->x;
            new_part->y = last_part->y-block_width;
            new_part->dir = last_part->dir;
            break;
    }
}

bool snake_collision(SNAKE_PART *snake1,SNAKE_PART *snake2, int useable_width, int offset, int *winner){
    SNAKE_PART *iterator = snake1->next;
    if (snake2 == NULL){
        if (snake1->x < offset || snake1->x >= offset + useable_width || snake1->y < offset || snake1->y >= offset + useable_width)
            return true;
        while(iterator != NULL){
            if (snake1->x == iterator->x && snake1->y == iterator->y)
                return true;
            iterator = iterator->next;
        }
    }
    if (snake2 != NULL){
        if (snake1->x == snake2->x && snake1->y == snake2->y){
            *winner = 0;
            return true;
        }
        else{
            while(iterator != NULL){
                if (snake2->x == iterator->x && snake2->y == iterator->y){
                    *winner = 1;
                    return true;
                }
                if (snake1->x == iterator->x && snake1->y == iterator->y){
                    *winner = 2;
                    return true;
                }
                iterator = iterator->next;
            }
            SNAKE_PART *iterator2 = snake2->next;
            if (snake2->x >= useable_width + offset || snake2->x < offset || snake2->y < offset || snake2->y >= useable_width + offset){
                *winner = 1;
                return true;
            }
            if (snake1->x >= useable_width + offset || snake1->x < offset || snake1->y < offset || snake1->y >= useable_width + offset){
                *winner = 2;
                return true;
            }
            while(iterator2 != NULL){
                if (snake1->x == iterator2->x && snake1->y == iterator2->y){
                    *winner = 2;
                    return true;
                }
                if (snake2->x == iterator2->x && snake2->y == iterator2->y){
                    *winner = 1;
                    return true;
                }
                iterator2 = iterator2->next;
            }
        }
    }
    return false;
}

void free_snake_memory(SNAKE_PART *snake){
    SNAKE_PART *iterator = snake;
    while (iterator != NULL){
        SNAKE_PART *temp = iterator;
        iterator = iterator->next;
        free(temp);
    }
}

void snake_init(SNAKE_PART **snake1, SNAKE_PART **snake2, int block_width, int offset, int snake_num){
    for (int i = 0; i < 3; i++){
        SNAKE_PART *new = (SNAKE_PART*) malloc(sizeof(SNAKE_PART));
        new->next = *snake1;
        new->x = block_width * (i-1) + offset;
        new->y = block_width * 11 + offset;
        new->dir = RIGHT;
        *snake1 = new;
    }
    if (snake_num == 2){
        for (int i = 0; i < 3; i++){
            SNAKE_PART *new = (SNAKE_PART*) malloc(sizeof(SNAKE_PART));
            new->next = *snake2;
            new->x = block_width * (21-i) + offset;
            new->y = block_width * 11 + offset;
            new->dir = LEFT;
            *snake2 = new;
        }
    }
}