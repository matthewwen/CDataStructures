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
void FCK_Two_Three_Seq(int ** s, int * N);
sequence_t One_N_Seq(int Size);
void save_file(char * Filename, sequence_t seq);

long *Load_File(char * Filename, int * Size) {
    FILE * fp = fopen(Filename, "r");
	if (fp == NULL) {
		return NULL;
	}

    char c;
	int s = 0;
    for (; (c = fgetc(fp)) != EOF; s += c == '\n' ? 1: 0) {}
	*Size = --s;

    long * num = malloc(sizeof(*num) * s);
	
	if (num != NULL) {
		int i;
		fseek(fp, 0L, SEEK_SET);
		fscanf(fp, "%ld", num);
		for (i = 0; i < *Size; i++) {
			fscanf(fp, "%ld", num + i);
		}
	}
    fclose(fp);

    return num;
}

int Save_File(char *Filename, long *Array, int Size) {
    FILE * fp = fopen(Filename, "w");
    int i;
    fprintf(fp, "%d\n", Size);
    for (i = 0; i < Size; i++) {
        fprintf(fp, "%ld\n", Array[i]);
    }
    fclose(fp);
	return i;
}

void Shell_Insertion_Sort(long *Array, int Size, double *N_Comp, double *N_Move) {
	double local_comp = 0.0;
	double local_move = 0.0;

	int * seq = NULL;
	int seq_size = Size;
    FCK_Two_Three_Seq(&seq, &seq_size);

    int i, j, k;
    for (i = 0; i < seq_size; i++) {
		long gap = seq[i];
        for (j = gap; j < Size; j++) {
            long temp = Array[j];
			long prev_element;
            for (k = j; k >= gap && temp < (prev_element = Array[k - gap]); k -= gap) {
                Array[k] = prev_element;
                local_comp = local_comp + 1;
                local_move = local_move + 1;
            }
            Array[k] = temp;
            local_comp = local_comp + 1;
        }
    }

	*N_Comp += local_comp;
	*N_Move += local_move;

    free(seq);
}

void Improved_Bubble_Sort(long *Array, int Size, double *N_Comp, double *N_Move) {
	double local_comp = 0.0;
	double local_move = 0.0;
    sequence_t info = One_N_Seq(Size);
    int i, j, k;
    for (i = 0; i < info.size; i++) {
		int gap = info.seq[i];
        for (j = gap; j < Size; j++) {
			long temp;
            for (k = j; k > gap && Array[k - gap] > (temp = Array[k]); k -= gap) {
                local_comp = local_comp + 1;
                local_move = local_move + 1;
                Array[k]       = Array[k - gap];
                Array[k - gap] = temp; 
            }
			local_comp = local_comp + 1;
        }
    }
	*N_Comp += local_comp;
	*N_Move += local_move;
	free(info.seq);
}

void FCK_Two_Three_Seq(int ** s, int * N) {
    int degree = 0;
	int Size = *N;

    int i, j;
    for(i = 1; i * 3 < Size && ++degree; i *= 3){}
    int * seq_carrier = malloc((degree + 1) * sizeof(*seq_carrier));

    int two_mult = 1;
    int num_seq  = degree + 1;
    int num_two  = 0;
    for (j = degree; i > 0; i /= 3) {
        seq_carrier[j] = i * two_mult;
        for(;seq_carrier[j] * 2 < Size; seq_carrier[j] *= 2){
            two_mult *= 2;
            num_two++;
        }
		num_seq += num_two;
        j--;
    }

    int * seq = malloc(num_seq * sizeof(*seq));
	int last_idx = degree;
    for (i = 0; i < num_seq; i++) {
        int max_idx = 0;
		long max_value = seq_carrier[max_idx];
        for (j = 1; j <= last_idx; j++) {
			long temp;
            if (seq_carrier[j] != -1 && max_idx < (temp = seq_carrier[j])) {
                max_idx = j;
				max_value = temp;
            }
        }
        seq[i] = max_value;
		if (seq_carrier[max_idx] % 2 != 0) {
			seq_carrier[max_idx] = seq_carrier[last_idx];
			last_idx--;
		}
		else {
			seq_carrier[max_idx] /= 2;
		}
    }
    
   free(seq_carrier);
   *s = seq;
   *N = num_seq;
   //return (sequence_t) {.seq = seq, .size = num_seq};
}

sequence_t Two_Three_Seq(int Size) {
    int * seq = malloc(Size * sizeof(*seq));
	seq[0] = 1;
	int p2 = 0;
	int p3 = 0;
	int s  = 0;

	int i;
	int u2 = 0;
	int u3 = 0;
	for (i = 1; u2 < Size && u3 < Size && i < Size; i++){
		if (seq[p2] * 2 == seq[i - 1]) {
			p2 += 1;
		}
		if (seq[p3] * 3 == seq[i -1]) {
			p3 += 1;
		}
		u2 = seq[p2] * 2;
		u3 = seq[p3] * 3;
		if (u2 < u3) {
			p2 += 1;
			seq[i] = u2;
		}
		else {
			p3 += 1;
			seq[i] = u3;
		}
		s++;
	}

	return (sequence_t) {.seq = seq, .size = s};

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
    //sequence_t info = FCK_Two_Three_Seq(N);
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
