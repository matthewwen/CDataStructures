#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <stdint.h>
#include "sorting.h"

typedef struct{
    int * seq;
    int size;
}sequence_t;

sequence_t Two_Three_Seq(int Size);
sequence_t One_N_Seq(int Size);
void save_file(char * Filename, sequence_t seq);

long *Load_File(char * Filename, int * Size) {
    FILE * fp = fopen(Filename, "r");

    char c;
    for (*Size = 0; (c = fgetc(fp)) != EOF; *Size += c == '\n' ? 1: 0) {}

    long * num = malloc(sizeof(*num) * *Size);

	int i;
    fseek(fp, 0L, SEEK_SET);
    for (i = 0; i < *Size; i++) {
		fscanf(fp, "%ld", num + i);
    }
    fclose(fp);

    return num;
}

int Save_File(char *Filename, long *Array, int Size) {
    FILE * fp = fopen(Filename, "w");
    int i;
    for (i = 0; i < Size; i++) {
        fprintf(fp, "%ld\n", Array[i]);
    }
    fclose(fp);
}

void Shell_Insertion_Sort(long *Array, int Size, double *N_Comp, double *N_Move) {
    sequence_t info = Two_Three_Seq(Size);
    int i, j, k;
    for (i = 0; i < info.size; i++) {
        for (j = info.seq[i]; j < Size; j++) {
            long temp = Array[j];
            for (k = j; k >= info.seq[i] && temp < Array[k - info.seq[i]]; k -= info.seq[i]) {
                Array[k] = Array[k - info.seq[i]];
                *N_Comp = *N_Comp + 1;
                *N_Move = *N_Move + 1;
            }
            Array[k] = temp;
            *N_Comp = *N_Comp + 1;
        }
    }
    free(info.seq);
}

void Improved_Bubble_Sort(long *Array, int Size, double *N_Comp, double *N_Move) {
    sequence_t info = One_N_Seq(Size);
    int i, j, k;
    for (i = 0; i < info.size; i++) {
        for (j = info.seq[i]; j < Size; j++) {
            for (k = j; k > info.seq[i] && Array[k - info.seq[i]] > Array[k]; k -= info.seq[i]) {
                *N_Comp              = *N_Comp + 1;
                *N_Move              = *N_Move + 1;
                long temp            = Array[k];
                Array[k]             = Array[k - info.seq[i]];
                Array[k - info.seq[i]] = temp; 
            }
        }
    }
	free(info.seq);
}

sequence_t Two_Three_Seq(int Size) {
    int degree = 0;

    int i, j;
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
   return (sequence_t) {.seq = seq, .size = num_seq};
}

sequence_t One_N_Seq(int Size) {
    int * seq = malloc(sizeof(*seq) * Size);
    int N = Size;
    int i;
    int s = 0;
    for (i = 0; N > 1 && i < Size; i++) {
        N /= 1.3;
        N = (N == 9) || (N == 10) ? 11: N;
        seq[i] = N;
        s++;
    }
    return (sequence_t) {.seq = seq, .size = s};
}

void Save_Seq1(char * Filename, int N) {
    sequence_t info = Two_Three_Seq(N);
    save_file(Filename, info);
}

void Save_Seq2(char * Filename, int N) {
    sequence_t info = One_N_Seq(N);
    save_file(Filename, info);
}

void save_file(char * Filename, sequence_t info) {
    FILE * fp = fopen(Filename, "w");
    int i;
    for (i = 0; i < info.size; i++) {
        fprintf(fp, "%d\n", info.seq[i]);
    }
    fclose(fp);
}