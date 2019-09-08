#ifndef TEXT_H_
#define TEXT_H_

struct Text {
	long (*getSize)(struct Text *self);
	const char *(*getBytes)(struct Text *self);
};

extern const void *Text;

#endif
