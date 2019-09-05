#include <stdio.h>
#include <class.h>

#include "url-encoder.h"
#include "util.h"

enum Mode
{
	ENCODE,
	DECODE
};
static enum Mode mode = ENCODE;

int main(int argc, char **argv)
{
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] != '-') {
			fprintf(stderr, "bad argument: %s\n", argv[i]);
			continue;
		}
		switch (argv[i][1]) {
		case 'e':
			mode = ENCODE;
			break;
		case 'd':
			mode = DECODE;
			break;
		default:
			fprintf(stderr, "bad flag: %s\n", argv[i]);
		}
	}

	//TODO: base64 encoder here
	struct UrlEncoder *e = new(UrlEncoder);

	char *input = readline();
	char *output = mode == ENCODE
		? e->encode(e, input)
		: e->decode(e, input);

	printf("%s\n", output);

	free(output);
	free(input);
	delete(e);

	return 0;
}
