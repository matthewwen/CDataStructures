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
    uint32_t weight;
    uint64_t loc;
    uint8_t numbit;
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
    uint32_t decompressed_size;
    value_t  values[NUM_CHAR];
	node_t * order[NUM_CHAR];
    int buffer_end;
}header_t;

typedef struct{
	uint8_t * stack;
	uint8_t * size;
}bit_t;

// Function Definition
#ifdef _UNHUFF_C
#define FREE_TREE() \
		UPPER_FREE_TREE()\
		INT_FREE_TREE()\
		VAL_FREE_TREE() \
		LOWER_FREE_TREE()
#endif  

#ifdef _HUFF_C
#define FREE_TREE() \
		UPPER_FREE_TREE()\
		LOWER_FREE_TREE()
#endif

#define UPPER_FREE_TREE() \
void free_tree(node_t ** head); \
void free_tree(node_t ** head) {\
	node_t * cp_head = *head;\
	if (cp_head != NULL) {\
		data_t * cp_data = &(*head)->data;\
		if (cp_head->type == NODE) {\
			free_tree(&cp_data->i->left);\
			free_tree(&cp_data->i->right);
#define INT_FREE_TREE()\
			free(cp_data->i);
#define VAL_FREE_TREE()\
		}\
		else if (cp_head->type == VALUE) {\
			free((*head)->data.value);
#define LOWER_FREE_TREE() \
		}\
		free(cp_head);\
		*head = NULL;\
	}\
}

#define PUSH_BIT(VAR, STACK, SIZE)\
do {\
	uint8_t ccpy = ~(STACK);\
	VAR = (ccpy) < (STACK);\
	STACK = (STACK) << 1;\
	SIZE -= 1;\
}while(false);

#define PUSH_FRONT(STACK, START)\
do {\
	STACK = (STACK) << ((sizeof(STACK) * 8) - (START));\
}while(false);\


#define APPEND_BIT(STACK, NUM) \
do {\
	(STACK) = (STACK) << 1;\
	(STACK) += NUM;\
}while(false);

#define NEW_FILE(VNAME, ARG, END) \
	char * VNAME = NULL;\
	do {\
		VNAME = malloc((strlen(ARG) + strlen(END) + 1) * sizeof(*VNAME));\
		strcpy(VNAME, ARG);\
		strcpy(&VNAME[strlen(ARG)], END);\
	} \
	while(false);

#endif
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
