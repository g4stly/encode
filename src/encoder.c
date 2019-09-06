#include <class.h>
#include <string.h>

#include "encoder.h"
#include "util.h"

static const char *malloc_err = "Encoder->translate(): malloc():";

struct EncoderCls {
	struct cls super;
};

struct R_Encoder {
	void (*encode_step)(
		int *inputIdx,
		const char *input,
		int *outputIdx,
		char *output
	);
	void (*decode_step)(
		int *inputIdx,
		const char *input,
		int *outputIdx,
		char *output
	);
	struct EncoderCls *cls;
};

struct _Encoder {
	struct R_Encoder r;
	struct Encoder i;
};

static char *translate(
	const char *input,
	void (*step)(int *, const char *, int*, char *))
{
	if (!input) return NULL;

	int i = 0, j = 0;
	size_t len = 8 * sizeof(char);

	char *buffer = malloc(len);
	if (!buffer) die(malloc_err);
	memset(buffer, 0, len);

	while (input[i]) {
		if (j > len - 2) {
			len += len;
			buffer = realloc(buffer, len);
			if (!buffer) die(malloc_err);
			memset(buffer + (len / 2), 0, len / 2);
		}

		// with great power comes great responsibility
		step(&i, input, &j, buffer);
	}

	return buffer;
}

static char *encode(struct Encoder *_self, const char *input)
{
	struct R_Encoder *self = (void *)_self - sizeof(struct R_Encoder);
	return translate(input, self->encode_step);
}

static char *decode(struct Encoder *_self, const char *input)
{
	struct R_Encoder *self = (void *)_self - sizeof(struct R_Encoder);
	return translate(input, self->decode_step);
}

static void *EncoderCtor(void *_self, va_list *ap)
{
	struct _Encoder *self = _self;
	self->r.encode_step = va_arg(
		*ap, void (*)(int *, const char *, int *, char *)
	);
	self->r.decode_step = va_arg(
		*ap, void (*)(int *, const char *, int *, char *)
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
