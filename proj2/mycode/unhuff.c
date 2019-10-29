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

	fread(s, sizeof(*s), 1, fp_r);
	*s_size = 8 * sizeof(*s_size);

	for (;cs < 8; cs++) {
		PUSH_BIT(i, *s, *s_size);
		APPEND_BIT(c, i);
	}
	printf("%c\n", c);

	return c;
}

node_t * create_table(uint8_t * s, uint8_t * s_size, FILE * fp_r) {
	if ((*s_size) == 0) {
		printf("read file\n");
		fread(s, sizeof(*s), 1, fp_r);
		*s_size = sizeof(*s) * 8;
	}
	int i;
	PUSH_BIT(i, *s, *s_size);
	printf("68 update\n");
	node_t * n_node = malloc(sizeof(*n_node));
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
		printf("s: %c\n", v_node->value);
		n_node->data.value = v_node;
	}
	return n_node;
}

void print_btree(node_t * head) {
	if (head->type == NODE){
		printf("left\n");
		print_btree(head->data.i->left);
		printf("back left\n");
		printf("right\n");
		print_btree(head->data.i->right);
		printf("back right\n");
	}
	else {
		printf("c:%c\n", head->data.value->value);
	}
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
		         (h_size   = sizeof(h->header_size)) + 
				 (dec_size = sizeof(h->decompressed_size)), 1, fp);
				
		// change
		uint8_t s = 0, s_size = 0;
		node_t * head = create_table(&s, &s_size, fp);
		printf("location: %d, expected: %d\n", ftell(fp), h->header_size);
		//print_btree(head);

		// run data
		s = 0;
		s_size = 0;
		v_test(head, (bit_t) {.stack = &s, .size = &s_size}, h->decompressed_size, fp, fp_w);
		fclose(fp);
		free(h);
		free_tree(&head);

		fclose(fp_w);
		free(nname);
	}
	return input_valid ? EXIT_SUCCESS: EXIT_FAILURE;
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */