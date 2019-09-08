#include <stdarg.h>
#include <stdio.h>
#include <class.h>

#include "text.h"

struct TextCls {
	struct cls super;
};

struct R_Text {
	long size;
	char *bytes;
	struct TextCls *cls;
};

struct _Text {
	struct R_Text r;
	struct Text i;
};

static long getSize(struct Text *_self)
{
	struct _Text *self = (void *)_self - sizeof(struct R_Text);
	return self->r.size;
}

static const char *getBytes(struct Text *_self)
{
	struct _Text *self = (void *)_self - sizeof(struct R_Text);
	return self->r.bytes;
}

static void *TextCtor(void *_self, va_list *ap)
{
	struct _Text *self = _self;

	self->r.bytes = va_arg(*ap, char *);
	self->r.size = va_arg(*ap, long);

	self->i.getBytes = getBytes;
	self->i.getSize = getSize;

	return &self->i;
}

static void *TextDtor(void *_self)
{
	struct _Text *self = (void *)_self - sizeof(struct R_Text);
	free(self->r.bytes);
	return self;
}


static struct TextCls _Text = {
	{
		sizeof(struct _Text),
		TextCtor,
		TextDtor
	}
};
const void *Text = &_Text;
