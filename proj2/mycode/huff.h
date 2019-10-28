#ifndef _HUFF_H
#define _HUFF_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#define NUM_CHAR 256
#define MAX_BIT  (sizeof(uint64_t) * 8)

typedef struct{
    unsigned char value;
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
    uint64_t compressed_size;
    uint64_t header_size;
    uint64_t decompressed_size;
    value_t  values[NUM_CHAR];
    value_t * sorted[NUM_CHAR];
    int buffer_end;
}header_t;

typedef struct{
	uint8_t * stack;
	int * size;
}bit_t;

// Function Definition
#define APPEND_TREE() \
void append_tree(node_t ** a_node, i_t * i_node); \
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
			/* allocate memory*/\
			cp_i     = malloc(sizeof(*cp_i));\
			n_node   = malloc(sizeof(*n_node));\
\
			/* set up new intersection*/\
			cp_i->weight = (og_w = cp_node->data.i->weight) + (n_w = i_node->weight);\
\
			/* create node for i_node*/\
			*n_node  = (node_t) {.type = NODE, .is_leaf = true, .data = (data_t) {.i = i_node}};\
\
			/* determine left / right*/\
			cp_i->left  = (is_left = og_w < n_w) ? *a_node: n_node;\
			cp_i->right = is_left                ?  n_node: *a_node;\
			\
			/* reset copy node*/\
			cp_node  = malloc(sizeof(*cp_node));\
			*cp_node = (node_t) {.type = NODE, .is_leaf = false, .data = (data_t) {.i = cp_i}};\
			*a_node  = cp_node;\
	}\
}

#define FREE_TREE() \
void free_tree(node_t ** head); \
void free_tree(node_t ** head) {\
	node_t * cp_head = *head;\
	if (cp_head != NULL) {\
		data_t * cp_data = &(*head)->data;\
		if (cp_head->type == NODE) {\
			free_tree(&cp_data->i->left);\
			free_tree(&cp_data->i->right);\
			free(cp_data->i);\
		}\
		free(cp_head);\
		*head = NULL;\
	}\
}

#ifdef _HUFF_C
#define P_BIT() \
int push_bit(uint64_t stack[2], int * size) {\
	bool most_sig = (~stack[1]) < (stack[1]);\
	stack[1] = stack[1] << 1;\
	stack[1] += (~stack[0]) < (stack[0]) ? 1: 0;\
	stack[0] = stack[0] << 1;\
	*size -= 1;\
	return most_sig ? 1: 0;\
}
#endif

#ifdef _UNHUFF_C
#define P_BIT() \
int push_bit(uint8_t * stack, int * size) {\
	bool most_sig = (~(*stack)) < (*stack);\
	(*stack) = (*stack) << 1;\
	*size -= 1;\
	return most_sig ? 1: 0;\
}
#endif

#define PUSH_BIT(VAR, STACK, SIZE)\
do {\
	uint8_t cpy = *STACK;\
	uint8_t ccpy = ~cpy;\
	bool most_sig = ccpy < (cpy);\
	(*STACK) = cpy << 1;\
	*SIZE -= 1;\
	VAR =  most_sig;\
}while(false);



#define APPEND_BIT() \
void append(uint8_t * stack, int num) {\
	*stack = *stack << 1;\
	*stack += num;\
}\

#define NEW_FILE(VNAME, ARG, END) \
	char * VNAME;\
	do {\
		VNAME = malloc((strlen(ARG) + strlen(END) + 1) * sizeof(*VNAME));\
		strcpy(VNAME, ARG);\
		strcpy(&VNAME[strlen(ARG)], END);\
	} \
	while(false);

#endif
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
