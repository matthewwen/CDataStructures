#ifndef _HUFF_H
#define _HUFF_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#define NUM_CHAR 256

typedef union {
    struct{
        struct node_t * left;
        struct node_t * right;
    }node;
    struct {
        char value;
        int occurance;
    }value;
}data_t;

typedef struct node_t{
    enum {VALUE, NODE} type;
    data_t data;
}node_t;

typedef struct{
    uint32_t compressed_size;
    uint32_t header_size;
    uint32_t decompressed_size;
    node_t   nodes[NUM_CHAR];
}header_t;

#endif
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */