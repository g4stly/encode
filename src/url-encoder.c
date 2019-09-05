#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <class.h>

#include "url-encoder.h"
#include "util.h"

static const char *digits = "0123456789ABCDEF";
static const char *malloc_err = "UrlEncoder->decode(): malloc():";
static const char *strtol_err = "UrlEncoder->encode(): strtol():";
static const char *bad_code_err = "UrlEncoder->decode(): bad code: %%%s\n";

// class definition
struct UrlEncoderCls {
	struct cls super;
};

// representation
struct R_UrlEncoder {
	struct UrlEncoderCls *cls;
};

// complete instance struct
struct _UrlEncoder {
	struct R_UrlEncoder r;	// representation
	struct UrlEncoder i;	// interface
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

static char *encode(struct UrlEncoder *_self, const char *input)
{
	if (!input) return NULL;

	char c;
	int i = 0, j = 0;
	size_t len = 8 * sizeof(char);

	char *buffer = malloc(len);
	if (!buffer) die(malloc_err);
	memset(buffer, 0, len);

	while ((c = input[i++])) {
		if (j > len - 2) {
			len += len;
			buffer = realloc(buffer, len);
			if (!buffer) die(malloc_err);
			memset(buffer + (len / 2), 0, len / 2);
		}

		if (!unsafe(c)) {
			buffer[j++] = c;
			continue;
		}

		if (c < 0) c = 0;

		buffer[j++] = '%';
		buffer[j++] = digits[(c / 16) % 16];
		buffer[j++] = digits[(c) % 16];
	}

	return buffer;
}

static char *decode(struct UrlEncoder *_self, const char *input)
{
	if (!input) return NULL;

	char c;
	int i = 0, j = 0;
	char temp[] = { 0, 0, 0 };
	size_t len = 8 * sizeof(char);

	char *buffer = malloc(len);
	if (!buffer) die(malloc_err);
	memset(buffer, 0, len);

	while ((c = input[i++])) {
		if (j > len - 2) {
			len += len;
			buffer = realloc(buffer, len);
			if (!buffer) die(malloc_err);
			memset(buffer + (len / 2), 0, len / 2);
		}

		if (c != '%') {
			buffer[j++] = c;
			continue;
		}

		errno = 0;
		temp[0] = input[i++];
		temp[1] = input[i++];

		buffer[j++] = strtol(temp, NULL, 16);
		if (!buffer[j - 1]) die(bad_code_err, temp);
		if (errno) die(strtol_err);
	}

	return buffer;
}

static void *UrlEncoderCtor(void *_self, va_list *ap)
{
	struct _UrlEncoder *self = _self;
	self->i.encode = encode;
	self->i.decode = decode;
	return &self->i;
}

static void *UrlEncoderDtor(void *self)
{
	return self - sizeof(struct R_UrlEncoder);
}

static const struct UrlEncoderCls _UrlEncoder = {
	{
		sizeof(struct _UrlEncoder),
		UrlEncoderCtor,
		UrlEncoderDtor
	}
};

const void *UrlEncoder = &_UrlEncoder;
