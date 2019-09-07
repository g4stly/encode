#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"
#include "util.h"


void readStdin(struct Input *input)
{
	char c;
	size_t len = 8;

	input->size = 0;
	input->bytes = malloc(len * sizeof(*input->bytes));
	if (!input->bytes) die("readline():");
	memset(input->bytes, 0, len * sizeof(*input->bytes));

	while ((c = getc(stdin)) && c > 0 && c != '\n') {
		if (input->size > len - 2) {
			len += len;
			input->bytes = realloc(input->bytes, len);
			if (!input->bytes) die("readline():");
			memset(input->bytes + (len / 2), 0, len / 2);
		}
		input->bytes[input->size++] = c;
	}
}

void readFile(struct Input *input, const char *filename)
{
	char c;
	int i = 0;
	size_t len = 8;
	FILE *file = fopen(filename, "r");
	if (!file) die("readFile(): fopen():");

	fseek(file, 0, SEEK_END);
	input->size = ftell(file);
	fseek(file, 0, SEEK_SET);

	input->bytes = malloc(len * sizeof(*input->bytes));
	if (!input->bytes) die("readFile(): malloc():");
	memset(input->bytes, 0, len * sizeof(*input->bytes));

	while (i < input->size) {
		c = getc(file);
		if (i > len - 2) {
			len += len;
			input->bytes = realloc(input->bytes, len);
			if (!input->bytes) die("readline():");
			memset(input->bytes + (len / 2), 0, len / 2);
		}
		input->bytes[i++] = c;
	}

	fclose(file);
}
