#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sorting.h"

int main(int argc, char * argv[]) {
    int size;
    long * list = Load_File("100000.txt", &size);
    double n_comp;
    double n_move;
    Improved_Bubble_Sort(list, size, &n_comp, &n_move);
    bool is_error = false;
    for (int i = 0; i < size - 1; i++) {
        if (list[i] > list[i+1]) {
            printf("Error\n");
            is_error = true;
        }
    }
    if (is_error) {
        printf("MAJOR ERROR!!\n");
    }
    else {
        printf("NO ERROR!!\n");
    }
    
    //Save_File("okay.txt", list, size);
    free(list);
    return EXIT_SUCCESS;
}