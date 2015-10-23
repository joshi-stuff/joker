SRC_DIR=src

BUILD_DIR=build
OUT_DIR=out

KERNEL_FILE=jokernel.bin
ISO_FILE=jokernel.bin

all: clean iso

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(OUT_DIR)

mkdirs:
	[ -d $(BUILD_DIR) ] || mkdir $(BUILD_DIR)
	[ -d $(OUT_DIR) ] || mkdir $(OUT_DIR)

compile: mkdirs
	[ -d $(BUILD_DIR)/compile ] || mkdir $(BUILD_DIR)/compile
	i686-elf-as $(SRC_DIR)/kernel/boot.s -o $(BUILD_DIR)/compile/boot.o
	i686-elf-gcc -c $(SRC_DIR)/kernel/kernel.c -o $(BUILD_DIR)/compile/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	i686-elf-gcc -c $(SRC_DIR)/libc/libc.c -o $(BUILD_DIR)/compile/libc.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -DFREE_STANDING
	i686-elf-gcc -c $(SRC_DIR)/duktape/duktape.c -o $(BUILD_DIR)/compile/duktape.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -DFREE_STANDING -I$(SRC_DIR)/libc

link: compile
	i686-elf-gcc -T $(SRC_DIR)/kernel/linker.ld -o $(OUT_DIR)/$(KERNEL_FILE) -ffreestanding -O2 -nostdlib $(BUILD_DIR)/compile/*.o -lgcc

iso: link
	[ -d $(BUILD_DIR)/iso/boot/grub ] || mkdir -p $(BUILD_DIR)/iso/boot/grub
	cp $(OUT_DIR)/$(KERNEL_FILE) $(BUILD_DIR)/iso/boot
	cp $(SRC_DIR)/grub/grub.cfg $(BUILD_DIR)/iso/boot/grub
	grub-mkrescue -o $(OUT_DIR)/joker.iso $(BUILD_DIR)/iso

qemu: iso
	qemu-system-i386 -cdrom $(OUT_DIR)/joker.iso
