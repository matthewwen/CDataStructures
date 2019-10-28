#define _UNHUFF_C
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "huff.h"

APPEND_TREE()
FREE_TREE()
P_BIT()

bool f_test(node_t * curr, bit_t stack, uint64_t com_s, FILE * fp, FILE * fp_w);
void v_test(node_t * head, bit_t stack, uint64_t com_s, FILE * fp, FILE * fp_w) {
	while (ftell(fp_w) < com_s) {
		f_test(head, stack, com_s, fp, fp_w);
	}
}

bool f_test(node_t * curr, bit_t stack, uint64_t com_s, FILE * fp, FILE * fp_w) {
	if (curr->type == VALUE) {
		fputc(curr->data.value->value, fp_w);
		return true;
	}
	if (*stack.size == 0) {
		if (ftell(fp_w) >= com_s) {
			return false;
		}
		fread(stack.stack, sizeof(*stack.stack), 1, fp);
		*stack.size = 8 * sizeof(*stack.stack);
	}
	uint8_t cp  = ~(*stack.stack);
	int dir = cp < *stack.stack;
	*stack.stack = (*stack.stack) << 1;
	*stack.size -= 1;
	if (dir == 0) {
		return f_test(curr->data.i->left, stack, com_s, fp, fp_w);
	}
	return f_test(curr->data.i->right, stack, com_s, fp, fp_w);
}

int main(int argc, char* argv[]) {
	bool input_valid = false;
	if ((input_valid = (argc == 2))) {
		FILE * fp = fopen(argv[1], "r");
		NEW_FILE(nname, argv[1], ".unhuff")
		FILE * fp_w = fopen(nname, "w");
		header_t * h = malloc(sizeof(*h));
		size_t com_size, h_size, dec_size;
		fread(h, (com_size = sizeof(h->compressed_size)) + 
		         (h_size = sizeof(h->header_size)) + 
				 (dec_size = sizeof(h->decompressed_size)), 1, fp);
		int pair_size = 2 * sizeof(char) + sizeof(int);
		int num_pair  = (h->header_size -  com_size - h_size - dec_size) / pair_size;
		int i;
		value_t values[NUM_CHAR];
		node_t * head = NULL;
		for (i = 0; i < num_pair; i++) {
		 	char l, r;
			int w;
			l = fgetc(fp);
			r = fgetc(fp);
			values[2 * i] = (value_t) {.value = l};
			values[(2 * i) + 1] = (value_t) {.value = r};
			fread(&w, sizeof(w), 1, fp);
			i_t * i_node  = malloc(sizeof(*i_node));
			i_node->left  = malloc(sizeof(*i_node->left));
			i_node->right = malloc(sizeof(*i_node->right));
			i_node->weight = w;

			*i_node->left  = (node_t) {.type = VALUE, .data = (data_t) {.value = &values[2 * i]}};
			*i_node->right = (node_t) {.type = VALUE, .data = (data_t) {.value = &values[(2 * i) + 1]}};

			append_tree(&head, i_node);
		}
		uint8_t stack;
		int size = 0;
		v_test(head, (bit_t) {.stack = &stack, .size = &size}, h->decompressed_size, fp, fp_w);
		fclose(fp);
		free(h);
		free_tree(&head);

		fclose(fp_w);
		free(nname);
	}
	return input_valid ? EXIT_SUCCESS: EXIT_FAILURE;
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */