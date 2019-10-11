#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "huff.h"

header_t * create_table(FILE * fp) {
	size_t memory_size;
	header_t * header = malloc((memory_size = sizeof(*header)));
	memset(header, 0, memory_size);

	int i;
	fseek(fp, 0L, SEEK_SET);
	while ((i = fgetc(fp)) != EOF) {
		header->nodes[i].data.value.occurance += 1;
	}

	return header;
}

node_t * create_tree(header_t * header) {
	int i;
	data_t * data;
	int buffer_end = NUM_CHAR - 1;
	for (i = 0; i < buffer_end; i++) {
		data = &(header->nodes[i].data);
		if (data->value.occurance == 0) {
			*data             = header->nodes[buffer_end].data;
			data->value.value = buffer_end--;
			i--;
		}
		else if (data->value.value == 0) {
			data->value.value = i;
		}
	}

	for (i = 0; i < buffer_end; i++) {
		printf("val: %c, occurance: %d\n", header->nodes[i].data.value.value, header->nodes[i].data.value.occurance);
	}

	return NULL;
}

int main(int argc, char* argv[]) {
	bool input_valid = false;
		
	if ((input_valid = (argc == 2))) {
		printf("valid input");
		FILE * fp = fopen("example.txt", "r");
		header_t * h = create_table(fp);
		create_tree(h);
		free(h);
		fclose(fp);
	}
	return input_valid ? EXIT_SUCCESS: EXIT_FAILURE;
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
