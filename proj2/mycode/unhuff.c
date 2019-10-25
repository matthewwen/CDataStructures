#define _UNHUFF_C
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "huff.h"

APPEND_TREE()
FREE_TREE()
PUSH_BIT()
PUSH_FRONT()

int base(int occ) {
	uint32_t num = 1;
	return num << occ;
}

void tree_fun(node_t * head) {
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

bool f_test(node_t * curr, bit_t stack, uint64_t com_s, FILE * fp, FILE * fp_w);
void v_test(node_t * head, node_t * curr, bit_t stack, uint64_t com_s, FILE * fp, FILE * fp_w) {
	for (; ftell(fp_w) < com_s;) {
		f_test(head, stack, com_s, fp, fp_w);
	}
}

bool f_test(node_t * curr, bit_t stack, uint64_t com_s, FILE * fp, FILE * fp_w) {
	if (curr->type == VALUE) {
		//unsigned char val = curr->data.value->value;
		// printf("This is value: %c\n", val);
		fputc(curr->data.value->value, fp_w);
		return true;
	}
	if (*stack.size == 0) {
		if (ftell(fp_w) >= com_s) {
			// printf("end");
			return false;
		}
		fread(stack.stack, sizeof(*stack.stack), 1, fp);
		*stack.size = MAX_BIT;
		// printf("stack value: %ld\n", *stack.stack);
	}
	int dir = push_bit(stack.stack, stack.size);
	if (dir == 0) {
		printf("0");
		return f_test(curr->data.i->left, stack, com_s, fp, fp_w);
	}
	printf("1");
	return f_test(curr->data.i->right, stack, com_s, fp, fp_w);
}

int main(int argc, char* argv[]) {
	bool input_valid = false;
	if ((input_valid = (argc == 2))) {
		FILE * fp = fopen(argv[1], "r");
		NEW_FILE(nname, argv[1], ".txt")
		FILE * fp_w = fopen(nname, "w");
		header_t * h = malloc(sizeof(*h));
		size_t com_size, h_size, dec_size;
		fread(h, (com_size = sizeof(h->compressed_size)) + 
		         (h_size = sizeof(h->header_size)) + 
				 (dec_size = sizeof(h->decompressed_size)), 1, fp);
		// printf("%ld %ld %ld\n", h->compressed_size, h->header_size, h->decompressed_size);
		int pair_size = 2 * sizeof(char) + sizeof(int);
		int num_pair  = (h->header_size -  com_size - h_size - dec_size) / pair_size;
		// printf("num pair: %d\n", num_pair);
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
			// printf("l: %c, r: %c, w: %d\n", l, r, w);
		}
		// tree_fun(head);
		uint64_t stack;
		int size = 0;
		// printf("com size: %ld\n", com_size);
		v_test(head, head, (bit_t) {.stack = &stack, .size = &size}, h->decompressed_size, fp, fp_w);
		fclose(fp);
		free(h);
		free_tree(&head);
		printf("valid input");

		fclose(fp_w);
		free(nname);
	}
	return input_valid ? EXIT_SUCCESS: EXIT_FAILURE;
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
