#ifndef _INPUT_H_
#define _INPUT_H_

#include "text.h"

struct Text *readStdin();
struct Text *readFile(const char *filename);

#endif
