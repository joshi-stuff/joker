SRC_DIR=src
UCLIBC_DIR=$(LIBC_DIR)/uClibc
LIBC_DIR=$(SRC_DIR)/libc
DUKTAPE_DIR=$(SRC_DIR)/duktape
KERNEL_DIR=$(SRC_DIR)/kernel
DRIVERS_DIR=$(SRC_DIR)/drivers
GRUB_DIR=$(SRC_DIR)/grub

BUILD_DIR=build
COMPILE_DIR=$(BUILD_DIR)/compile
ISO_DIR=$(BUILD_DIR)/iso
OUT_DIR=out

CFLAGS=-std=gnu99 -ffreestanding -O0 -Wall -Wextra -DFREE_STANDING -Wno-unused-parameter
LDFLAGS=-ffreestanding -O2 -nostdlib -lgcc

KERNEL_FLAGS=$(CFLAGS) -I$(KERNEL_DIR) -masm=intel
DRIVERS_FLAGS=$(CFLAGS) -I$(KERNEL_DIR) -I$(DRIVERS_DIR) -masm=intel

UCLIBC_FLAGS=$(CFLAGS) -I$(UCLIBC_DIR)/stub_include -I$(UCLIBC_DIR)/fake_include -I$(LIBC_DIR)
LIBC_FLAGS=$(CFLAGS)
DUKTAPE_FLAGS=$(CFLAGS) -I$(DUKTAPE_DIR)/stub_include -I$(DUKTAPE_DIR)/fake_include -I$(LIBC_DIR) -DDUK_OPT_HAVE_CUSTOM_H

ASM=-S -masm=intel

all: clean compile link iso

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(OUT_DIR)

mkdirs:
	[ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR)
	[ -d $(COMPILE_DIR) ] || mkdir -p $(COMPILE_DIR)
	[ -d $(ISO_DIR)/boot/grub ] || mkdir -p $(ISO_DIR)/boot/grub
	[ -d $(OUT_DIR) ] || mkdir -p $(OUT_DIR)

compile: compile_kernel compile_drivers #compile_libc compile_duktape

compile_kernel: mkdirs
	i686-elf-as $(KERNEL_DIR)/impl/init.s -o $(COMPILE_DIR)/init.o
	i686-elf-gcc -c $(KERNEL_DIR)/impl/kernel.c -o $(COMPILE_DIR)/kernel.o $(KERNEL_FLAGS)

compile_drivers: compile_kernel
	i686-elf-gcc -c $(DRIVERS_DIR)/cpu/x386.c -o $(COMPILE_DIR)/cpu_x386.o $(DRIVERS_FLAGS)
	i686-elf-gcc -c $(DRIVERS_DIR)/cpu/idt_handler_bridges.c -o $(COMPILE_DIR)/cpu_idt_handler_bridges.c.o $(KERNEL_FLAGS)
	i686-elf-as     $(DRIVERS_DIR)/cpu/idt_handler_bridges.s -o $(COMPILE_DIR)/cpu_idt_handler_bridges.s.o
	i686-elf-gcc -c $(DRIVERS_DIR)/util/debug.c -o $(COMPILE_DIR)/util_debug.o $(DRIVERS_FLAGS)
	i686-elf-gcc -c $(DRIVERS_DIR)/util/string.c -o $(COMPILE_DIR)/util_str.o $(DRIVERS_FLAGS)
	i686-elf-gcc -c $(DRIVERS_DIR)/video/vga.c -o $(COMPILE_DIR)/video_vga.o $(DRIVERS_FLAGS)

compile_libc: compile_drivers
	i686-elf-gcc -c $(LIBC_DIR)/alloc.c -o $(COMPILE_DIR)/alloc.o $(LIBC_FLAGS)
	i686-elf-gcc -c $(LIBC_DIR)/control.c -o $(COMPILE_DIR)/control.o $(LIBC_FLAGS)
	i686-elf-gcc -c $(LIBC_DIR)/file.c -o $(COMPILE_DIR)/file.o $(LIBC_FLAGS)
	i686-elf-gcc -c $(LIBC_DIR)/format.c -o $(COMPILE_DIR)/format.o $(LIBC_FLAGS)
	i686-elf-gcc -c $(LIBC_DIR)/time.c -o $(COMPILE_DIR)/time.o $(LIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/memcpy.c -o $(COMPILE_DIR)/memcpy.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/memmove.c -o $(COMPILE_DIR)/memmove.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/memset.c -o $(COMPILE_DIR)/memset.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/memcmp.c -o $(COMPILE_DIR)/memcmp.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/strcmp.c -o $(COMPILE_DIR)/strcmp.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/strlen.c -o $(COMPILE_DIR)/strlen.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/strncmp.c -o $(COMPILE_DIR)/strncmp.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/s_isnan.c -o $(COMPILE_DIR)/s_isnan.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/s_isnanf.c -o $(COMPILE_DIR)/s_isnanf.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/s_floor.c -o $(COMPILE_DIR)/s_floor.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/e_fmod.c -o $(COMPILE_DIR)/e_fmod.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/s_fmin.c -o $(COMPILE_DIR)/s_fmin.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/s_fmax.c -o $(COMPILE_DIR)/s_fmax.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/s_fabs.c -o $(COMPILE_DIR)/s_fabs.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/e_pow.c -o $(COMPILE_DIR)/e_pow.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/e_acos.c -o $(COMPILE_DIR)/e_acos.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/e_asin.c -o $(COMPILE_DIR)/e_asin.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/s_atan.c -o $(COMPILE_DIR)/s_atan.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/s_ceil.c -o $(COMPILE_DIR)/s_ceil.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/s_cos.c -o $(COMPILE_DIR)/s_cos.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/s_sin.c -o $(COMPILE_DIR)/s_sin.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/e_exp.c -o $(COMPILE_DIR)/e_exp.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/e_log.c -o $(COMPILE_DIR)/e_log.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/e_sqrt.c -o $(COMPILE_DIR)/e_sqrt.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/s_tan.c -o $(COMPILE_DIR)/s_tan.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/e_atan2.c -o $(COMPILE_DIR)/e_atan2.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/s_scalbn.c -o $(COMPILE_DIR)/s_scalbn.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/s_copysign.c -o $(COMPILE_DIR)/s_copysign.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/s_finite.c -o $(COMPILE_DIR)/s_finite.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/s_finitef.c -o $(COMPILE_DIR)/s_finitef.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/s_matherr.c -o $(COMPILE_DIR)/s_matherr.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/s_rint.c -o $(COMPILE_DIR)/s_rint.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/ldouble_wrappers.c -o $(COMPILE_DIR)/ldouble_wrappers.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/k_standard.c -o $(COMPILE_DIR)/k_standard.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/k_sin.c -o $(COMPILE_DIR)/k_sin.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/k_cos.c -o $(COMPILE_DIR)/k_cos.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/k_tan.c -o $(COMPILE_DIR)/k_tan.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/e_rem_pio2.c -o $(COMPILE_DIR)/e_rem_pio2.o $(UCLIBC_FLAGS)
	i686-elf-gcc -c $(UCLIBC_DIR)/k_rem_pio2.c -o $(COMPILE_DIR)/k_rem_pio2.o $(UCLIBC_FLAGS)

compile_duktape: compile_libc
	i686-elf-gcc -c $(DUKTAPE_DIR)/duktape.c -o $(COMPILE_DIR)/duktape.o $(DUKTAPE_FLAGS)

link: compile
	i686-elf-gcc -T $(KERNEL_DIR)/impl/joker.ld -o $(OUT_DIR)/joker.bin $(COMPILE_DIR)/*.o $(LDFLAGS)

iso: link
	cp $(OUT_DIR)/joker.bin $(ISO_DIR)/boot
	cp $(GRUB_DIR)/grub.cfg $(ISO_DIR)/boot/grub
	grub-mkrescue -o $(OUT_DIR)/joker.iso $(BUILD_DIR)/iso

qemu: iso
	qemu-system-i386 -cdrom $(OUT_DIR)/joker.iso
