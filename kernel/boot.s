# Declare constants used for creating a multiboot header.

.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set VIDINFO,  1<<2             # provide header graphics info
.set ADDRINFO, 1<<16						# provide header address info

.set FLAGS,    ALIGN | MEMINFO | VIDINFO
.set MAGIC,    0x1BADB002       
.set CHECKSUM, -(MAGIC + FLAGS)

.set HDRADDR, 0
.set LOADADDR, 0
.set LOADENDADDR, 0
.set BSSENDADDR, 0
.set ENTRYADDR, 0

.set VIDMODE, 1  								# 0=graphics mode, 1=text mode 
.set VIDWIDTH, 80								
.set VIDHEIGHT, 43
.set VIDDEPTH, 0


# Declare a header as in the Multiboot Standard

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
.long HDRADDR
.long LOADADDR
.long LOADENDADDR
.long BSSENDADDR
.long ENTRYADDR
.long VIDMODE
.long VIDWIDTH
.long VIDHEIGHT
.long VIDDEPTH


# Currently the stack pointer register (esp) points at anything and using it may
# cause massive harm. Instead, we'll provide our own stack. We will allocate
# room for a small temporary stack by creating a symbol at the bottom of it,
# then allocating 16384 bytes for it, and finally creating a symbol at the top.

.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 1048576 # 1MB
stack_top:
.global stack_bottom, stack_top

# Reserve a temporary working area for creating the memory map without destroying
# valueble data in limbo
.section .twa, "aw", @nobits
twa_bottom:
.skip 1048576 # 1MB
twa_top:
.global twa_bottom, twa_top

# The linker script specifies _start as the entry point to the kernel and the
# bootloader will jump to this position once the kernel has been loaded. It
# doesn't make sense to return from this function as the bootloader is gone.

.section .text
.global _start
.type _start, @function
_start:

	# To set up a stack, we simply set the esp register to point to the top of
	# our stack (as it grows downwards).
	movl $stack_top, %esp

	# Push address of multiboot information and magic number and call main()
  push %esp
	push %eax
	push %ebx
	call main

	# In case the function returns, we'll want to put the computer into an
	# infinite loop. To do that, we use the clear interrupt ('cli') instruction
	# to disable interrupts, the halt instruction ('hlt') to stop the CPU until
	# the next interrupt arrives, and jumping to the halt instruction if it ever
	# continues execution, just to be safe. We will create a local label rather
	# than real symbol and jump to there endlessly.
	cli
.Lhang:
	hlt
	jmp .Lhang


# Set the size of the _start symbol to the current location '.' minus its start.
# This is useful when debugging or when you implement call tracing.
.size _start, . - _start
