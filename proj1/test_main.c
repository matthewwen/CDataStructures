#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sorting.h"

int main(int argc, char * argv[]) {
    int size;
    long * list = Load_File("1000.txt", &size);
    double n_comp;
    double n_move;
    //Shell_Insertion_Sort(list, 1001, &n_comp, &n_move);
    Shell_Insertion_Sort(list, 10001, &n_comp, &n_move);
    // Shell_Insertion_Sort(list, 100001, &n_comp, &n_move);
    // Shell_Insertion_Sort(list, 1000001, &n_comp, &n_move);
    bool is_error = false;
    
    //Save_File("okay.txt", list, size);
    free(list);
    return EXIT_SUCCESS;
}