#ifndef _INPUT_H_
#define _INPUT_H_

struct Input {
	long size;
	char *bytes;
};

void readStdin(struct Input *input);
void readFile(struct Input *input, const char *filename);

#endif
