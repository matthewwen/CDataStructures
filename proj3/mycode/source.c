#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "header.h"

int  get_distance(Node_t * nodes, int idx1, int idx2);
int get_min(Node_t * nodes, ListNode * list_heap);
void update_distance(Node_t * nodes, Edge_t * edges, int idx, int diff);
void append_element(Node_t *, ListNode * list_heap, int idx);
// ------------------ FOR DEBUGGING ------------------------
void print_heap(char * label, Node_t * nodes, ListNode list_heap) {
    int i;
    int * heap = list_heap.heap;
    printf("--%s--\n", label);
    printf("curr idx: %ld -> ", list_heap.idx);
    for (i = 0; i < list_heap.idx; i++) {
        printf("%f ", nodes[heap[i]].distance);
    }
    printf("\n---\n");
}
// ------------------ FOR DEBUGGING ------------------------
bool read_cord(char * file_name, ListNode * a_node) {
    long num_node = 0, num_edge = 0, i, j;
    Node_t * nodes = NULL;

    bool isvalid;
    FILE * fp;
    if ((isvalid = ((fp = fopen(file_name, "r")) != NULL))) {
        isvalid = fscanf(fp, "%ld %ld", &num_node, &num_edge) == 2;

        // store nodes in an array
        size_t alloc_size = 0;
        if (isvalid && 
           (isvalid = (nodes = malloc((alloc_size = (num_node * sizeof(*nodes))))) 
                       != NULL)) {
            memset(nodes, 0, alloc_size);
            for(i = 0; i < num_node && isvalid; i++) {
                long x, y, idx;
                isvalid = fscanf(fp, "%ld %ld %ld", &idx, &x, &y) == 3;
                nodes[idx] = (Node_t) {.coord = {.x = x, .y = y}, .idx = -1, .minidx = -1, .adj_head = NULL};
            }
            if (!isvalid) {
                free(nodes);
            }
        }
        *a_node = (ListNode) {.heap = nodes, .size = num_node};

        // store the edge as linked list
        if (isvalid) {
            for (i = 0; i < num_edge; i++) {
                long idx_array[2];
                isvalid = (fscanf(fp, "%ld %ld", &(idx_array[0]), &(idx_array[1])) == 2);
                if (!isvalid) {
                    free(nodes);
                }
                else {
                    for (j = 0; j < 2; j++) {
                        int idx     = idx_array[j];
                        int tempidx = idx_array[(j + 1) % 2];
                        llong_t ** curr;
                        bool add_idx = true;
                        for (curr = &(nodes[idx].adj_head); 
                            ((*curr) != NULL) && (add_idx = ((*curr)->idx != tempidx)); 
                            curr = &(*curr)->next) {}

                        if (add_idx) {
                            llong_t * temp = malloc(sizeof(*temp));
                            *temp = (llong_t) {.idx = tempidx, .next = NULL};
                            *curr = temp;
                        }

                    }
                }
            }
        }
        fclose(fp);
    }

    return isvalid;
}

void function(dijkstra_t dijkstra, int adj_idx) {
    int new_distance = get_distance(dijkstra.nodes, dijkstra.curr_idx, adj_idx) + dijkstra.nodes[dijkstra.curr_idx].distance;
    if (adj_idx != dijkstra.curr_pidx) {
        if ((dijkstra.prev[adj_idx] == -1) ||
            (dijkstra.nodes[adj_idx].distance > new_distance)) {
            dijkstra.nodes[adj_idx].distance = new_distance;
            append_element(dijkstra.nodes, dijkstra.heap, adj_idx);
            dijkstra.prev[adj_idx] = dijkstra.curr_idx;
        }
    }
    
}

