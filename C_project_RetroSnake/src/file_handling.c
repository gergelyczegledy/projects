#include <stdio.h>
#include <stdbool.h>

// #include "../include/debugmalloc.h" // IWYU pragma: keep

void write_to_file(char *file,int score){
    FILE *fp;
    fp = fopen(file,"a");

    if (fp == NULL){
        perror("Can't open file.");
        return;
    }

    fprintf(fp,"%d\n",score);
    fclose(fp);
}

bool read_from_file(char *file,int *top10){
    FILE *fp;
    fp = fopen(file,"r");

    if (fp == NULL){
        perror("Can't open file.");
        return false;
    }
    
    int score = 0;
    int i = 0;
    while(fscanf(fp,"%d",&score) != EOF){
        top10[i] = score;
        i++;
    }
    fclose(fp);
    return true;
}

void create_empty_file(char *file){
    FILE *f = fopen(file,"w");
    fclose(f);
}

void ascending_order(int *top10, int x) {
    for (int i = x-1; i > 0; --i) {
        for (int j = 0; j < i; ++j) {
            if (top10[j] > top10[j+1]) {
                int temp = top10[j];
                top10[j] = top10[j+1];
                top10[j+1] = temp;
            }
        }
    }
}

void put_in_order(int score, int *top10, int x){
    for (int i = 0; i < x; i++)
        if (i != 9){
            if (top10[i+1] > score){
                for (int j = 0; j < i; j++){
                    top10[j] = top10[j+1];
                }
                top10[i] = score;
                break;
            }
        }
        else{
            for (int j = 0; j < i; j++){
                top10[j] = top10[j+1];
            }
            top10[i] = score;
        }
}

void save_score(int score, int *top10){
    char file[] = "data/top10.txt";
    if (!read_from_file(file,top10)){
        write_to_file(file,score);
        top10[9] = score;
    }
    else{
        ascending_order(top10,10);
        if (score > top10[0]){
            put_in_order(score, top10, 10);
            create_empty_file(file);
            for (int i = 0; i < 10; i++)
                write_to_file(file,top10[i]);
        }
    }
}