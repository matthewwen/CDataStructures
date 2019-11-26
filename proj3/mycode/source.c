#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "header.h"

int get_distance(Node_t * nodes, int idx1, int idx2);
void update_distance(Node_t * nodes, Edge_t * edges, int idx, int diff);
void append_element(ListNode list_node, ListNode * list_heap, int idx);

bool read_cord(char * file_name, ListNode * a_node, ListNode * a_edge) {
    bool isvalid;
    FILE * fp;
    long num_node = 0, num_edge = 0, i, j;
    Node_t * nodes = NULL;
    Edge_t * edge  = NULL;
    size_t alloc_size = 0;
    if ((isvalid = ((fp = fopen(file_name, "r")) != NULL))) {
        isvalid = fscanf(fp, "%ld %ld", &num_node, &num_edge) == 2;

        // store nodes in an array
        if (isvalid && 
           (isvalid = (nodes = malloc((alloc_size = (num_node * sizeof(*nodes))))) 
                       != NULL)) {
            memset(nodes, 0, alloc_size);
            for(i = 0; i < num_node && isvalid; i++) {
                long x, y, idx;
                isvalid = fscanf(fp, "%ld %ld %ld", &idx, &x, &y) == 3;
                nodes[idx] = (Node_t) {.coord = {.x = x, .y = y}, .idx = -1, .adj_head = NULL, .adj_tail = NULL};
            }
            if (!isvalid) {
                free(nodes);
            }
        }

        // store the edge in an array
        if (isvalid) {
            alloc_size = num_edge * sizeof(*edge);
            edge = malloc(alloc_size);
            if (isvalid) {
                memset(edge, 0, alloc_size);
                long pidx = -1;
                for (i = 0; i < num_edge; i++) {
                    long nidx, lidx;
                    isvalid = (fscanf(fp, "%ld %ld", &nidx, &lidx) == 2);
                    if (!isvalid) {
                        free(edge);
                        free(nodes);
                    }
                    else {
                        if (pidx != nidx) {
                            pidx = nidx;
                            nodes[nidx].idx = i;
                        }
                        edge[i] = (Edge_t) {.node_idx = nidx, .leaf = lidx};
                    }
                }
            }
        }

        // add adjacent values in linked list
        if (isvalid) {
            for (i = 0; i < num_edge; i++) {
                int adj_idx  = edge[i].leaf;
                int curr_idx = edge[i].node_idx;
                bool already_inserted = false;
                for (j = nodes[adj_idx].idx; j >= 0 && j < num_node && 
                                             edge[j].node_idx == adj_idx; j++) {
                    already_inserted = edge[j].leaf == curr_idx;
                }
                if (!already_inserted) {
                    llong_t * temp = malloc(sizeof(*temp));
                    *temp = (llong_t) {.idx = curr_idx, .next = NULL};
                    if (nodes[adj_idx].adj_head == NULL) {
                        nodes[adj_idx].adj_head = temp;
                        nodes[adj_idx].adj_tail = temp;
                    }
                    else {
                        nodes[adj_idx].adj_tail->next = temp;
                        nodes[adj_idx].adj_tail = temp;
                    }
                }
            }
        }

        *a_node = (ListNode) {.heap = nodes, .size = num_node};
        *a_edge = (ListNode) {.heap = edge,  .size = num_edge};
        fclose(fp);
    }

    return isvalid;
}

void dijkstra(int node1, int node2, ListNode list_node, ListNode list_edge) {
    size_t alloc_size;
    int * heap = malloc(alloc_size = list_node.size * sizeof(*heap));
    memset(heap, 0, alloc_size);
    ListNode list_heap = {.heap = heap, .size = list_node.size, .idx = 0};
    // this is for gdb
    int i;
    Node_t * nodes = list_node.heap;

    nodes[0].distance = 100;
    append_element(list_node, &list_heap, 0);
    printf("-----\n");
    for (i = 0; i < list_heap.idx; i++) {
        printf("%d ", nodes[heap[i]].distance);
    }
    printf("\n");

    nodes[1].distance = 50;
    append_element(list_node, &list_heap, 1);
    printf("-----\n");
    for (i = 0; i < list_heap.idx; i++) {
        printf("%d ", nodes[heap[i]].distance);
    }
    printf("\n");

    nodes[2].distance = 30;
    append_element(list_node, &list_heap, 2);
    printf("-----\n");
    for (i = 0; i < list_heap.idx; i++) {
        printf("%d ", nodes[heap[i]].distance);
    }
    printf("\n");

    nodes[3].distance = 150;
    printf("-----\n");
    append_element(list_node, &list_heap, 3);
    for (i = 0; i < list_heap.idx; i++) {
        printf("%d ", nodes[heap[i]].distance);
    }
    printf("\n");

    // Node_t * nodes = list_node.heap;
    // Edge_t * edge  = list_node.heap;

    // printf("%d\n", (int) sqrt(25));
    free(heap);
}

void append_element(ListNode list_node, ListNode * list_heap, int idx) {
    Node_t * nodes = list_node.heap;
    int * heap     = list_heap->heap;
    int curr_idx   = list_heap->idx;
    heap[curr_idx] = idx;

    int temp, node_idx = -1;
    for (;curr_idx >= 0 && 
          nodes[heap[(node_idx = ((curr_idx - 1) / 2))]].distance > nodes[heap[curr_idx]].distance; curr_idx = node_idx) {
        temp           = heap[node_idx];
        heap[node_idx] = heap[curr_idx];
        heap[curr_idx] = temp;
    }

    list_heap->idx = list_heap->idx + 1;
}

int get_min(ListNode list_node, ListNode list_heap) {
    Node_t * nodes = list_node.heap;
    int * heap     = list_heap.heap;
    int size       = list_heap.idx - 1;

    int min       = heap[0];
    heap[0]       = heap[list_heap.idx - 1];
    list_heap.idx = size;

    // still need to implement
    int temp;
    int left_dist, right_dist;
    int left_idx, right_idx, i = 0;
    bool is_left;
    for (;(nodes[heap[i]].distance < (left_dist  = nodes[heap[left_idx  = (2 * i) + 1]].distance) || 
           nodes[heap[i]].distance < (right_dist = nodes[heap[right_idx = (2 * i) + 2]].distance)) && 
           left_idx < size; i = is_left ? left_idx: right_idx) {
              is_left = (left_dist > right_dist) || (right_idx > size);
              temp    = heap[i];
              heap[i] = is_left ? heap[left_idx]: heap[right_idx];
              heap[is_left ? left_idx: right_idx] = temp;
          }

    return min;
}

void free_nodes(Node_t * nodes, int size) {
    int i;
    for (i = 0; i < size; i++) {
        llong_t * curr, * prev;
        if ((curr = nodes[i].adj_head) != NULL) {
            while (curr != NULL) {
                prev = curr;
                curr = curr->next;
                free(prev);
            }
        }
    }
    free(nodes);
}

int get_distance(Node_t * nodes, int idx1, int idx2) {
    int x_diff = nodes[idx1].coord.x - nodes[idx2].coord.x;
    int y_diff = nodes[idx1].coord.y - nodes[idx2].coord.y;

    double dist_square = (x_diff * x_diff) + (y_diff * y_diff);
    return sqrt(dist_square);
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */