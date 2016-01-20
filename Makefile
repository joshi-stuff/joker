###############################################################################
# Configuration
###############################################################################
TARGET=i686-elf

###############################################################################
# Tools
###############################################################################
TOOLS=/Users/ivan/Desarrollo/Ivan/OS/usr/bin
GCC=$(TOOLS)/$(TARGET)-gcc
AS=$(TOOLS)/$(TARGET)-as
GRUB_MKRESCUE=$(TOOLS)/grub-mkrescue
XORRISO=$(TOOLS)/xorriso


###############################################################################
# Modules
###############################################################################
ASSEMBLE_FLAGS=-asm=intel
COMPILE_FLAGS=-std=gnu99 -ffreestanding -DFREE_STANDING -O0 -masm=intel
LINK_FLAGS=-ffreestanding -O2 -nostdlib -lgcc

LIBC_SOURCES=libc.c stdio.c string.c _helpers.c 
LIBC_GCC_FLAGS=$(COMPILE_FLAGS) -Ilibc -Ikernel

DUKTAPE_SOURCES=duktape.c
DUKTAPE_GCC_FLAGS=$(COMPILE_FLAGS) -Ilibc

KERNEL_SOURCES=boot.s kernel.c bios.c
KERNEL_AS_FLAGS=$(ASSEMBLE_FLAGS)
KERNEL_GCC_FLAGS=$(COMPILE_FLAGS) -Ilibc -Iduktape

ISO_GRUB_MKRESCUE_FLAGS=--xorriso=$(XORRISO)


###############################################################################
# Auxiliary vars
###############################################################################
LIBC_OBJS=$(patsubst %, build/libc/%.o, $(LIBC_SOURCES))
DUKTAPE_OBJS=$(patsubst %, build/duktape/%.o, $(DUKTAPE_SOURCES))
KERNEL_OBJS=$(patsubst %, build/kernel/%.o, $(KERNEL_SOURCES))


###############################################################################
# Alias targets
###############################################################################
iso: build/kernel.iso
	
kernel: build/kernel.bin
	
compile: compile_libc compile_duktape compile_kernel

compile_libc: $(LIBC_OBJS)

compile_duktape: $(DUKTAPE_OBJS)
	
compile_kernel: $(KERNEL_OBJS)
	
clean:
	rm -rf build

qemu: iso
	qemu-system-i386 build/kernel.iso


###############################################################################
# Artifact targets
###############################################################################
build/kernel.iso: build/iso/boot/grub/grub.cfg build/iso/boot/kernel.bin 
	+@[ -d build ] || mkdir -p build
	$(GRUB_MKRESCUE) -o build/kernel.iso build/iso $(ISO_GRUB_MKRESCUE_FLAGS) 
	
build/iso/boot/grub/grub.cfg: iso/grub.cfg
	+@[ -d build/iso/boot/grub ] || mkdir -p build/iso/boot/grub
	cp iso/grub.cfg build/iso/boot/grub/grub.cfg
	
build/iso/boot/kernel.bin: build/kernel.bin
	+@[ -d build/iso/boot ] || mkdir -p build/iso/boot
	cp build/kernel.bin build/iso/boot/kernel.bin

build/kernel.bin: $(LIBC_OBJS) $(DUKTAPE_OBJS) $(KERNEL_OBJS)
	+@[ -d build ] || mkdir -p build	
	$(GCC) -T kernel/kernel.ld -o build/kernel.bin $(LIBC_OBJS) $(DUKTAPE_OBJS) $(KERNEL_OBJS) $(LINK_FLAGS)
	
build/libc/%.c.o: libc/%.c
	+@[ -d build/libc ] || mkdir -p build/libc
	$(GCC) -o $@ -c $< $(LIBC_GCC_FLAGS) 

build/duktape/%.c.o: duktape/%.c
	+@[ -d build/duktape ] || mkdir -p build/duktape
	$(GCC) -o $@ -c $< $(DUKTAPE_GCC_FLAGS) 

build/kernel/%.c.o: kernel/%.c
	+@[ -d build/kernel ] || mkdir -p build/kernel
	$(GCC) -o $@ -c $< $(KERNEL_GCC_FLAGS) 

build/kernel/%.s.o: kernel/%.s
	+@[ -d build/kernel ] || mkdir -p build/kernel
	$(AS) -o $@ -c $< $(KERNEL_AS_FLAGS) 
