#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "huff.h"

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

void append_tree(node_t ** a_node, value_t * value) {
	// create new node
	node_t * l_node = malloc(sizeof(*l_node));
	l_node->type    = VALUE;
	l_node->data    = (data_t) {.value = value};
	
	// copy address of a_node
	node_t * cp_node = *a_node;
	value_t * cp_value;
	i_t     * cp_i;

	if (cp_node == NULL) {
		// create a new node
		(*a_node) = l_node;
	}
	else if (cp_node->type == VALUE && (cp_value = cp_node->data.value)) {
		// split node into an intersection
		node_t * i_node = malloc(sizeof(*i_node));
		i_node->type    = NODE;
		int cp_weight, curr_weight;
		i_node->data.intrsect.weight = (cp_weight = cp_value->weight) + (curr_weight = value->weight);
		bool left_cp = cp_weight > curr_weight;
		i_node->data.intrsect.left  = left_cp ? cp_node: l_node;
		i_node->data.intrsect.right = left_cp ? l_node : cp_node;
		*a_node = i_node;
	}
	else if (cp_node->type == NODE && (cp_i = &cp_node->data.intrsect)) {
		// recursively go left or right
		if (cp_i->weight < value->weight) {
			append_tree(&cp_i->left, value);
		}
		else {
			append_tree(&cp_i->right, value);
		}
	}
}

int main(int argc, char* argv[]) {
	bool input_valid = false;
	int i;
	node_t * head = NULL;
		
	if ((input_valid = (argc == 2))) {
		FILE * fp = fopen("example.txt", "r");
		header_t * h = create_table(fp);
		for (i = 0; i < NUM_CHAR; i++) {
			value_t * v = &h->values[i];
			if (v->weight > 0) {
				append_tree(&head, v);
			}
		}
		free(h);
		fclose(fp);
	}
	return input_valid ? EXIT_SUCCESS: EXIT_FAILURE;
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
