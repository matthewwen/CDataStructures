#ifndef _HUFF_H
#define _HUFF_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#define NUM_CHAR 256


typedef struct{
    char  value;
    int   weight;
}value_t;

typedef struct i_t{
    int weight;
    struct i_t * left;
    struct i_t * right;
}i_t;

typedef union{
    i_t  intrsect;
    value_t value;
}data_t;

typedef struct{
    enum {VALUE, NODE} type;
    data_t data;
}node_t;

typedef struct{
    uint32_t compressed_size;
    uint32_t header_size;
    uint32_t decompressed_size;
    value_t  values[NUM_CHAR];
}header_t;

#endif
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */