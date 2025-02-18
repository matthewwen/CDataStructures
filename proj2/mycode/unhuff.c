#define _UNHUFF_C
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "huff.h"

FREE_TREE()

bool f_test(node_t * curr, bit_t stack, uint64_t * com_s, FILE * fp, FILE * fp_w);
void v_test(node_t * head, uint64_t com_s, FILE * fp, FILE * fp_w) {
	uint8_t stack = 0, size = 0;
	bool alloc_more;
	for (alloc_more = true; alloc_more;) {
		alloc_more = f_test(head, (bit_t) {.stack = &stack, .size = &size}, &com_s, fp, fp_w);
	}
}

bool f_test(node_t * curr, bit_t stack, uint64_t * com_s, FILE * fp, FILE * fp_w) {
	if (curr->type == VALUE) {
		fputc(curr->data.value->value, fp_w);
		*com_s =(*com_s) - 1;
		return (*com_s) > 0;
	}
	if (*stack.size == 0) {
		if ((*com_s) <= 0) {
			return false;
		}
		if (fread(stack.stack, sizeof(*stack.stack), 1, fp) == 0){
			return false;	
		}
		*stack.size = 8 * sizeof(*stack.stack);
	}
	int dir;
	PUSH_BIT(dir, *stack.stack, *stack.size);
	if (dir == 0) {
		return f_test(curr->data.i->left, stack, com_s, fp, fp_w);
	}
	return f_test(curr->data.i->right, stack, com_s, fp, fp_w);
}

uint8_t get_char(uint8_t * s, uint8_t * s_size, FILE * fp_r) {
	uint8_t c = 0, cs = 0;
	int i;
	for (; (*s_size) > 0; cs++) {
		PUSH_BIT(i, *s, *s_size);
		APPEND_BIT(c, i);
	}

	if (fread(s, sizeof(*s), 1, fp_r) == 0) {
		return 0;
	}
	*s_size = 8 * sizeof(*s_size);

	for (;cs < 8; cs++) {
		PUSH_BIT(i, *s, *s_size);
		APPEND_BIT(c, i);
	}

	return c;
}

node_t * create_table(uint8_t * s, uint8_t * s_size, FILE * fp_r) {
	if ((*s_size) == 0) {
		if (fread(s, sizeof(*s), 1, fp_r) == 0) {
			return NULL;
		}
		*s_size = sizeof(*s) * 8;
	}
	int i;
	PUSH_BIT(i, *s, *s_size);
	node_t * n_node = malloc(sizeof(*n_node));
	if (n_node == NULL) {
		return NULL;
	}
	*n_node = (node_t) {.type = (i == 1 ? VALUE: NODE)};
	if (i == 0) {
		i_t * i_node = malloc(sizeof(*i_node));
		*i_node      = (i_t) {.left  = create_table(s, s_size, fp_r), 
							  .right = create_table(s, s_size, fp_r)};
		n_node->data.i = i_node;
	}
	else {
		value_t * v_node = malloc(sizeof(*v_node));
		*v_node = (value_t) {.value = get_char(s, s_size, fp_r)};
		n_node->data.value = v_node;
	}
	return n_node;
}

int main(int argc, char* argv[]) {
	bool input_valid = false;
	if ((input_valid = (argc == 2))) {
		FILE * fp = fopen(argv[1], "r");

		// check if it is my huffman
		char is_huff[5];
		if (strlen(argv[1]) < 4) {
			return EXIT_FAILURE;
		}
		memcpy(is_huff, (argv[1] + strlen(argv[1]) - 4), sizeof(is_huff));
		if (strcmp(is_huff, "huff") != 0) {
			return EXIT_FAILURE;
		}
		
		// set up new information about file
		NEW_FILE(nname, argv[1], ".unhuff")
		if (nname == NULL) {
			return EXIT_FAILURE;
		}
		FILE * fp_w = fopen(nname, "w");
		header_t * h = malloc(sizeof(*h));
		if (fp_w == NULL || h == NULL)  {
			return EXIT_FAILURE;
		}
		size_t dec_size;

		if (fread(&h->decompressed_size, (dec_size = sizeof(h->decompressed_size)), 1, fp) == 0) {
			return EXIT_FAILURE;
		}
				
		// change
		uint8_t s = 0, s_size = 0;
		node_t * head = create_table(&s, &s_size, fp);
		if (head == NULL) {
			return EXIT_FAILURE;
		}

		// run data
		v_test(head, h->decompressed_size, fp, fp_w);
		fclose(fp);
		free(h);
		free_tree(&head);

		fclose(fp_w);
		free(nname);
	}
	return input_valid ? EXIT_SUCCESS: EXIT_FAILURE;
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */