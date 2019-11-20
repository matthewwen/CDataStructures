#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct{
    struct {
        long x;
        long y;
    } coord;
    long idx;
}Node_t;

typedef struct{
    long node_idx;
    long leaf; 
}Edge_t;

typedef struct{
    void * heap;
    long size;
}ListNode;

bool read_cord(char * file_name, ListNode * a_node, ListNode * a_edge) {
    bool isvalid;
    FILE * fp;
    long num_node, num_edge, i;
    if ((isvalid = ((fp = fopen(file_name, "r")) != NULL))) {
        size_t alloc_size;
        fscanf(fp, "%ld %ld", &num_node, &num_edge);

        Node_t * nodes = malloc((alloc_size = (num_node * sizeof(*nodes))));
        if ((isvalid = (nodes != NULL))) {
            memset(nodes, 0, alloc_size);
            for(i = 0; i < num_node; i++) {
                long x, y, idx;
                fscanf(fp, "%ld %ld %ld", &idx, &x, &y);
                nodes[idx] = (Node_t) {.coord = {.x = x, .y = y}};
            }
        }

        Edge_t * edge = malloc((alloc_size = num_edge * sizeof(*edge)));
        if (isvalid && 
           (isvalid = (edge != NULL))) {
            memset(edge, 0, alloc_size);
            long pidx = -1;
            for (i = 0; i < num_edge; i++) {
                long nidx, lidx;
                fscanf(fp, "%ld %ld", &nidx, &lidx);
                if (pidx != nidx) {
                    pidx = nidx;
                    nodes[nidx].idx = i;
                }
                edge[i] = (Edge_t) {.node_idx = nidx, .leaf = lidx};
            }
        }
        *a_node = (ListNode) {.heap = nodes, .size = num_node};
        *a_edge = (ListNode) {.heap = edge,  .size = num_edge};
        fclose(fp);
    }

    return isvalid;
}

int main(int argc, char* argv[]) {
    bool is_valid;
    long i, j;
    if ((is_valid = (argc > 1))) {
        ListNode list_node; ListNode list_edge;
        is_valid = read_cord(argv[1], &list_node, &list_edge);

        // print out results
        Node_t * nodes = list_node.heap;
        Edge_t * edges = list_edge.heap;
        for (i = 0; i < list_node.size; i++) {
            printf("%2ld: ", i);
            for (j = 0; j < list_edge.size; j++) {
                if (edges[j].leaf == i) {
                    printf("%2ld ", edges[j].node_idx);
                }
            }
            for (j = nodes[i].idx; j < list_edge.size && edges[j].node_idx == i; j++) {
                printf("%2ld ", edges[j].leaf);
            }
            printf("\n");
        }
        free(nodes);
        free(edges);
    }
	
	return is_valid ? EXIT_SUCCESS: EXIT_FAILURE;
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
