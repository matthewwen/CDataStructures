#ifndef _HEADER_H_
#define _HEADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

// struct llong_t{
//     int idx;
//     struct llong_t * next;
// };
// typedef struct llong_t llong_t;
struct llong_t{
    int * idx;
    int i;
    int size;
};
typedef struct llong_t llong_t;

typedef struct{
    int x;
    int y;
    llong_t adj_head; // Linked list
    int distance; // dijkstra algorithm
}Node_t;

typedef struct{
    void * heap;
    long size;
    long idx;
}ListNode;

typedef struct{
    int curr_idx;
    int curr_pidx;
    Node_t * nodes;
    int * prev;
    ListNode * heap;
}dijkstra_t;

bool read_cord(char * file_name, ListNode * a_node);
bool dijkstra(int node1, int node2, ListNode list_node,  ListNode * list_heap, int ** a_prev);
void free_nodes(Node_t * nodes, int size);

#endif
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
