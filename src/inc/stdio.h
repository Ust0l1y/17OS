#ifndef STDIO_H
#define STDIO_H

#include "stdarg.h"

void clean(void);
void putc(char c);
void puts(const char *s);
void printf(const char *fmt, ...);

#endif