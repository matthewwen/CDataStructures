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

	int i;
    for (i = *Size - 1; i >= 0; i--) {
        num[i] = 0;
        fseek(fp, -1L, SEEK_CUR);
		int j;
        for (j = 1; num_elements > 0 && (c = fgetc(fp)) != '\n'; j *= 10){
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
	long val;
    for(val = LONG_MAX; val > 0 && (++size > 0); val /= 10);
    char * buffer = malloc(size * sizeof(*buffer));
	int i;
    for (i = 0; i < Size; i++) {
        sprintf(buffer, "%ld\n", Array[i]);
        fwrite(buffer, sizeof(*buffer), strlen(buffer), fp);
    }
    free(buffer);
    fclose(fp);
}

void Shell_Insertion_Sort(long *Array, int Size, double *N_Comp, double *N_Move) {

}

void Improved_Bubble_Sort(long *Array, int Size, double *N_Comp, double *N_Move) {
    int left_idx  = 0;
    int right_idx = Size - 1;

	int i;
    for (i = 0; i < Size && (left_idx >= 0) && (right_idx >= 0); i++) {
        bool go_left = (i % 2) == 0;
        int pos      = -1;
		int j;
        for (j = go_left ? left_idx : right_idx; j <= right_idx && j >= left_idx; j += go_left ? 1: -1) {
            *N_Comp = *N_Comp + 1;
            if (Array[j] > Array[j + 1]) {
                Array[j]     = Array[j] + Array[j + 1];
                Array[j + 1] = Array[j] - Array[j + 1];
                Array[j]     = Array[j] - Array[j + 1];
                *N_Move      = *N_Move + 1;
                pos          = j;
            }
        }
        left_idx     = go_left ? left_idx: pos + 1; 
        right_idx    = go_left ? pos - 1 : right_idx;
    }
}
