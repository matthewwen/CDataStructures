#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "header.h"

int get_distance(Node_t * nodes, int idx1, int idx2);
int get_min(ListNode list_node, ListNode * list_heap);
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
                nodes[idx] = (Node_t) {.coord = {.x = x, .y = y}, .idx = -1, .minidx = -1, .adj_head = NULL, .adj_tail = NULL};
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


// ------------------ FOR DEBUGGING ------------------------
void print_heap(char * label, ListNode list_node, ListNode list_heap) {
    int i;
    Node_t * nodes = list_node.heap;
    int * heap = list_heap.heap;
    printf("--%s--\n", label);
    for (i = 0; i < list_heap.idx; i++) {
        printf("%d ", nodes[heap[i]].distance);
    }
    printf("\n---\n");
}
// ------------------ FOR DEBUGGING ------------------------

void dijkstra(int node1, int node2, ListNode list_node, ListNode list_edge) {
    size_t alloc_size;
    int * heap = malloc(alloc_size = list_node.size * sizeof(*heap));
    memset(heap, 0, alloc_size);
    ListNode list_heap = {.heap = heap, .size = list_node.size, .idx = 0};
    Node_t * nodes = list_node.heap;
    Edge_t * edges = list_edge.heap;


    /* TODO
    - Check to see if index is repeated
    - Check smaller than or greater than values
    - Store previous location
    - Done
    */

    bool not_found = true;
    nodes[node1].distance = 0;
    append_element(list_node, &list_heap, node1);
    print_heap("add element", list_node, list_heap);
    int i, new_distance;
    llong_t * curr;
    while (not_found) {
        int curr_idx = get_min(list_node, &list_heap);
        print_heap("remove element", list_node, list_heap);
        if ((not_found = (curr_idx != node2))) {
            for (i = nodes[curr_idx].idx; edges[i].node_idx == curr_idx; i++) {
                printf("here\n");
                printf("i: %d, curr_idx: %d, edges[i]: %ld\n", i, curr_idx, edges[i].node_idx);
                new_distance = get_distance(nodes, curr_idx, edges[i].leaf) + nodes[curr_idx].distance;
                if (nodes[edges[i].leaf].distance == 0 && edges[i].leaf != node1) {
                    nodes[edges[i].leaf].distance = new_distance;
                    printf("this is good\n");
                    append_element(list_node, &list_heap, edges[i].leaf);
                }
                else {
                    if (new_distance < nodes[curr_idx].distance) {
                        printf("DO update: %ld, %d\n", edges[i].leaf, nodes[edges[i].leaf].distance);
                    }
                    else {
                        printf("DO NOT update: %ld, %d\n", edges[i].leaf, nodes[edges[i].leaf].distance);
                    }
                }
                printf("size: %ld\n", list_heap.idx);
                print_heap("add element", list_node, list_heap);
            }
            for (curr = nodes[curr_idx].adj_head; curr != NULL; curr = curr->next) {
                new_distance = get_distance(nodes, curr_idx, curr->idx) + nodes[curr_idx].distance;
                if (nodes[curr->idx].distance == 0 && curr->idx != node1) {
                    printf("this is good\n");
                    nodes[curr->idx].distance = new_distance;
                    append_element(list_node, &list_heap, curr->idx);
                }
                else {
                    if (new_distance < nodes[curr_idx].distance) {
                        printf("DO update: %ld, %d\n", curr->idx, nodes[curr->idx].distance);
                    }
                    else {
                        printf("DO NOT update: %ld, %d\n", curr->idx, nodes[curr->idx].distance);
                    }
                }
                printf("size: %ld\n", list_heap.idx);
                print_heap("add element", list_node, list_heap);
            }
        }
        not_found = list_heap.idx != 0 && not_found;
    }

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
        // swap
        temp           = heap[node_idx];
        heap[node_idx] = heap[curr_idx];
        heap[curr_idx] = temp;

        // set
        nodes[heap[node_idx]].minidx = node_idx;
        nodes[heap[curr_idx]].minidx = curr_idx;
    }
    list_heap->idx = list_heap->idx + 1;
}

int get_min(ListNode list_node, ListNode * list_heap) {
    Node_t * nodes = list_node.heap;
    int * heap     = list_heap->heap;
    int size       = list_heap->idx - 1;

    int min       = heap[0];
    heap[0]       = heap[size];
    list_heap->idx = size;

    int temp;
    int left_dist, right_dist;
    int left_idx, right_idx, i = 0;
    int idx;
    bool is_left = true;
    bool not_finished = true;
    for (;not_finished; i = is_left ? left_idx: right_idx) {
        // check values
        left_idx  = (2 * i) + 1;
        right_idx = left_idx + 1;
        if (left_idx < list_heap->size) {
            if (right_idx > list_heap->size) {
                right_idx = left_idx;
            }

            left_dist  = nodes[heap[left_idx]].distance;
            right_dist = nodes[heap[right_idx]].distance;

            if ((not_finished = ((nodes[heap[i]].distance > left_dist || nodes[heap[i]].distance > right_dist) && left_idx < size))) {
                // swap
                is_left = (left_dist >= right_dist);
                temp    = heap[i];
                heap[i] = is_left ? heap[left_idx]: heap[right_idx];
                heap[(idx = (is_left ? left_idx: right_idx))] = temp;
                
                // set
                nodes[heap[i]].minidx   = i;
                nodes[heap[idx]].minidx = idx;
            }
        }

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