#ifndef CLASS_H
#define CLASS_H

#include <stdlib.h>
#include <stdarg.h>

struct cls {
	size_t size;
	void * (*ctor)(void *self, va_list *ap);
	void * (*dtor)(void *self);
};

void *new(const void *cls, ...);
const struct cls *get_cls(void *obj);
void delete(void *obj);

#endif
