#ifndef _HUFF_H
#define _HUFF_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#define NUM_CHAR 256
#define MAX_BIT  (sizeof(uint64_t) * 8)


#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c""%c%c%c%c%c%c%c%c"\
"%c%c%c%c%c%c%c%c""%c%c%c%c%c%c%c%c"\
"%c%c%c%c%c%c%c%c""%c%c%c%c%c%c%c%c"\
"%c%c%c%c%c%c%c%c""%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & base(63) ? '1' : '0') ,\
  (byte & base(62) ? '1' : '0') ,\
  (byte & base(61) ? '1' : '0') ,\
  (byte & base(60) ? '1' : '0') ,\
  (byte & base(59) ? '1' : '0') ,\
  (byte & base(58) ? '1' : '0') ,\
  (byte & base(57) ? '1' : '0') ,\
  (byte & base(56) ? '1' : '0') ,\
  (byte & base(55) ? '1' : '0') ,\
  (byte & base(54) ? '1' : '0') ,\
  (byte & base(53) ? '1' : '0') ,\
  (byte & base(52) ? '1' : '0') ,\
  (byte & base(51) ? '1' : '0') ,\
  (byte & base(50) ? '1' : '0') ,\
  (byte & base(49) ? '1' : '0') ,\
  (byte & base(48) ? '1' : '0') ,\
  (byte & base(47) ? '1' : '0') ,\
  (byte & base(46) ? '1' : '0') ,\
  (byte & base(45) ? '1' : '0') ,\
  (byte & base(44) ? '1' : '0') ,\
  (byte & base(43) ? '1' : '0') ,\
  (byte & base(42) ? '1' : '0') ,\
  (byte & base(41) ? '1' : '0') ,\
  (byte & base(40) ? '1' : '0') ,\
  (byte & base(39) ? '1' : '0') ,\
  (byte & base(38) ? '1' : '0') ,\
  (byte & base(37) ? '1' : '0') ,\
  (byte & base(36) ? '1' : '0') ,\
  (byte & base(35) ? '1' : '0') ,\
  (byte & base(34) ? '1' : '0') ,\
  (byte & base(33) ? '1' : '0') ,\
  (byte & base(32) ? '1' : '0') ,\
  (byte & base(31) ? '1' : '0') ,\
  (byte & base(30) ? '1' : '0') ,\
  (byte & base(29) ? '1' : '0') ,\
  (byte & base(28) ? '1' : '0') ,\
  (byte & base(27) ? '1' : '0') ,\
  (byte & base(26) ? '1' : '0') ,\
  (byte & base(25) ? '1' : '0') ,\
  (byte & base(24) ? '1' : '0') ,\
  (byte & base(23) ? '1' : '0') ,\
  (byte & base(22) ? '1' : '0') ,\
  (byte & base(21) ? '1' : '0') ,\
  (byte & base(20) ? '1' : '0') ,\
  (byte & base(19) ? '1' : '0') ,\
  (byte & base(18) ? '1' : '0') ,\
  (byte & base(17) ? '1' : '0') ,\
  (byte & base(16) ? '1' : '0') ,\
  (byte & base(15) ? '1' : '0') ,\
  (byte & base(14) ? '1' : '0') ,\
  (byte & base(13) ? '1' : '0') ,\
  (byte & base(12) ? '1' : '0') ,\
  (byte & base(11) ? '1' : '0') ,\
  (byte & base(10) ? '1' : '0') ,\
  (byte & base(9) ? '1' : '0') ,\
  (byte & base(8) ? '1' : '0') ,\
  (byte & base(7) ? '1' : '0'), \
  (byte & base(6) ? '1' : '0'), \
  (byte & base(5) ? '1' : '0'), \
  (byte & base(4) ? '1' : '0'), \
  (byte & base(3) ? '1' : '0'), \
  (byte & base(2) ? '1' : '0'), \
  (byte & base(1) ? '1' : '0'), \
  (byte & base(0) ? '1' : '0') 

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
	uint64_t * stack;
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
#define PUSH_BIT() \
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
#define PUSH_BIT() \
int push_bit(uint64_t * stack, int * size) {\
	bool most_sig = (~(*stack)) < (*stack);\
	(*stack) = (*stack) << 1;\
	*size -= 1;\
	return most_sig ? 1: 0;\
}
#endif

#ifdef _HUFF_C
#define PUSH_FRONT()\
void push_front(uint64_t stack[2], int start) {\
	int max_bit = MAX_BIT;\
	for (;start < (max_bit * 2); start++) {\
		stack[1] = stack[1] << 1;\
		stack[1] += (~stack[0]) < (stack[0]) ? 1: 0;\
		stack[0] = stack[0] << 1;\
	}\
}
#endif

#ifdef _UNHUFF_C
#define PUSH_FRONT()\
void push_front(uint64_t * stack, int start) {\
	int max_bit = MAX_BIT;\
    *stack = (*stack) << (max_bit - start - 1);\
}
#endif


#define APPEND_BIT() \
void append(uint64_t * stack, int num) {\
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
