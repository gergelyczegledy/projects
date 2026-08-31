#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

typedef enum {
    RIGHT,
    LEFT,
    UP,
    DOWN
} Dir;

typedef struct SNAKE_PART{
    int x;
    int y;
    struct SNAKE_PART *next;
    Dir dir;
}SNAKE_PART;

typedef struct APPLE{
    int x;
    int y;
    int r;
}APPLE;

#endif // STRUCTS_H_INCLUDED
