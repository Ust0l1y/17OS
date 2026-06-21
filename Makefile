AS = nasm
CC = gcc
LD = ld
VM = qemu-system-i386

AFLAGS = -f elf32
CFLAGS = -m32 -ffreestanding -fno-pic -fno-stack-protector -nostdlib
LDFLAGS = -m elf_i386 -T linker.ld

OBJ = boot.o main.o

all: os.iso

boot.o: src/boot/boot.asm src/boot/header.asm
	$(AS) $(AFLAGS) src/boot/boot.asm -o $@

main.o: src/kernel/main.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel.bin: $(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) -o $@

os.iso: kernel.bin
	mkdir -p iso/boot/grub
	cp kernel.bin iso/boot/kernel.bin
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo 'menuentry "OS" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/kernel.bin' >> iso/boot/grub/grub.cfg
	echo '  boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue -o os.iso iso
	rm -rf iso

run: os.iso
	$(VM) -cdrom os.iso

clean:
	rm -f *.o kernel.bin os.iso
	rm -rf iso