#ifndef OPTIONS_H_
#define OPTIONS_H_

enum Mode {
	ENCODE,
	DECODE
};

enum Scheme {
	URL,
	B64,
	B64U
};

#define SCHEME 1
#define SOURCE 2

void parseOptions(int argc, char **argv, 
	enum Mode *mode, 	// encode or decode?
	enum Scheme *scheme,	// which scheme?
	const char **source	// null or filename
);

#endif
