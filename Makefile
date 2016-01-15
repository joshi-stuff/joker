###############################################################################
# Configuration
###############################################################################
TARGET=i686-elf
BUILD=build

###############################################################################
# Tools
###############################################################################
GCC=/Users/ivan/Desarrollo/Ivan/OS/usr/bin/$(TARGET)-gcc
AS=/Users/ivan/Desarrollo/Ivan/OS/usr/bin/$(TARGET)-as


###############################################################################
# Modules
###############################################################################
COMPILE_FLAGS=-std=gnu99 -ffreestanding -DFREE_STANDING -O0
LINK_FLAGS=-ffreestanding -O2 -nostdlib -lgcc

LIBC=libc
LIBC_GCC_FLAGS=$(COMPILE_FLAGS) -I$(LIBC)
LIBC_OBJS=libc.o

DUKTAPE=duktape
DUKTAPE_GCC_FLAGS=$(COMPILE_FLAGS) -I$(LIBC)
DUKTAPE_OBJS=duktape.o

KERNEL=kernel
KERNEL_GCC_FLAGS=$(COMPILE_FLAGS) -I$(LIBC) -I$(DUKTAPE)
KERNEL_OBJS=kernel.o


###############################################################################
# Main targets
###############################################################################
link: compile
	$(GCC) -T $(KERNEL)/kernel.ld -o $(BUILD)/kernel.bin $(BUILD)/*/*.o $(LINK_FLAGS)

compile: mk_build compile_libc compile_duktape compile_kernel

compile_libc: $(patsubst %, $(BUILD)/$(LIBC)/%, $(LIBC_OBJS))

compile_duktape: $(patsubst %, $(BUILD)/$(DUKTAPE)/%, $(DUKTAPE_OBJS))
	
compile_kernel: $(patsubst %, $(BUILD)/$(KERNEL)/%, $(KERNEL_OBJS))
	
clean:
	rm -rf $(BUILD)


###############################################################################
# Auxiliary targets
###############################################################################
mk_build:
	+@[ -d $(BUILD) ] || mkdir -p $(BUILD)
	+@[ -d $(BUILD)/$(LIBC) ] || mkdir -p $(BUILD)/$(LIBC)
	+@[ -d $(BUILD)/$(DUKTAPE) ] || mkdir -p $(BUILD)/$(DUKTAPE)
	+@[ -d $(BUILD)/$(KERNEL) ] || mkdir -p $(BUILD)/$(KERNEL)


###############################################################################
# Compilation rules
###############################################################################
$(BUILD)/$(LIBC)/%.o: $(LIBC)/%.c 
	$(GCC) -o $@ -c $< $(LIBC_GCC_FLAGS) 

$(BUILD)/$(DUKTAPE)/%.o: $(DUKTAPE)/%.c 
	$(GCC) -o $@ -c $< $(DUKTAPE_GCC_FLAGS) 

$(BUILD)/$(KERNEL)/%.o: $(KERNEL)/%.c 
	$(GCC) -o $@ -c $< $(KERNEL_GCC_FLAGS) 
