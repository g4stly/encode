#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <class.h>

#include "text.h"
#include "input.h"
#include "util.h"


struct Text *readStdin()
{
	char c;
	int size = 0;
	size_t len = 8;

	char *bytes = malloc(len * sizeof(*bytes));
	if (!bytes) die("readline():");
	memset(bytes, 0, len * sizeof(*bytes));

	while ((c = getc(stdin)) && c > 0) {
		if (size > len - 2) {
			len += len;
			bytes = realloc(bytes, len);
			if (!bytes) die("readline():");
			memset(bytes + (len / 2), 0, len / 2);
		}
		bytes[size++] = c;
	}

	return new(Text, bytes, size);
}

struct Text *readFile(const char *filename)
{
	char c;
	size_t len = 8;
	int size, i = 0;
	FILE *file = fopen(filename, "r");
	if (!file) die("readFile(): fopen():");

	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);

	char *bytes = malloc(len * sizeof(*bytes));
	if (!bytes) die("readFile(): malloc():");
	memset(bytes, 0, len * sizeof(*bytes));

	while (i < size) {
		c = getc(file);
		if (i > len - 2) {
			len += len;
			bytes = realloc(bytes, len);
			if (!bytes) die("readline():");
			memset(bytes + (len / 2), 0, len / 2);
		}
		bytes[i++] = c;
	}

	fclose(file);

	return new(Text, bytes, size);
}
