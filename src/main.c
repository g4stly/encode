#include <stdio.h>
#include <string.h>
#include <class.h>

#include "url-encoder.h"
#include "options.h"
#include "encoder.h"
#include "util.h"

int main(int argc, char **argv)
{
	int aux = 0;
	enum Mode mode = ENCODE;
	enum Scheme scheme = URL;

	parseOptions(argc, argv, &mode, &scheme);

	struct Encoder *e;
	switch (scheme) {
	case URL:
		e = new(UrlEncoder);
		break;
	case B64:
		e = new(UrlEncoder);
		break;
	case B64U:
		e = new(UrlEncoder);
		break;
	default:
		fprintf(stderr, "missing scheme!\n");
		return -1;
	}

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
