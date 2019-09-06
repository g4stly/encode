#ifndef ENCODER_H_
#define ENCODER_H_


struct Encoder {
	char *(*encode)(struct Encoder *e, const char *input);
	char *(*decode)(struct Encoder *e, const char *input);
};

extern const void *Encoder;

#endif
