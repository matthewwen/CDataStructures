#include <stdio.h>
#include <stdlib.h>
#include "header.h"

int main(int argc, char* argv[]) {
    bool is_valid;
    if ((is_valid = (argc > 1))) {
        ListNode list_node; ListNode list_edge;
        is_valid = read_cord(argv[1], &list_node, &list_edge);

        // print out results
        // Edge_t * edges = list_edge.heap;
        printf("Done Creating Nodes\n");

        dijkstra(0, 1, list_node, list_edge);

        // free method
        free_nodes(list_node.heap, list_node.size);
        free(list_edge.heap);
        printf("Done Freeing\n");
    }
	
	return is_valid ? EXIT_SUCCESS: EXIT_FAILURE;

	return EXIT_SUCCESS;
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
