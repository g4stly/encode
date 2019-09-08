#ifndef ENCODER_H_
#define ENCODER_H_

#include "text.h"

struct Encoder {
	struct Text *(*encode)(struct Encoder *e, struct Text *input);
	struct Text *(*decode)(struct Encoder *e, struct Text *input);
};

extern const void *Encoder;

#endif
