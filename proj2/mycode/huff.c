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
		header->values[i].weight += 1;
	}

	return header;
}

node_t * create_tree(header_t * header) {
	int i;
	int buffer_end = NUM_CHAR - 1;
	value_t * value;
	for (i = 0; i < buffer_end; i++) {
		value = &(header->values[i]);
		if (value->weight == 0) {
			*value       = header->values[buffer_end];
			value->value = buffer_end--;
			i--;
		}
		else if (value->value == 0) {
			value->value = i;
		}
	}

	for (i = 0; i < buffer_end; i+= 2) {
		int j;
		int minidx  = i;
		int min1idx = i + 1;
		value_t min     = header->values[minidx];
		value_t min1    = header->values[min1idx];
		for (j = i; j < buffer_end; j++) {
			value_t value = header->values[j];
			if (value.weight < min.weight) {
				minidx = j;
				min    = value;
			}
			else if (value.weight < min1.weight) {
				min1idx = j;
				min1    = value;
			}
		}
		header->values[minidx]  = header->values[i];
		header->values[min1idx] = header->values[i + 1];
		printf("weight: %d\n", min.weight + min1.weight);
	}

	printf("i: %d, buffer_end: %d\n", i, buffer_end);

	return NULL;
}

int main(int argc, char* argv[]) {
	bool input_valid = false;
		
	if ((input_valid = (argc == 2))) {
		FILE * fp = fopen("example.txt", "r");
		header_t * h = create_table(fp);
		create_tree(h);
		free(h);
		fclose(fp);
	}
	return input_valid ? EXIT_SUCCESS: EXIT_FAILURE;
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
