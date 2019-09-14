#include <stdio.h>
#include <stdlib.h>
#include "sorting.h"

int main(int argc, char * argv[]) {
    int size;
    long * list = Load_File("1000.txt", &size);
    Save_File("okay.txt", list, size);
    free(list);
    return EXIT_SUCCESS;
}