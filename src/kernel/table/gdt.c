#include "gdt.h"

struct gdt_entry gdt[3];
struct gdt_ptr gp;

extern void gdt_flush(unsigned int);

void gdt_set_gate(int i, unsigned int b, unsigned int l, unsigned char a, unsigned char g) {
    gdt[i].base_low = (b & 0xFFFF);
    gdt[i].base_middle = (b >> 16) & 0xFF;
    gdt[i].base_high = (b >> 24) & 0xFF;
    gdt[i].limit_low = (l & 0xFFFF);
    gdt[i].granularity = ((l >> 16) & 0x0F) | (g & 0xF0);
    gdt[i].access = a;
}

void init_gdt(void) {
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gp.base = (unsigned int)&gdt;

    gdt_set_gate(0, 0, 0, 0, 0);
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    gdt_flush((unsigned int)&gp);
}

void gdt_get_info(int i, struct gdt_info *inf) {
    if (i < 0 || i >= 3) return;
    inf->base = gdt[i].base_low | (gdt[i].base_middle << 16) | (gdt[i].base_high << 24);
    inf->limit = gdt[i].limit_low | ((gdt[i].granularity & 0x0F) << 16);
    inf->access = gdt[i].access;
    inf->gran = gdt[i].granularity & 0xF0;
}