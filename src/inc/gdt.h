#ifndef GDT_H
#define GDT_H

struct gdt_entry {
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char  base_middle;
    unsigned char  access;
    unsigned char  granularity;
    unsigned char  base_high;
} __attribute__((packed));

struct gdt_ptr {
    unsigned short limit;
    unsigned int   base;
} __attribute__((packed));

struct gdt_info {
    unsigned int base;
    unsigned int limit;
    unsigned char access;
    unsigned char gran;
} __attribute__((packed));

void init_gdt(void);
void gdt_get_info(int i, struct gdt_info *inf);

#endif