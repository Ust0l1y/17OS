void kmain(void) {
    char *v = (char*)0xB8000;
    v[0] = 'O';
    v[1] = 0x17;
    v[2] = 'S';
    v[3] = 0x17;
    while(1);
}