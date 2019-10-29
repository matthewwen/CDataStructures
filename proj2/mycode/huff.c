#define _HUFF_C
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "huff.h"

FREE_TREE()

node_t * append_tree(node_t * n1, node_t * n2) {
	/* allocate memory*/
	i_t * n_i;
	node_t * n_node = malloc(sizeof(*n_node) + sizeof(*n_i));
	n_i             = (i_t *) (n_node + 1);

	/* set up new intersection and left / right*/
	int og_w, n_w;
	n_i->weight = (og_w = (n1->type == VALUE) ? n1->data.value->weight : n1->data.i->weight) + 
				  (n_w  = (n2->type == VALUE) ? n2->data.value->weight : n2->data.i->weight);
	bool is_left;
	n_i->left   = (is_left = og_w < n_w) ?  n1: n2;
	n_i->right  = is_left                ?  n2: n1;

	/* reset copy node */
	*n_node = (node_t) {.type = NODE, .is_leaf = true, .data = (data_t) {.i = n_i}};

	return n_node;
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

node_t * f_test(header_t * head);
void assign_loc(node_t * head, uint64_t loc, int size);
node_t * f_test(header_t * head) {
	// perform insertion, sort in assending order
	node_t * order_list[NUM_CHAR];
	int i, j, last_pos = 0;

	value_t * pos;
	node_t * n_node;
	for (i = 0; i < NUM_CHAR; i++) {
		if ((pos = &head->values[i])->weight != 0) {
			pos->value = i;
			n_node     = malloc(sizeof(*n_node));
			*n_node    = (node_t) {.type = VALUE, .data = (data_t) {.value = pos}};
			for (j = last_pos; j > 0 && order_list[j - 1]->data.value->weight < pos->weight; j--) {
				order_list[j] = order_list[j - 1];
			}
			order_list[j] = n_node;
			last_pos++;
		}
	}
	// saving inside of header
	head->buffer_end = last_pos;

	// add element into binary tree
	node_t * holder;
	for (;last_pos > 1; last_pos--) {
		n_node = append_tree(order_list[last_pos - 1], order_list[last_pos - 2]);
		order_list[last_pos - 1] = NULL;
		order_list[last_pos - 2] = NULL;
		for (j = last_pos - 2; j > 0 && ((holder = order_list[j - 1])->type == VALUE ?
							              holder->data.value->weight: holder->data.i->weight) < n_node->data.i->weight; j--) {
			order_list[j] = order_list[j - 1];
		}
		order_list[j] = n_node;
	}

	assign_loc(order_list[0], 0, 0);
	return order_list[0];
}

void assign_loc(node_t * head, uint64_t loc, int size){
	if (head != NULL && head->type == NODE) {
		uint64_t left = loc << 1, right = left + 1;
		
		assign_loc(head->data.i->left, left, size + 1);
		assign_loc(head->data.i->right, right, size + 1);
	}
	else if (head != NULL && head->type == VALUE) {
		head->data.value->loc = loc;
		head->data.value->numbit = size;
	}
}

// redit this entire function
void write_header(node_t * head, uint8_t * stack, uint8_t * size, FILE * fp);
void add_stack(uint8_t * s, uint8_t * s_size, uint8_t e, uint8_t e_size, FILE * fp);
void write_header(node_t * head, uint8_t * stack, uint8_t * size, FILE * fp) {
	if (head == NULL) {
		return;
	}
	else if (head->type == NODE) {
		// tell program to go left
		add_stack(stack, size, 0, 1, fp);
		write_header(head->data.i->left, stack, size, fp);
		write_header(head->data.i->right, stack, size, fp);
	}
	else {
		// found a node
		value_t * v = head->data.value;
		add_stack(stack, size, 1, 1, fp);
		add_stack(stack, size, v->value, 8, fp);
	}
}

void add_stack(uint8_t * s, uint8_t * s_size, uint8_t e, uint8_t e_size, FILE * fp) {
	PUSH_FRONT(e, e_size);
	for (;e_size > 0; ) {
		if ((*s_size) ==  8) {
			fwrite(s, sizeof(*s), 1, fp);
			(*s) = 0;
			(*s_size) = 0;
		}
		uint8_t push;
		PUSH_BIT(push, e, e_size);
		APPEND_BIT(*s, push);
		(*s_size) = (*s_size) + 1;
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
		          (h_size   = sizeof(h->header_size)) + 
				  (dec_size = sizeof(h->decompressed_size)), 1, write_fp);
		uint8_t stack = 0;
		uint8_t size  = 0;
		write_header(nhead, &stack, &size, write_fp);
		PUSH_FRONT(stack, size);
		fwrite(&stack, sizeof(stack), 1, write_fp);
		h->header_size = ftell(write_fp);

		// write data into file
		int idx; 
		uint8_t buffer = 0;
		uint8_t buffer_size = 0;
		fseek(fp, 0, SEEK_SET);

		for (;(idx = fgetc(fp)) != EOF;) {
			value_t value = h->values[idx];
			add_stack(&buffer, &buffer_size, value.loc, value.numbit, write_fp);
		}
		PUSH_FRONT(buffer, buffer_size);
		fwrite(&buffer, sizeof(buffer), 1, write_fp);

		h->compressed_size = ftell(write_fp);
		free_tree(&nhead);

		// edit the header
		fseek(write_fp, 0L, SEEK_SET);
		fwrite(&h->compressed_size, com_size, 1, write_fp);
		fwrite(&h->header_size, h_size, 1, write_fp);
		fwrite(&h->decompressed_size, dec_size, 1, write_fp);

		// clean up
		fclose(write_fp);
		fclose(fp);
		free(h);
		free(n_name);

	}
	return input_valid ? EXIT_SUCCESS: EXIT_FAILURE;
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
