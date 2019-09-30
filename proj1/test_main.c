#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sorting.h"

int main(int argc, char * argv[]) {
    int size;
    long * list = Load_File("100000.txt", &size);
    double n_comp;
    double n_move;
    //Shell_Insertion_Sort(list, size, &n_comp, &n_move);
    //Shell_Insertion_Sort(list, 10001, &n_comp, &n_move);
    //Shell_Insertion_Sort(list, 100001, &n_comp, &n_move);
    Improved_Bubble_Sort(list, size, &n_comp, &n_move);
    //bool is_error = false;

    bool is_error = false;
    for (int i = 1; i < size; i++) {
        //printf("%ld\n", list[i]);
        if (list[i - 1] > list[i]) {
            is_error = true;
        }
    }
    printf("status: %s\n", is_error ? "Error": "Success");
    
    //Save_File("okay.txt", list, size);
    free(list);
    return EXIT_SUCCESS;
}