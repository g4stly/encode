#include <class.h>
#include <string.h>

#include "encoder.h"
#include "text.h"
#include "util.h"

static const char *malloc_err = "Encoder->translate(): malloc():";

struct EncoderCls {
	struct cls super;
};

struct R_Encoder {
	void (*encode_step)(
		int inputSz,
		int *inputIdx,
		const char *input,
		int outputSz,
		int *outputIdx,
		char *output
	);
	void (*decode_step)(
		int inputSz,
		int *inputIdx,
		const char *input,
		int outputSz, int *outputIdx,
		char *output
	);
	struct EncoderCls *cls;
};

struct _Encoder {
	struct R_Encoder r;
	struct Encoder i;
};

static struct Text *translate(
	struct Text *input,
	void (*step)(int, int *, const char *, int, int*, char *))
{
	if (!input) return NULL;

	int i = 0, j = 0;
	size_t len = 8 * sizeof(char);
	long size = input->getSize(input);
	const char *buffer = input->getBytes(input);

	char *output = malloc(len);
	if (!output) die(malloc_err);
	memset(output, 0, len);

	while (i < size) {
		if (j > len - 2) {
			len += len;
			output = realloc(output, len);
			if (!output) die(malloc_err);
			memset(output + (len / 2) + 1, 0, len / 2);
		}

		// with great power comes great responsibility
		step(size, &i, buffer, len, &j, output);
	}

	return new(Text, output, j);
}

static struct Text *encode(struct Encoder *_self, struct Text *input)
{
	struct R_Encoder *self = (void *)_self - sizeof(struct R_Encoder);
	return translate(input, self->encode_step);
}

static struct Text *decode(struct Encoder *_self, struct Text *input)
{
	struct R_Encoder *self = (void *)_self - sizeof(struct R_Encoder);
	return translate(input, self->decode_step);
}

static void *EncoderCtor(void *_self, va_list *ap)
{
	struct _Encoder *self = _self;
	self->r.encode_step = va_arg(
		*ap, void (*)(int, int *, const char *, int, int *, char *)
	);
	self->r.decode_step = va_arg(
		*ap, void (*)(int, int *, const char *, int, int *, char *)
	);
	self->i.encode = encode;
	self->i.decode = decode;
	return &self->i;
}

static void *EncoderDtor(void *self)
{
	return self - sizeof(struct R_Encoder);
}

static const struct EncoderCls _Encoder = {
	{
		sizeof(struct _Encoder),
		EncoderCtor,
		EncoderDtor	
	}
};

const void *Encoder = &_Encoder;