bool dijkstra(int node1, int node2, ListNode list_node, ListNode * list_heap, int ** a_prev) {
    size_t alloc_size;
    Node_t * nodes = list_node.heap;

    if (list_heap->heap == NULL) {
        *list_heap = (ListNode) {.heap = malloc((alloc_size = (list_node.size * sizeof(int)))), .size = list_node.size};
        memset(list_heap->heap, 0, alloc_size);
    }
    list_heap->idx = 0;

    int * prev;
    if (*a_prev == NULL) {
        prev = malloc(list_node.size * sizeof(*prev));
        *a_prev = prev;
    }
    else {
        prev = *a_prev;
    }
    int i;
    for (i = 0; i < list_node.size; i++) {
        prev[i] = -1;
    }

    bool not_found        = true;
    nodes[node1].distance = 0;
    dijkstra_t dijkstra   = {.nodes = nodes, .heap = list_heap, .prev = prev};
    append_element(nodes, list_heap, node1);
    while (not_found) {
        dijkstra.curr_idx  = get_min(nodes, list_heap);
        dijkstra.curr_pidx = prev[dijkstra.curr_idx];

        if ((not_found = (dijkstra.curr_idx != node2))) {
            llong_t * curr;
            for (curr = nodes[dijkstra.curr_idx].adj_head; curr != NULL; curr = curr->next) {
                function(dijkstra, curr->idx);
            }
        }
        
        not_found = list_heap->idx != 0 && not_found;
    }
    
    // clear min idx
    int * heap = list_heap->heap;
    for (i = 0; i < list_heap->idx; i++) {
        nodes[heap[i]].minidx = -1;
    }

    return dijkstra.curr_idx == node2;
}

void append_element(Node_t * nodes, ListNode * list_heap, int idx) {
    int * heap     = list_heap->heap;

    int curr_idx   = nodes[idx].minidx;
    if (curr_idx < 0) {
        curr_idx = list_heap->idx;
        heap[curr_idx] = idx;
        list_heap->idx = list_heap->idx + 1;
        nodes[idx].minidx = curr_idx;
    }
    else {
        // int i;
        // int * heap = list_heap->heap;
        // bool is_found = false;
        // for (i = 0; i < list_heap->size; i++) {
        //     is_found = heap[i] == idx || is_found;
        // }
        // assert(is_found);
    }

    int temp, node_idx = -1;
    for (;curr_idx > 0 && 
          nodes[heap[(node_idx = ((curr_idx - 1) / 2))]].distance > nodes[heap[curr_idx]].distance; curr_idx = node_idx) {

        int swap_idx = curr_idx;
        // swap
        temp           = heap[node_idx];
        heap[node_idx] = heap[swap_idx];
        heap[swap_idx] = temp;

        // set
        nodes[heap[node_idx]].minidx = node_idx;
        nodes[heap[swap_idx]].minidx = swap_idx;
    }
}

int get_min(Node_t * nodes, ListNode * list_heap) {
    int * heap     = list_heap->heap;
    int size       = list_heap->idx - 1;

    int min           = heap[0];
    nodes[min].minidx = -1;
    heap[0]           = heap[size];
    list_heap->idx    = size;

    int temp, idx;
    int left_dist, right_dist;
    int left_idx, right_idx, i = 0;
    bool is_left      = true;
    bool not_finished = size > 1;
    for (;not_finished; i = is_left ? left_idx: right_idx) {
        // check values
        left_idx  = (2 * i) + 1;
        right_idx = left_idx + 1;
        if ((not_finished = (left_idx < size))) {
            if (right_idx > size) {
                right_idx = left_idx;
            }

            left_dist  = nodes[heap[left_idx]].distance;
            right_dist = nodes[heap[right_idx]].distance;

            if ((not_finished = ((nodes[heap[i]].distance > left_dist || nodes[heap[i]].distance > right_dist) && left_idx < size))) {
                // swap
                is_left = (left_dist < right_dist);
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
    long x_diff = nodes[idx1].coord.x - nodes[idx2].coord.x;
    long y_diff = nodes[idx1].coord.y - nodes[idx2].coord.y;

    double dist_square = (x_diff * x_diff) + (y_diff * y_diff);
    return (sqrt(dist_square));
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */