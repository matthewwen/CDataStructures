#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include "sorting.h"

long *Load_File(char * Filename, int * Size) {
    FILE * fp = fopen(Filename, "r");
    char c;

    for (*Size = 0; (c = fgetc(fp)) != EOF; *Size += c == '\n' ? 1: 0) {}
    fseek(fp, -1L, SEEK_CUR);
    long num_elements = ftell(fp);

    long * num = malloc(sizeof(*num) * *Size);

    for (int i = *Size - 1; i >= 0; i--) {
        num[i] = 0;
        fseek(fp, -1L, SEEK_CUR);
        for (int j = 1; num_elements > 0 && (c = fgetc(fp)) != '\n'; j *= 10){
            num[i] += j * (c - '0');
            num_elements -= 1;
            fseek(fp, -2L, SEEK_CUR);
        }
        fseek(fp, -1L, SEEK_CUR);
        num_elements--;
    }

    return num;
}

int Save_File(char *Filename, long *Array, int Size) {
    FILE * fp = fopen(Filename, "w");
    int size  = 2;
    for(long val = LONG_MAX; val > 0 && (++size > 0); val /= 10);
    char * buffer = malloc(size * sizeof(*buffer));
    for (int i = 0; i < Size; i++) {
        sprintf(buffer, "%ld\n", Array[i]);
        fwrite(buffer, sizeof(*buffer), strlen(buffer), fp);
    }
    free(buffer);
    fclose(fp);
}

void Shell_Insertion_Sort(long *Array, int Size, double *N_Comp, double *N_Move) {

}

void Improved_Bubble_Sort(long *Array, int Size, double *N_Comp, double *N_Move) {

    for (;;) {

    }

}