#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <class.h>

#include "encoder.h"
#include "url-encoder.h"
#include "text.h"
#include "util.h"

static const char *digits = "0123456789ABCDEF";
static const char *bad_code_err = "UrlEncoder->decode(): bad code: %%%s\n";

// class definition
struct UrlEncoderCls {
	struct cls super;
};

// representation
struct R_UrlEncoder {
	struct Encoder *encoder;
	struct UrlEncoderCls *cls;
};

// complete instance struct
struct _UrlEncoder {
	struct R_UrlEncoder r;	// representation
	struct Encoder i;	// interface
};

static int unsafe(char c)
{
	switch (c) {
	case ' ':
	case '"':
	case '\'':
	case '<':
	case '>':
	case '{':
	case '}':
	case '[':
	case ']':
	case '#':
	case '%':
	case '\\':
	case '|':
	case '^':
	case '~':
	case '`':
		return 1;
	}
	return c <= 0x1F || c >= 0x80;
}

static struct Text *encode(struct Encoder *_self, struct Text *input)
{
	struct _UrlEncoder *self = (void *)_self - sizeof(struct R_UrlEncoder);
	return self->r.encoder->encode(self->r.encoder, input);
}

static void encode_step(
	int inputSz, int *i, const char *input,
	int outputSz, int *j, char *output)
{
	char c = input[(*i)++];

	if (!unsafe(c)) {
		output[(*j)++] = c;
		return;
	}

	if (c < 0) c = 0;

	output[(*j)++] = '%';
	output[(*j)++] = digits[(c / 16) % 16];
	output[(*j)++] = digits[(c) % 16];
}

static struct Text *decode(struct Encoder *_self, struct Text *input)
{
	struct _UrlEncoder *self = (void *)_self - sizeof(struct R_UrlEncoder);
	return self->r.encoder->decode(self->r.encoder, input);
}

static char temp[] = { 0, 0, 0 };
static void decode_step(
	int inputSz, int *i, const char *input, 
	int outputSz, int *j, char *output)
{
	char c = input[(*i)++];

	if (c != '%') {
		output[(*j)++] = c;
		return;
	}

	temp[0] = input[(*i)++];
	temp[1] = input[(*i)++];

	output[(*j)++] = strtol(temp, NULL, 16);
	if (!output[(*j) - 1]) die(bad_code_err, temp);
}

static void *UrlEncoderCtor(void *_self, va_list *ap)
{
	struct _UrlEncoder *self = _self;
	self->r.encoder = (struct Encoder*) new(Encoder, encode_step, decode_step);
	self->i.encode = encode;
	self->i.decode = decode;

	return &self->i;
}

static void *UrlEncoderDtor(void *_self)
{
	struct _UrlEncoder *self = (void *)_self - sizeof(struct R_UrlEncoder);
	delete(self->r.encoder);
	return self;
}

static const struct UrlEncoderCls _UrlEncoder = {
	{
		sizeof(struct _UrlEncoder),
		UrlEncoderCtor,
		UrlEncoderDtor
	}
};

const void *UrlEncoder = &_UrlEncoder;
