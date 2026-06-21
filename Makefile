AS = nasm
CC = gcc
LD = ld
VM = qemu-system-i386

AFLAGS = -f elf32
CFLAGS = -m32 -O3 -march=i686 -ffreestanding -fno-pic -fno-stack-protector \
         -fomit-frame-pointer -fno-asynchronous-unwind-tables -nostdlib -Isrc/inc
LDFLAGS = -m elf_i386 -T linker.ld --gc-sections

SRCS_ASM = $(shell find src -name "*.asm")
SRCS_C   = $(shell find src -name "*.c")

OBJ = $(patsubst %.asm, bin/%.o, $(notdir $(SRCS_ASM))) \
      $(patsubst %.c, bin/%.o, $(notdir $(SRCS_C)))

vpath %.asm $(sort $(dir $(SRCS_ASM)))
vpath %.c $(sort $(dir $(SRCS_C)))

all: os.iso

bin/%.o: %.asm
	@mkdir -p bin
	$(AS) $(AFLAGS) $< -o $@

bin/%.o: %.c
	@mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@

kernel.bin: $(OBJ)
	$(LD) $(LDFLAGS) bin/boot.o $(filter-out bin/boot.o, $(OBJ)) -o $@

os.iso: kernel.bin
	mkdir -p iso/boot/grub
	cp kernel.bin iso/boot/kernel.bin
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo 'menuentry "17OS" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/kernel.bin' >> iso/boot/grub/grub.cfg
	echo '  boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue -o os.iso iso
	rm -rf iso

run: os.iso
	$(VM) -cdrom os.iso

clean:
	rm -rf bin kernel.bin os.iso iso