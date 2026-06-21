#ifndef STRING_H
#define STRING_H

typedef unsigned int size_t;

void* memcpy(void *dst, const void *src, size_t n);
void* memset(void *dst, int c, size_t n);
size_t strlen(const char *s);
int strcmp(const char *s1, const char *s2);

#endif