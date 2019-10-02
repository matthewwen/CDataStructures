#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <stdint.h>
#include "sorting.h"
#include <assert.h>

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
	if (fp == NULL) {
		return 0;
	}
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

	int degree = 0;
	int i, j, k;
	for(i = 1; i * 3 < Size && ++degree; i *= 3){}
	int * seq_carrier = malloc((degree + 1) * sizeof(*seq_carrier));

	int two_mult = 1;
	for (j = degree; i > 0; i /= 3) {
		seq_carrier[j] = i * two_mult;
		for(;seq_carrier[j] * 2 < Size; seq_carrier[j] *= 2){
			two_mult *= 2;
		}
		j--;
	}

	int last_idx = degree;
	int max_idx;
	long gap;
	long gtemp, etemp;
	while (last_idx >= 0) {
		// Get's Next Value in Sequence
		max_idx = 0;
		gap     = seq_carrier[max_idx];
		for (i = 0; i <= last_idx; i++) {
			if (gap < (gtemp = seq_carrier[i])) {
				gap     = seq_carrier[i];
				max_idx = i;
			}
		}

		// Peforms insertion
		for (j = gap; j < Size; j++) {
			etemp = Array[j];
			for (k = j; k >= gap && etemp <  Array[k - gap]; k -= gap) {
				Array[k] = Array[k-gap];
				local_move = local_move + 1; 
			}
			Array[k] = etemp;
		}
		local_comp = local_comp + Size - gap + 1;

		// Update the Seq Carrier
		if (seq_carrier[max_idx] % 2 != 0) {
			seq_carrier[max_idx] = seq_carrier[last_idx];
			last_idx--;
		}
		else {
			seq_carrier[max_idx] /= 2;
		}
	}

	free(seq_carrier);
	local_comp = local_comp + local_move;
	*N_Comp += local_comp;
	*N_Move += local_move;

}

void Improved_Bubble_Sort(long *Array, int Size, double *N_Comp, double *N_Move) {
	double local_comp = 0.0;
	double local_move = 0.0;
	sequence_t info = One_N_Seq(Size);
	int j, k;
	long temp, temp_mgap;
	int gap = Size;
	while (gap > 0) {
		if (gap == 10 || gap == 9) {
			gap = 11;
		}
		for (j = gap; j < Size; j++) {
			for (k = j; k >= gap && (temp_mgap = Array[k - gap]) > (temp = Array[k]); k -= gap) {
				local_move = local_move + 1;
				Array[k]       = temp_mgap;
				Array[k - gap] = temp; 
			}
		}
		local_comp = local_comp + Size - gap + 1;
		gap = gap / 1.3;
	}
	local_comp = local_comp + local_move;
	*N_Comp += local_comp;
	*N_Move += local_move;
	free(info.seq);
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
