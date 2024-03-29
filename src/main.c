#include <stdio.h>
#include <string.h>
#include <class.h>

#include "url-encoder.h"
#include "b64-encoder.h"
#include "options.h"
#include "encoder.h"
#include "input.h"
#include "text.h"

int main(int argc, char **argv)
{
	enum Mode mode = ENCODE;
	enum Scheme scheme = B64;
	const char *source = NULL;

	parseOptions(argc, argv, &mode, &scheme, &source);

	struct Encoder *e;
	switch (scheme) {
	case URL:
		e = new(UrlEncoder);
		break;
	case B64:
		e = new(B64Encoder);
		break;
	case B64U:
		e = new(UrlEncoder);
		break;
	default:
		fprintf(stderr, "missing scheme!\n");
		return -1;
	}

	struct Text *input = source 
		? readFile(source) 
		: readStdin();

	struct Text *output = mode == ENCODE
		? e->encode(e, input)
		: e->decode(e, input);

	fwrite(output->getBytes(output), sizeof(char),
		output->getSize(output), stdout);

	delete(output);
	delete(input);
	delete(e);

	return 0;
}
