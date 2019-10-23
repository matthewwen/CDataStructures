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
	node_t * n_node, * cp_node = *a_node;
	i_t    * cp_i;

	int og_w, n_w;
	bool is_left;

	if (cp_node == NULL) {
		cp_node  = malloc(sizeof(*cp_node));
		*cp_node = (node_t) {.type = NODE, .is_leaf = true, .data = (data_t) {.i = i_node}};
		*a_node  = cp_node;
	}
	else if (cp_node->type == NODE) {
		if (cp_node->is_leaf) {
			// allocate memory
			cp_i     = malloc(sizeof(*cp_i));
			n_node   = malloc(sizeof(*n_node));

			// set up new intersection
			*cp_i    = (i_t) {.weight = (og_w = cp_node->data.i->weight) + (n_w = i_node->weight)};

			// create node for i_node
			*n_node  = (node_t) {.type = NODE, .is_leaf = true, .data = (data_t) {.i = i_node}};

			// determine left / right
			cp_i->left  = (is_left = og_w > n_w) ? *a_node: n_node;
			cp_i->right = is_left                ?  n_node: *a_node;
			
			// reset copy node
			cp_node  = malloc(sizeof(*cp_node));
			*cp_node = (node_t) {.type = NODE, .is_leaf = false, .data = (data_t) {.i = cp_i}};
			*a_node  = cp_node;
		}
		else {
			if (cp_node->data.i->weight < i_node->weight) {
				append_tree(&(*a_node)->data.i->left, i_node);
			}
			else {
				append_tree(&(*a_node)->data.i->right, i_node);

			}
			(*a_node)->data.i->weight += i_node->weight;
		}
	}
}

void f_test(header_t * head) {
	// perform insertion, sort in decending order
	value_t * order_list[NUM_CHAR];
	int i, j, last_pos = 0;
	node_t * nhead = NULL;

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
		i_node->left   = left;
		i_node->right  = right;
		append_tree(&nhead, i_node);
	}

}

int main(int argc, char* argv[]) {
	bool input_valid = false;
		
	if ((input_valid = (argc == 2))) {
		FILE * fp = fopen(argv[1], "r");
		header_t * h = create_table(fp);
		f_test(h);
		free(h);
		fclose(fp);
	}
	return input_valid ? EXIT_SUCCESS: EXIT_FAILURE;
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
