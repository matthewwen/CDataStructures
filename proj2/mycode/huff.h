#ifndef _HUFF_H
#define _HUFF_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#define NUM_CHAR 256
#define MAX_BIT  (sizeof(uint64_t) * 8)

typedef struct{
    char  value;
    int   weight;
    uint64_t loc[2];
    int numbit;
}value_t;

struct node_t;

typedef struct i_t{
    int weight;
    struct node_t * left;
    struct node_t * right;
}i_t;

typedef union{
    i_t * i;
    value_t * value;
}data_t;

typedef struct node_t{
    enum {VALUE, NODE} type;
    bool is_leaf;
    data_t data;
}node_t;

typedef struct{
    uint32_t compressed_size;
    uint32_t header_size;
    uint32_t decompressed_size;
    value_t  values[NUM_CHAR];
    value_t * sorted[NUM_CHAR];
    int buffer_end;
}header_t;

// Function Definition
#define APPEND() \
void append_tree(node_t ** a_node, i_t * i_node) {\
	node_t * n_node, * cp_node = *a_node;\
	i_t    * cp_i;\
\
	int og_w, n_w;\
	bool is_left;\
\
	if (cp_node == NULL) {\
		cp_node  = malloc(sizeof(*cp_node));\
		*cp_node = (node_t) {.type = NODE, .is_leaf = true, .data = (data_t) {.i = i_node}};\
		*a_node  = cp_node;\
	}\
	else if (cp_node->type == NODE) {\
		if (cp_node->is_leaf) {\
			/* allocate memory*/\
			cp_i     = malloc(sizeof(*cp_i));\
			n_node   = malloc(sizeof(*n_node));\
\
			/* set up new intersection*/\
			*cp_i    = (i_t) {.weight = (og_w = cp_node->data.i->weight) + (n_w = i_node->weight)};\
\
			/* create node for i_node*/\
			*n_node  = (node_t) {.type = NODE, .is_leaf = true, .data = (data_t) {.i = i_node}};\
\
			/* determine left / right*/\
			cp_i->left  = (is_left = og_w > n_w) ? *a_node: n_node;\
			cp_i->right = is_left                ?  n_node: *a_node;\
			\
			/* reset copy node*/\
			cp_node  = malloc(sizeof(*cp_node));\
			*cp_node = (node_t) {.type = NODE, .is_leaf = false, .data = (data_t) {.i = cp_i}};\
			*a_node  = cp_node;\
		}\
		else {\
			if (cp_node->data.i->weight < i_node->weight) {\
				append_tree(&(*a_node)->data.i->left, i_node);\
			}\
			else {\
				append_tree(&(*a_node)->data.i->right, i_node);\
\
			}\
			/* update the weight*/\
			(*a_node)->data.i->weight += i_node->weight;\
\
			/* flip node of left occurance is greater than right*/\
			if ((*a_node)->is_leaf == false) {\
				cp_i = (*a_node)->data.i;\
				if (cp_i->left->data.i->weight > cp_i->right->data.i->weight) {\
					node_t * holder = cp_i->right;\
					cp_i->right = cp_i->left;\
					cp_i->left = holder;\
				}\
			}\
		}\
	}\
}\

#endif
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
