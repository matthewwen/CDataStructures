#ifndef _HUFF_H
#define _HUFF_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#define NUM_CHAR 256
#define SIG_BIT 

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

#endif
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */