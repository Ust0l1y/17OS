#include "stdio.h"

#define MEM 0xB8000
#define W 80
#define H 25

static int x = 0;
static int y = 0;
static unsigned char attr = 0x0F;

void clean(void) {
    char *v = (char*)MEM;
    for (int i = 0; i < W * H * 2; i += 2) {
        v[i] = ' ';
        v[i+1] = attr;
    }
    x = 0; y = 0;
}

static void scroll(void) {
    char *v = (char*)MEM;
    for (int i = 0; i < (H - 1) * W * 2; i++) {
        v[i] = v[i + W * 2];
    }
    for (int i = (H - 1) * W * 2; i < H * W * 2; i += 2) {
        v[i] = ' ';
        v[i+1] = attr;
    }
    y = H - 1;
}

void putc(char c) {
    char *v = (char*)MEM;
    if (c == '\n') {
        x = 0;
        if (++y >= H) scroll();
        return;
    }
    if (c == '\r') {
        x = 0;
        return;
    }
    
    int p = (y * W + x) * 2;
    v[p] = c;
    v[p+1] = attr;
    
    if (++x >= W) {
        x = 0;
        if (++y >= H) scroll();
    }
}

void puts(const char *s) {
    while (*s) putc(*s++);
}

static void pnum(unsigned long long u, unsigned int b, int sign) {
    char buf[64];
    int i = 0;
    if (sign) putc('-');
    
    do {
        unsigned long long r = 0, res = 0;
        for (int bit = 63; bit >= 0; bit--) {
            r = (r << 1) | ((u >> bit) & 1);
            res <<= 1;
            if (r >= b) {
                r -= b;
                res |= 1;
            }
        }
        buf[i++] = (r < 10) ? (r + '0') : (r - 10 + 'a');
        u = res;
    } while (u > 0);
    
    while (i > 0) putc(buf[--i]);
}

static void pcase(const char *s, int m) {
    while (*s) {
        char c = *s++;
        if (m == 1 && c >= 'a' && c <= 'z') c -= 32;
        else if (m == 2 && c >= 'A' && c <= 'Z') c += 32;
        putc(c);
    }
}

void printf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    
    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            if (*fmt == 'c') putc((char)va_arg(ap, int));
            else if (*fmt == 's') puts(va_arg(ap, char*));
            else if (*fmt == 'd') {
                int n = va_arg(ap, int);
                pnum(n < 0 ? -n : n, 10, n < 0);
            }
            else if (*fmt == 'x') pnum(va_arg(ap, unsigned int), 16, 0);
            else if (*fmt == 'b') pnum(va_arg(ap, unsigned int), 2, 0);
            else if (*fmt == 'C') attr = (unsigned char)va_arg(ap, int);
            else if (*fmt == 'U') pcase(va_arg(ap, char*), 1);
            else if (*fmt == 'D') pcase(va_arg(ap, char*), 2);
            else if (*fmt == 'l' && *(fmt+1) == 'l' && *(fmt+2) == 'd') {
                long long n = va_arg(ap, long long);
                pnum(n < 0 ? -n : n, 10, n < 0);
                fmt += 2;
            }
            else if (*fmt == '%') putc('%');
        } else {
            putc(*fmt);
        }
        fmt++;
    }
    va_end(ap);
}