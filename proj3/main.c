#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "header.h"

// you could just go the other way right?
void print_list(int * prev, int i, int j) {
    if (i == j) {
        printf(" %d ", i);
    }
    else if (i >= 0) {
        print_list(prev, i, prev[j]);
        printf(" %d ", j);
    }
}

int main(int argc, char* argv[]) {
    bool is_valid = false;
    if ((is_valid = (argc > 1))) {
        ListNode list_node; ListNode list_edge;
        is_valid = read_cord(argv[1], &list_node, &list_edge);

        // print out results
        printf("Done Creating Nodes\n");
        int i, j;
        int * prev = NULL;
        ListNode list_heap = {.heap = NULL, .size = 0, .idx = 0};

        // debug
        // bool test = dijkstra(14, 0, list_node, list_edge, &list_heap, &prev);
        // assert(test == false);
        // assert(false);
        // test = dijkstra(0, 14, list_node, list_edge, &list_heap, &prev);
        // assert(test == true);
        // debug

        Node_t * nodes = list_node.heap;
        for (i = 0; i < 24; i++) {
            for (j = 0; j < 24; j++) {
                printf("i: %2d, j: %2d, ", i, j);
                if (dijkstra(i, j, list_node, list_edge, &list_heap, &prev)){
                    printf("d: %f -> ", nodes[j].distance);
                    print_list(prev, i, j);
                }
                else {
                    printf("No Path Found");
                }
                printf("\n");
            }
        }
        if (prev != NULL) {
            free(prev);
            prev = NULL;
        }
        if (list_heap.heap != NULL) {
            free(list_heap.heap);
            list_heap.heap = NULL;
        }

        // free method
        free_nodes(list_node.heap, list_node.size);
        free(list_edge.heap);
        printf("Done Freeing\n");
    }
	
	return is_valid ? EXIT_SUCCESS: EXIT_FAILURE;
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
