#include <stdio.h>
#include <stdlib.h>
#include "header.h"

void print_list(int * prev, int i, int j) {
    if (i == j) {
        printf("%d ", i);
    }
    else if (i >= 0) {
        print_list(prev, i, prev[j]);
        printf("%d ", j);
    }
}

int main(int argc, char* argv[]) {
    bool is_valid = false;
    if ((is_valid = (argc > 2))) {
        ListNode list_node;
        is_valid = read_cord(argv[1], &list_node);
        FILE * fp = fopen(argv[2], "r");
        long num_query = 0;
        is_valid = fscanf(fp, "%ld", &num_query);

        long i;
        int * prev = NULL;
        ListNode list_heap = {.heap = NULL, .size = 0, .idx = 0};
        Node_t * nodes = list_node.heap;
        for (i = 0; i < num_query; i++) {
            long start = 0; long end = 0;
            is_valid = fscanf(fp, "%ld %ld", &start, &end);
            if (dijkstra(start, end, list_node, &list_heap, &prev)){
                long distance = nodes[end].distance;
                printf("%ld\n", distance);
                print_list(prev, start, end);
                printf("\n");
            }
            else {
                printf("INF\n%ld %ld\n", start, end);
            }
        }
        fclose(fp);
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
    }
	
	return is_valid ? EXIT_SUCCESS: EXIT_FAILURE;
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
