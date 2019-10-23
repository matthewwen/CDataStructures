#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "huff.h"

void f_test(header_t * head);

header_t * create_table(FILE * fp) {
	size_t memory_size;
	header_t * header = malloc((memory_size = sizeof(*header)));
	memset(header, 0, memory_size);

	int i;
	fseek(fp, 0L, SEEK_SET);
	while ((i = fgetc(fp)) != EOF) {
		header->values[i].weight += 1;
	}

	return header;
}

void append_tree(node_t ** a_node, i_t * i_node) {
	node_t * cp_node = &a_node;

	if (cp_node == NULL) {
		cp_node  = malloc(sizeof(*cp_node));
		*cp_node = (node_t) {.type = NODE, .data = (data_t) {.i = i_node}};
		*a_node  = &cp_node;
	}
	else if (cp_node->type == NODE) {
		if (cp_node->data.i->weight < i_node->weight) {
			append_tree(&(*a_node)->data.i->left, i_node);
		}
		else {
			append_tree(&(*a_node)->data.i->right, i_node);

		}
		(*a_node)->data.i->weight += i_node->weight;
	}
}

void f_test(header_t * head) {
	// perform insertion, sort in decending order
	value_t * order_list[NUM_CHAR];
	int last_pos = 0;
	int i;
	int j;

	value_t * pos;
	for (i = 0; i < NUM_CHAR; i++) {
		if ((pos = &head->values[i])->weight != 0) {
			pos->value = i;
			for (j = last_pos; j > 0 && order_list[j - 1]->weight < pos->weight; j--) {
				order_list[j] = order_list[j - 1];
			}
			order_list[j] = pos;
			last_pos++;
		}
	}

	for (i = 0; i < last_pos; i += 2) {
		i_t    * i_node = malloc(sizeof(*i_node));
		node_t * left   = malloc(sizeof(*left));
		node_t * right  = malloc(sizeof(*right));

		*left  = (node_t) {.type = VALUE, .data = (data_t) {.value = order_list[i]}};
		*right = (node_t) {.type = VALUE, .data = (data_t) {.value = order_list[i + 1]}};
		i_node->weight = order_list[i]->weight + order_list[i + 1]->weight;
	}

}

int main(int argc, char* argv[]) {
	bool input_valid = false;
		
	if ((input_valid = (argc == 2))) {
		FILE * fp = fopen("example.txt", "r");
		header_t * h = create_table(fp);
		f_test(h);
		free(h);
		fclose(fp);
	}
	return input_valid ? EXIT_SUCCESS: EXIT_FAILURE;
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
