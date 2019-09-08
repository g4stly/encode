#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <class.h>

#include "encoder.h"
#include "b64-encoder.h"
#include "text.h"
#include "util.h"


#define FIRST 	(0x00FC0000)
#define SECOND 	(0x0003F000)
#define THIRD 	(0x00000FC0)
#define FOURTH	(0x0000003F)

// class definition
struct B64EncoderCls {
	struct cls super;
};

// representation
struct R_B64Encoder {
	struct Encoder *encoder;
	struct B64EncoderCls *cls;
};

// complete instance struct
struct _B64Encoder {
	struct R_B64Encoder r;	// representation
	struct Encoder i;	// interface
};

static char alphabetIn(int ch)
{
	if (ch < 26) return ch + 65;	// A - Z
	if (ch < 52) return ch + 71;	// a - z
	if (ch < 62) return ch - 4;	// 0 - 9
	return (ch == 62) ? '+' : '/';
}

static char alphabetOut(int ch)
{
	if (ch > 64 && ch < 91) return ch - 65;		// A - Z
	if (ch > 96 && ch < 123) return ch - 71;	// a - z
	if (ch > 47 && ch < 58) return ch + 4;		// 0 - 9
	if (ch == '+') return 62;
	if (ch == '/') return 63;

	die("B64Encoder->decode(): illegal character: %c\n", ch);

	return 0;
}

static struct Text *encode(struct Encoder *_self, struct Text *input)
{
	struct _B64Encoder *self = (void *)_self - sizeof(struct R_B64Encoder);
	return self->r.encoder->encode(self->r.encoder, input);
}

static const int masks[] = { FIRST, SECOND, THIRD, FOURTH };
static void encode_step(
	int inputSz, int *i, const char *input,
	int outputSz, int *j, char *output)
{
	int a, temp = 0;

	// read three? bytes
	for (a = 0; a < 3; a++) {
		if (*i >= inputSz) break;
		// & 0xFF here because wordsize is > 1 byte and 
		// my platform fills the empty bits with 1's
		// was the source of a sneaky bug
		temp = temp | (input[(*i)++] & 0xFF) << ((2 - a) * 8);
	}

	// write four-ish bytes
	for (int b = 0; b < 1 + a; b++) {
		int x = (temp & masks[b]) >> ((3 - b) * 6);
		output[(*j)++] = alphabetIn(x);
	}

	// write padding
	for (; a < 3; a++) output[(*j)++] = '=';
}

static struct Text *decode(struct Encoder *_self, struct Text *input)
{
	struct _B64Encoder *self = (void *)_self - sizeof(struct R_B64Encoder);
	return self->r.encoder->decode(self->r.encoder, input);
}

static void decode_step(
	int inputSz, int *i, const char *input,
	int outputSz, int *j, char *output)
{
	char c;
	int shift, read = 0, temp = 0;

	// read four? bytes
	for (int a = 0; a < 4; a++) {
		if (*i >= inputSz || (c = input[(*i)++])  == '=') break;
		temp = temp | alphabetOut(c) << ((3 - a) * 6);
		read++;
	}

	if (read < 2) { return; }

	// write three? bytes
	for (int b = 0; b < read - 1; b++) {
		shift = (2 - b) * 8;
		output[(*j)++] = (temp & 0xFF << shift) >> shift;
	}
}

static void *B64EncoderCtor(void *_self, va_list *ap)
{
	struct _B64Encoder *self = _self;
	self->r.encoder = (struct Encoder*) new(Encoder, encode_step, decode_step);
	self->i.encode = encode;
	self->i.decode = decode;

	return &self->i;
}

static void *B64EncoderDtor(void *_self)
{
	struct _B64Encoder *self = (void *)_self - sizeof(struct R_B64Encoder);
	delete(self->r.encoder);
	return self;
}

static const struct B64EncoderCls _B64Encoder = {
	{
		sizeof(struct _B64Encoder),
		B64EncoderCtor,
		B64EncoderDtor
	}
};

const void *B64Encoder = &_B64Encoder;
