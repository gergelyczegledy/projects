#ifndef FILE_HANDLING_H_INCLUDED
#define FILE_HANDLING_H_INCLUDED

void write_to_file(char *file, int score);
bool read_from_file(char *file, int *top10);
void ascending_order(int *top10, int x);
void put_in_order(int score, int *top10, int x);
void create_empty_file(char *file);
void save_score(int score, int *top10);

#endif // FILE_HANDLING_H_INCLUDED
