#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "huff.h"

node_t * f_test(header_t * head);
void free_tree(node_t ** head);
void assign_loc(node_t * head, uint64_t loc[2], int size);

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
			// update the weight
			(*a_node)->data.i->weight += i_node->weight;

			// flip node of left occurance is greater than right
			if ((*a_node)->is_leaf == false) {
				cp_i = (*a_node)->data.i;
				if (cp_i->left->data.i->weight > cp_i->right->data.i->weight) {
					node_t * holder = cp_i->right;
					cp_i->right = cp_i->left;
					cp_i->left = holder;
				}
			}
		}
	}
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
	for (i = 0; i < last_pos - 2; i += 2) {
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
	if (i > last_pos - 2) {
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

void free_tree(node_t ** head) {
	node_t * cp_head = *head;
	if (cp_head != NULL) {
		data_t * cp_data = &(*head)->data;
		if (cp_head->type == NODE) {
			free_tree(&cp_data->i->left);
			free_tree(&cp_data->i->right);
			free(cp_data->i);
		}
		free(cp_head);
		*head = NULL;
	}
}

void push_front(uint64_t stack[2], int start) {
	int max_bit = MAX_BIT;
	for (;start < (max_bit * 2); start++) {
		stack[1] = stack[1] << 1;
		stack[1] += (~stack[0]) < (stack[0]) ? 1: 0;
		stack[0] = stack[0] << 1;
	}
}

int push_bit(uint64_t stack[2], int * size) {
	bool most_sig = (~stack[1]) < (stack[1]);
	stack[1] = stack[1] << 1;
	stack[1] += (~stack[0]) < (stack[0]) ? 1: 0;
	stack[0] = stack[0] << 1;
	*size -= 1;
	return most_sig ? 1: 0;
}

void append(uint64_t * stack, int num) {
	*stack = *stack << 1;	
	*stack += num;
}

void assign_loc(node_t * head, uint64_t loc[2], int size){
	if (head != NULL && head->type == NODE) {
		uint64_t left[2], right[2];

		memcpy(left, loc, sizeof(left));
		memcpy(right, loc, sizeof(right));

		// shift bytes to the left by 1
		left[1]  = left[1] << 1;
		right[1] = right[1] << 1;

		// push elements if necessary
		left[1] += ~left[0] < left[0] ? 1: 0;
		right[1] += ~right[0] < right[0] ? 1: 0;

		// add element to bottom
		right[0] += 1;
		
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
	for (i = 0; i < h->buffer_end - 2; i+=2) {
		fputc((v1 = h->sorted[i])->value, fp);
		fputc((v2 = h->sorted[i + 1])->value, fp);
		printf("%c, %c\n", v1->value, v2->value);
		w = v1->weight + v2->weight;
		fwrite(&w, sizeof(w), 1, fp);
	}

	if (i > (h->buffer_end - 2)) {
		fputc((v1 = h->sorted[h->buffer_end - 1])->value, fp);
		fputc(0, fp);
		w = v1->weight;
		fwrite(&w, sizeof(w), 1, fp);
	}
}

int main(int argc, char* argv[]) {
	bool input_valid = false;
	node_t * nhead   = NULL;

	FILE * bb = fopen("bb.txt", "w");
	int val = 'a';
	fputc(val, bb);
	fclose(bb);

	if ((input_valid = (argc == 2))) {
		// count occurance
		FILE * fp = fopen(argv[1], "r");
		header_t * h = create_table(fp);
		h->decompressed_size = ftell(fp);
		fseek(fp, 0L, SEEK_SET);

		// create tree, and table
		nhead = f_test(h);

		// create new file
		char * end    = ".huff";
		char * n_name = malloc((strlen(argv[1]) + strlen(end) + 1) * sizeof(*n_name));
		strcpy(n_name, argv[1]);
		strcpy(&n_name[strlen(argv[1])], end);
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
		uint64_t buffer = 0;
		uint64_t idx_loc[2];
		int buffer_size = 0;
		for (;(idx = fgetc(fp)) != EOF;) {
			value_t value = h->values[idx];
			int size = value.numbit;
			memcpy(idx_loc, value.loc, sizeof(idx_loc));
			for (; size > 0; ) {
				if (buffer_size == MAX_BIT) {
					fwrite(&buffer, sizeof(buffer), 1, write_fp);
					buffer = 0, buffer_size = 0;
				}
				append(&buffer, push_bit(idx_loc, &size));
				buffer_size++;
			}
			fwrite(&buffer, sizeof(buffer), 1, write_fp);
		}
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