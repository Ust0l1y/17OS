section .multiboot
%include "src/boot/header.asm"

section .text
bits 32
global start
extern kmain

start:
    cli
    mov esp, stk
    call kmain
    hlt

section .bss
align 16
resb 8192
stk: