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

void Insert_Sort(long *Array, int Size, int Start, int Increment, double *N_Comp, double *N_Move);
void Bubble_Sort(long *Array, int Size, int Start, int Increment, double *N_Comp, double *N_Move);
sequence_t Two_Three_Seq(int Size);

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
        for (j = info.seq[i]; j < Size; j++) {
            Insert_Sort(Array, Size, j, info.seq[i], N_Comp, N_Move);
        }
    }
    free(info.seq);
}

void Insert_Sort(long *Array, int Size, int Start, int Increment, double *N_Comp, double *N_Move) {
    int i, j;
    for (i = Start + Increment; i < Size; i += Increment) {
		long temp = Array[i];
        for (j = i; j >= Increment && temp < Array[j - Increment]; j -= Increment) {
            Array[j] = Array[j - Increment];
            *N_Comp = *N_Comp + 1;
            *N_Move = *N_Move + 1;
        }
		Array[j] = temp;
        *N_Comp = *N_Comp + 1;
    }
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

void Improved_Bubble_Sort(long *Array, int Size, double *N_Comp, double *N_Move) {
    sequence_t info = One_N_Seq(Size);
    int i;
    for (i = 0; i < info.size; i++) {
        //printf("%d\n", info.seq[i]);
        Bubble_Sort(Array, Size, info.seq[i] - 1, info.seq[i], N_Comp, N_Move);
    }
	free(info.seq);
}


/*void Bubble_Sort(long *Array, int Size, int Start, int Increment, double *N_Comp, double *N_Move) {
    int left_idx  = Start;
    int right_idx = Size - 1 - Increment;

	int i , j;
    long temp;
    bool go_left  = false;
    while (right_idx > left_idx) {
        go_left  = !go_left;
        int pos;
        int gap = go_left ? Increment: -1 * Increment;
        for (j = go_left ? left_idx : right_idx; j <= right_idx && j >= left_idx; j += gap) {
            *N_Comp = *N_Comp + 1;
            if (Array[j] > Array[j + Increment]) {
                temp                 = Array[j];
                Array[j]             = Array[j + Increment];
                Array[j + Increment] = temp; 
                *N_Move              = *N_Move + Increment;
                pos                  = j;
            }
        }
        if (go_left) {
            right_idx = pos - Increment;
        }
        else {
            left_idx = pos + Increment; 
        }
    }
}*/
void Bubble_Sort(long *Array, int Size, int Start, int Increment, double *N_Comp, double *N_Move) {
	int i, j;
	int pos = Size - 1 - Increment;;
	while (pos != -1) {
		int p = -1;
		for (j = Start; j <= pos; j += Increment) {
			*N_Comp = *N_Comp + 1;
			if (Array[j] > Array[j + Increment]) {
				long temp            = Array[j];
				Array[j]             = Array[j + Increment];
				Array[j + Increment] = temp; 
				p                    = j;
				*N_Move              = *N_Move + 1;
			}
		}
		pos = p;
	}
}
