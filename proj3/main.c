#include <stdio.h>
#include <stdlib.h>
#include "header.h"

int main(int argc, char* argv[]) {
    bool is_valid;
    long i, j;
    if ((is_valid = (argc > 1))) {
        ListNode list_node; ListNode list_edge;
        is_valid = read_cord(argv[1], &list_node, &list_edge);

        // print out results
        Node_t * nodes = list_node.heap;
        Edge_t * edges = list_edge.heap;
        printf("Done Creating Nodes\n");
        // for (i = 0; i < list_node.size; i++) {
        //     printf("%2ld: ", i);
        //     llong_t * curr;
        //     for (curr = nodes[i].adj_head; curr != NULL; curr = curr->next) {
        //         printf("%2ld ", curr->idx);
        //     }
        //     for (j = nodes[i].idx; j >= 0 && j < list_edge.size && edges[j].node_idx == i; j++) {
        //         printf("%2ld ", edges[j].leaf);
        //     }
        //     printf("\n");
        // }

        // free method
        for (i = 0; i < list_node.size; i++) {
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
        free(edges);
        printf("Done Freeing\n");
    }
	
	return is_valid ? EXIT_SUCCESS: EXIT_FAILURE;

	return EXIT_SUCCESS;
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
