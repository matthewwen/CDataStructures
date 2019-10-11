#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "huff.h"

header_t * create_table(FILE * fp) {
	fseek(fp, 0L, SEEK_SET);
	size_t memory_size;
	header_t * header = malloc((memory_size = sizeof(*header)));
	memset(header, 0, memory_size);
	header->nodes[0] = (node_t) {.left = NULL, .right = NULL};
	printf("memory size: %ld\n", memory_size);
	for (int i = 0; i < 256; i++) {
		printf("successful = %s\n", header->nodes[i].left == NULL ? "yes": "no");
	}
	return header;
}

int main(int argc, char* argv[]) {
	bool input_valid = false;
		
	for (int i = 0; i < argc; i++) {
		printf("%s\n", argv[i]);
	}
	if ((input_valid = (argc == 2))) {
		printf("valid input");
		FILE * fp = fopen("example.txt", "r");
		header_t * h = create_table(fp);
		free(h);
		fclose(fp);
	}
	return input_valid ? EXIT_SUCCESS: EXIT_FAILURE;
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
