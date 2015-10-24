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

SYSTEM=system
SYSTEM_GCC_FLAGS=$(COMPILE_FLAGS) -I$(SYSTEM)
SYSTEM_OBJS=system.o

DUKTAPE=duktape
DUKTAPE_GCC_FLAGS=$(COMPILE_FLAGS) -I$(SYSTEM)
DUKTAPE_OBJS=duktape.o

KERNEL=kernel
KERNEL_GCC_FLAGS=$(COMPILE_FLAGS) -I$(SYSTEM) -I$(DUKTAPE)
KERNEL_OBJS=kernel.o


###############################################################################
# Main targets
###############################################################################
link: compile
	$(GCC) -T $(KERNEL)/kernel.ld -o $(BUILD)/kernel.bin $(BUILD)/system/*.o $(BUILD)/duktape/*.o $(BUILD)/kernel/*.o $(LINK_FLAGS)

compile: mk_build compile_system compile_duktape compile_kernel

compile_system: $(patsubst %, $(BUILD)/$(SYSTEM)/%, $(SYSTEM_OBJS))

compile_duktape: $(patsubst %, $(BUILD)/$(DUKTAPE)/%, $(DUKTAPE_OBJS))
	
compile_kernel: $(patsubst %, $(BUILD)/$(KERNEL)/%, $(KERNEL_OBJS))
	
clean:
	rm -rf $(BUILD)


###############################################################################
# Auxiliary targets
###############################################################################
mk_build:
	+@[ -d $(BUILD) ] || mkdir -p $(BUILD)
	+@[ -d $(BUILD)/$(SYSTEM) ] || mkdir -p $(BUILD)/$(SYSTEM)
	+@[ -d $(BUILD)/$(DUKTAPE) ] || mkdir -p $(BUILD)/$(DUKTAPE)
	+@[ -d $(BUILD)/$(KERNEL) ] || mkdir -p $(BUILD)/$(KERNEL)


###############################################################################
# Compilation rules
###############################################################################
$(BUILD)/$(SYSTEM)/%.o: $(SYSTEM)/%.c 
	$(GCC) -o $@ -c $< $(SYSTEM_GCC_FLAGS) 

$(BUILD)/$(DUKTAPE)/%.o: $(DUKTAPE)/%.c 
	$(GCC) -o $@ -c $< $(DUKTAPE_GCC_FLAGS) 

$(BUILD)/$(KERNEL)/%.o: $(KERNEL)/%.c 
	$(GCC) -o $@ -c $< $(KERNEL_GCC_FLAGS) 
