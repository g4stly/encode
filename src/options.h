#ifndef OPTIONS_H_
#define OPTIONS_H_

enum Mode {
	ENCODE,
	DECODE
};

#define SCHEME 1
enum Scheme {
	URL,
	B64,
	B64U
};

void parseOptions(int argc, char **argv, enum Mode *mode, enum Scheme *scheme);

#endif
