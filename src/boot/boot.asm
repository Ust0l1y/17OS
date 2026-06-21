bits 32

section .multiboot
align 4
    magic   equ 0x1BADB002
    flags   equ 0x00
    chksum  equ -(magic + flags)

    dd magic
    dd flags
    dd chksum

section .text
global start
global gdt_flush
extern kmain
extern init_gdt

start:
    cli
    mov esp, stk
    call init_gdt
    call kmain
    hlt

gdt_flush:
    mov eax, [esp + 4]
    lgdt [eax]
    jmp 0x08:.reload
.reload:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret

section .bss
align 16
resb 8192
stk: