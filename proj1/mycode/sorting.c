#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <stdint.h>
#include "sorting.h"

typedef struct{
    int * sequence;
    int size;
}sequence_t;

void Insert_Sort(long *Array, int Size, int Start, int Increment, double *N_Comp, double *N_Move);
void Bubble_Sort(long *Array, int Size, double *N_Comp, double *N_Move);
sequence_t Two_Three_Seq(int Size);

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
    fclose(fp);

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
    int i;
    int j;
    sequence_t info = Two_Three_Seq(Size);
    for (i = 0; i < info.size; i++) {
        for (j = 0; j < info.sequence[i]; j++) {
            Insert_Sort(Array, Size, j, info.sequence[i], N_Comp, N_Move);
        }
    }
    free(info.sequence);
}

sequence_t Two_Three_Seq(int Size) {
    int degree = 0;

    int i;
    int j;
    for(i = 1; i * 3 < Size && ++degree; i *= 3){}
    int * seq_carrier = malloc((degree + 1) * sizeof(*seq_carrier));

    int two_mult = 1;
    int num_seq  = degree + 1;
    int num_two  = 0;
    for (j = degree; i > 0; i /= 3) {
        seq_carrier[j] = i * two_mult;
        num_seq += num_two;
        for(;seq_carrier[j] * 2 < Size; seq_carrier[j] *= 2){
            two_mult *= 2;
            num_two++;
            num_seq++;
        }
        j--;
    }

    int * seq = malloc(num_seq * sizeof(*seq));
    for (i = 0; i < num_seq; i++) {
        int max_idx = 0;
        for (j = 1; j <= degree; j++) {
            if (seq_carrier[j] != -1 && seq_carrier[max_idx] < seq_carrier[j]) {
                max_idx = j;
            }
        }
        seq[i] = seq_carrier[max_idx];
        seq_carrier[max_idx] = seq_carrier[max_idx] % 2 != 0 ? -1 : seq_carrier[max_idx] / 2;
    }
    
   free(seq_carrier);
   return (sequence_t) {.sequence = seq, .size = num_seq};
}

void Improved_Bubble_Sort(long *Array, int Size, double *N_Comp, double *N_Move) {
}

void Insert_Sort(long *Array, int Size, int Start, int Increment, double *N_Comp, double *N_Move) {
    int i, j;
    for (i = Start; i < Size; i += Increment) {
        for (j = i; j >= Increment && Array[j] < Array[j - Increment]; j -= Increment) {
            long temp   = Array[j];
            Array[j]    = Array[j - Increment];
            Array[j - Increment] = temp;
            *N_Comp = *N_Comp + 1;
            *N_Move = *N_Move + 1;
        }
        *N_Comp = *N_Comp + 1;
    }
}

void Bubble_Sort(long *Array, int Size, double *N_Comp, double *N_Move) {
    int left_idx  = 0;
    int right_idx = Size - 2;

	int i , j;
    long temp;
    for (i = 0; i < Size && (left_idx >= 0) && (right_idx >= 0); i++) {
        bool go_left  = (i % 2) == 0;
        int pos       = -1;
        int increment = go_left ? 1: -1;
        for (j = go_left ? left_idx : right_idx; j <= right_idx && j >= left_idx; j += increment) {
            *N_Comp = *N_Comp + 1;
            if (Array[j] > Array[j + 1]) {
                temp         = Array[j];
                Array[j]     = Array[j + 1];
                Array[j + 1] = temp; 
                *N_Move      = *N_Move + 1;
                pos          = j;
            }
        }
        if (go_left) {
            right_idx = pos - 1;
        }
        else {
            left_idx = pos + 1; 
        }
    }
}