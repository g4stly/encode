#include <string.h>
#include <stdio.h>
#include "options.h"

void parseOptions(int argc, char **argv,
	enum Mode *mode,
	enum Scheme *scheme,
	const char **source)
{
	int aux = 0;
	for (int i = 1; i < argc; i++) {
		switch(aux) {
		case SOURCE:
			aux = 0;
			*source = argv[i];
			continue;
		case SCHEME:
			aux = 0;
			if (!strncmp("url", argv[i], 3)) {
				scheme = URL;
				continue;
			}
			if (!strncmp("b64", argv[i], 3)) {
				*scheme = B64;
				continue;
			}
			if (!strncmp("b64u", argv[i], 4)) {
				*scheme = B64U;
				continue;
			}
			fprintf(stderr, "bad scheme: %s\n", argv[i]);
			continue;
		}
		if (argv[i][0] != '-') {
			fprintf(stderr, "bad argument: %s\n", argv[i]);
			continue;
		}
		switch (argv[i][1]) {
		case 'e':
			*mode = ENCODE;
			break;
		case 'd':
			*mode = DECODE;
			break;
		case 's':
			aux = SCHEME;
			break;
		case 'f':
			aux = SOURCE;
			break;
		default:
			fprintf(stderr, "bad flag: %s\n", argv[i]);
			break;
		}
	}
}
