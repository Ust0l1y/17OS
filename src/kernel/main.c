#include "gdt.h"
#include "stdio.h"

void kmain(void) {
    init_gdt();
    clean();

    printf("%C17OS Booted!\n\n", 0x0A);
    
    printf("%C--- Format Test ---\n", 0x0E);
    printf("%CNum: %d\nHex: 0x%x\nBin: 0b%b\nLL:  %lld\nStr: %U %D\n\n",
           0x07, -1337, 0xDEADBEEF, 42, 12345678987654321LL, "upper", "LOWER");

    printf("%C--- GDT Info ---\n", 0x0E);
    struct gdt_info inf;
    for (int i = 0; i < 3; ++i) {
        gdt_get_info(i, &inf);
        printf("%CGDT %d: B=0x%x L=0x%x A=0x%x G=0x%x\n",
               0x0F, i, inf.base, inf.limit, inf.access, inf.gran);
    }

    while(1);
}