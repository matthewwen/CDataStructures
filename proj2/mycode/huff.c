#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "huff.h"

int main(int argc, char* argv[]) {
	bool input_valid = false;
		
	for (int i = 0; i < argc; i++) {
		printf("%s\n", argv[i]);
	}
	if ((input_valid = (argc == 2))) {
		printf("valid input");
	}
	return input_valid ? EXIT_SUCCESS: EXIT_FAILURE;
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
