#define _HUFF_C
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "huff.h"

node_t * f_test(header_t * head);
void assign_loc(node_t * head, uint64_t loc[2], int size);
APPEND_TREE()
FREE_TREE()
P_BIT()
APPEND_BIT()

int base(int occ) {
	uint32_t num = 1;
	return num << occ;
}

void tree_fun(node_t * head) {
	if (head == NULL) {
		printf("null, reverse");
		return;
	}
	// pre order
	if (head->type == VALUE) {
		printf("value: %c\n", head->data.value->value);
	}
	else {
		printf("left\n");
		tree_fun(head->data.i->left);
		printf("back left\n");
		printf("right\n");
		tree_fun(head->data.i->right);
		printf("back right\n");
	}
}

header_t * create_table(FILE * fp) {
	size_t memory_size;
	header_t * header = malloc((memory_size = sizeof(*header)));
	memset(header, 0, memory_size);

	char i;
	fseek(fp, 0L, SEEK_SET);
	while (((i = fgetc(fp)) != EOF)) {
		header->values[(unsigned int) i].weight += 1;
	}

	return header;
}

node_t * f_test(header_t * head) {
	// perform insertion, sort in assending order
	value_t * order_list[NUM_CHAR];
	int i, j, last_pos = 0;
	node_t * nhead = NULL;

	value_t * pos;
	for (i = 0; i < NUM_CHAR; i++) {
		if ((pos = &head->values[i])->weight != 0) {
			pos->value = i;
			for (j = last_pos; j > 0 && order_list[j - 1]->weight > pos->weight; j--) {
				order_list[j] = order_list[j - 1];
			}
			order_list[j] = pos;
			last_pos++;
		}
	}

	// saving inside of header
	memcpy(head->sorted, order_list, sizeof(order_list));
	head->buffer_end = last_pos;

	// add element into binary tree
	i_t * i_node;
	node_t * left, * right;
	for (i = 0; i < last_pos - 1; i += 2) {
		i_node = malloc(sizeof(*i_node));
		left   = malloc(sizeof(*left));
		right  = malloc(sizeof(*right));

		*left  = (node_t) {.type = VALUE, .data = (data_t) {.value = order_list[i]}};
		*right = (node_t) {.type = VALUE, .data = (data_t) {.value = order_list[i + 1]}};
		i_node->weight = order_list[i]->weight + order_list[i + 1]->weight;
		i_node->left   = left;
		i_node->right  = right;
		append_tree(&nhead, i_node);
	}

	// if odd number of elements is noticed
	if (i == (last_pos - 1)) {
		left   = malloc(sizeof(*left));
		i_node = malloc(sizeof(*i_node));
		*left  = (node_t) {.type = VALUE, .data = (data_t) {.value = order_list[last_pos - 1]}};
		*i_node = (i_t) {.weight = left->data.value->weight, .left = left, .right = NULL};
		append_tree(&nhead, i_node);
	}

	uint64_t loc_holder[2] = {0, 0};
	assign_loc(nhead, loc_holder, 0);
	return nhead;
}

void push_front(uint64_t stack[2], int start) {
	int max_bit = MAX_BIT;
	for (;start < (max_bit * 2); start++) {
		stack[1] = stack[1] << 1;
		stack[1] += (~stack[0]) < (stack[0]) ? 1: 0;
		stack[0] = stack[0] << 1;
	}
}

void assign_loc(node_t * head, uint64_t loc[2], int size){
	if (head != NULL && head->type == NODE) {
		uint64_t left[2], right[2];

		memcpy(left, loc, sizeof(left));
		memcpy(right, loc, sizeof(right));

		// shift bits to the left by 1
		left[1]  = left[1] << 1;
		right[1] = right[1] << 1;

		// push elements if necessary
		left[1] += ~left[0] < left[0] ? 1: 0;
		right[1] += ~right[0] < right[0] ? 1: 0;

		// shift bits to the left by 1
		left[0]  = left[0] << 1;
		right[0] = right[0] << 1;

		// add element to bottom
		right[0] = right[0] + 1;
		
		assign_loc(head->data.i->left, left, size + 1);
		assign_loc(head->data.i->right, right, size + 1);
	}
	else if (head != NULL && head->type == VALUE) {
		memcpy(head->data.value->loc, loc, 2 * sizeof(uint64_t));
		push_front(head->data.value->loc, size);
		head->data.value->numbit = size;
	}
}

void write_header(header_t * h, FILE * fp) {
	int i, w;
	value_t *v1, *v2;
	for (i = 0; i < h->buffer_end - 1; i+=2) {
		fputc((v1 = h->sorted[i])->value, fp);
		fputc((v2 = h->sorted[i + 1])->value, fp);
		w = v1->weight + v2->weight;
		fwrite(&w, sizeof(w), 1, fp);
	}

	if (i == (h->buffer_end - 1)) {
		fputc((v1 = h->sorted[h->buffer_end - 1])->value, fp);
		fputc(0, fp);
		w = v1->weight;
		fwrite(&w, sizeof(w), 1, fp);
	}
}

int main(int argc, char* argv[]) {
	bool input_valid = false;
	node_t * nhead   = NULL;

	if ((input_valid = (argc == 2))) {
		// count occurance
		FILE * fp = fopen(argv[1], "r");
		header_t * h = create_table(fp);
		h->decompressed_size = ftell(fp);
		fseek(fp, 0L, SEEK_SET);

		// create tree, and table
		nhead = f_test(h);

		NEW_FILE(n_name, argv[1], ".huff")
		FILE * write_fp = fopen(n_name, "w");

		// write header into file
		size_t com_size, h_size, dec_size;
		fwrite(h, (com_size = sizeof(h->compressed_size)) + 
		           (h_size = sizeof(h->header_size)) + 
				   (dec_size = sizeof(h->decompressed_size)), 1, write_fp);
		write_header(h, write_fp);
		h->header_size = ftell(write_fp);

		// write data into it
		int idx; 
		uint8_t buffer = 0;
		uint64_t idx_loc[2];
		int buffer_size = 0;
		fseek(fp, 0, SEEK_SET);
		for (;(idx = fgetc(fp)) != EOF;) {
			value_t value = h->values[idx];
			int size = value.numbit;
			memcpy(idx_loc, value.loc, sizeof(idx_loc));
			if (size > 8) {
				printf("size is greater than 8 %c\n", size);
			}
			while (size > 0) {
				if (buffer_size == (sizeof(buffer) * 8)) {
					fwrite(&buffer, sizeof(buffer), 1, write_fp);
					buffer = 0, buffer_size = 0;
				}
				append(&buffer, push_bit(idx_loc, &size));
				buffer_size++;
			}
		}
    	buffer = buffer << ((sizeof(buffer) * 8) - buffer_size);
		fwrite(&buffer, sizeof(buffer), 1, write_fp);
		h->compressed_size = ftell(write_fp);
		free_tree(&nhead);

		// edit the header
		fseek(write_fp, 0L, SEEK_SET);
		fwrite(&h->compressed_size, com_size, 1, write_fp);
		fwrite(&h->header_size, h_size, 1, write_fp);
		fwrite(&h->decompressed_size, dec_size, 1, write_fp);
		printf("compressed: %ld, header: %ld, decompressed: %ld\n", h->compressed_size, h->header_size, h->decompressed_size);

		// clean up
		fclose(write_fp);
		fclose(fp);
		free(h);
		free(n_name);

	}
	return input_valid ? EXIT_SUCCESS: EXIT_FAILURE;
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
