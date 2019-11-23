#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "header.h"

int get_distance(Node_t * nodes, int idx1, int idx2);

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

    Node_t * nodes = list_node.heap;
    Edge_t * edge  = list_node.heap;

    // printf("%d\n", (int) sqrt(25));
    free(heap);
}

int set_distance(Node_t * nodes, int idx1, int idx2) {
    int x_diff = nodes[idx1].coord.x - nodes[idx2].coord.x;
    int y_diff = nodes[idx1].coord.y - nodes[idx2].coord.y;

    double dist_square = (x_diff * x_diff) + (y_diff * y_diff);
    int dist           = sqrt(dist_square);

    nodes[idx1].distance = dist;

    return dist;
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
